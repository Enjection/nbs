#include "tablet_actor.h"

#include <cloud/filestore/libs/diagnostics/throttler_info_serializer.h>
#include <cloud/filestore/libs/diagnostics/trace_serializer.h>
#include <cloud/filestore/libs/storage/tablet/model/split_range.h>

#include <library/cpp/actors/core/actor_bootstrapped.h>

#include <util/generic/algorithm.h>
#include <util/generic/cast.h>
#include <util/generic/hash.h>
#include <util/generic/set.h>
#include <util/generic/string.h>
#include <util/generic/vector.h>

namespace NCloud::NFileStore::NStorage {

using namespace NActors;

using namespace NKikimr;
using namespace NKikimr::NTabletFlatExecutor;

namespace {

////////////////////////////////////////////////////////////////////////////////

bool ShouldReadBlobs(const TVector<TBlockDataRef>& blocks)
{
    for (const auto& block: blocks) {
        if (block.BlobId) {
            return true;
        }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////

void CopyFileData(
    const TString& LogTag,
    const TByteRange origin,
    const TByteRange aligned,
    const ui64 fileSize,
    TStringBuf content,
    TString* out)
{
    auto end = Min(fileSize, origin.End());
    if (end < aligned.End()) {
        ui64 delta = Min(aligned.End() - end, content.size());
        content.Chop(delta);
    }

    TABLET_VERIFY(origin.Offset >= aligned.Offset);
    content.Skip(origin.Offset - aligned.Offset);

    out->assign(content.data(), content.size());
}

////////////////////////////////////////////////////////////////////////////////

void ApplyBytes(
    const TString& LogTag,
    const TByteRange& byteRange,
    TVector<TBlockBytes> bytes,
    IBlockBuffer& buffer)
{
    TABLET_VERIFY(byteRange.IsAligned());
    for (ui32 i = 0; i < byteRange.AlignedBlockCount(); ++i) {
        auto target = buffer.GetBlock(i);
        for (auto& interval: bytes[i].Intervals) {
            memcpy(
                const_cast<char*>(target.Data()) + interval.OffsetInBlock,
                interval.Data.data(),
                interval.Data.size()
            );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

class TReadDataVisitor final
    : public IFreshBlockVisitor
    , public IMixedBlockVisitor
    , public IFreshBytesVisitor
{
private:
    const TString& LogTag;
    TTxIndexTablet::TReadData& Args;
    bool ApplyingByteLayer = false;

public:
    TReadDataVisitor(const TString& logTag, TTxIndexTablet::TReadData& args)
        : LogTag(logTag)
        , Args(args)
    {
        TABLET_VERIFY(Args.AlignedByteRange.IsAligned());
    }

    void Accept(const TBlock& block, TStringBuf blockData) override
    {
        TABLET_VERIFY(!ApplyingByteLayer);

        ui32 blockOffset = block.BlockIndex - Args.AlignedByteRange.FirstBlock();
        TABLET_VERIFY(blockOffset < Args.AlignedByteRange.BlockCount());

        auto& prev = Args.Blocks[blockOffset];
        if (Update(prev, block, {}, 0)) {
            Args.Buffer->SetBlock(blockOffset, blockData);
        }
    }

    void Accept(
        const TBlock& block,
        const TPartialBlobId& blobId,
        ui32 blobOffset) override
    {
        TABLET_VERIFY(!ApplyingByteLayer);
        TABLET_VERIFY(blobId);

        ui32 blockOffset = block.BlockIndex - Args.AlignedByteRange.FirstBlock();
        TABLET_VERIFY(blockOffset < Args.AlignedByteRange.BlockCount());

        auto& prev = Args.Blocks[blockOffset];
        if (Update(prev, block, blobId, blobOffset)) {
            Args.Buffer->ClearBlock(blockOffset);
        }
    }

    void Accept(const TBytes& bytes, TStringBuf data) override
    {
        ApplyingByteLayer = true;

        const auto firstBlockOffset =
            Args.AlignedByteRange.FirstBlock() * Args.AlignedByteRange.BlockSize;
        ui64 i = 0;

        while (i < bytes.Length) {
            auto offset = bytes.Offset + i;
            auto relOffset = offset - firstBlockOffset;
            auto blockIndex = relOffset / Args.AlignedByteRange.BlockSize;
            auto offsetInBlock = relOffset - blockIndex * Args.AlignedByteRange.BlockSize;
            // FreshBytes should be organized in such a way that newer commits
            // for the same bytes will be visited later than older commits, so
            // tracking individual byte commit ids is not needed
            auto& prev = Args.Blocks[blockIndex];
            auto next = Min<ui32>(
                bytes.Length,
                (blockIndex + 1) * Args.AlignedByteRange.BlockSize
            );
            if (prev.MinCommitId < bytes.MinCommitId) {
                Args.Bytes[blockIndex].Intervals.push_back({
                    IntegerCast<ui32>(offsetInBlock),
                    TString(data.Data() + i, next - i)
                });
            }

            i = next;
        }
    }

private:
    bool Update(
        TBlockDataRef& prev,
        const TBlock& block,
        const TPartialBlobId& blobId,
        ui32 blobOffset)
    {
        if (prev.MinCommitId < block.MinCommitId) {
            memcpy(&prev, &block, sizeof(TBlock));
            prev.BlobId = blobId;
            prev.BlobOffset = blobOffset;
            return true;
        }
        return false;
    }
};

////////////////////////////////////////////////////////////////////////////////

class TReadDataActor final
    : public TActorBootstrapped<TReadDataActor>
{
private:
    const ITraceSerializerPtr TraceSerializer;
    const TString LogTag;
    const TActorId Tablet;
    const TRequestInfoPtr RequestInfo;
    const ui64 CommitId;
    const TByteRange OriginByteRange;
    const TByteRange AlignedByteRange;
    const ui64 TotalSize;
    const TVector<TBlockDataRef> Blocks;
    TVector<TBlockBytes> Bytes;
    const IBlockBufferPtr Buffer;
    /*const*/ TSet<ui32> MixedBlocksRanges;

public:
    TReadDataActor(
        ITraceSerializerPtr traceSerializer,
        TString logTag,
        TActorId tablet,
        TRequestInfoPtr requestInfo,
        ui64 commitId,
        TByteRange originByteRange,
        TByteRange alignedByteRange,
        ui64 totalSize,
        TVector<TBlockDataRef> blocks,
        TVector<TBlockBytes> bytes,
        IBlockBufferPtr buffer,
        TSet<ui32> mixedBlocksRanges);

    void Bootstrap(const TActorContext& ctx);

private:
    STFUNC(StateWork);

    void ReadBlob(const TActorContext& ctx);
    void HandleReadBlobResponse(
        const TEvIndexTabletPrivate::TEvReadBlobResponse::TPtr& ev,
        const TActorContext& ctx);

    void HandlePoisonPill(
        const TEvents::TEvPoisonPill::TPtr& ev,
        const TActorContext& ctx);

    void ReplyAndDie(
        const TActorContext& ctx,
        const NProto::TError& error = {});
};

////////////////////////////////////////////////////////////////////////////////

TReadDataActor::TReadDataActor(
        ITraceSerializerPtr traceSerializer,
        TString logTag,
        TActorId tablet,
        TRequestInfoPtr requestInfo,
        ui64 commitId,
        TByteRange originByteRange,
        TByteRange alignedByteRange,
        ui64 totalSize,
        TVector<TBlockDataRef> blocks,
        TVector<TBlockBytes> bytes,
        IBlockBufferPtr buffer,
        TSet<ui32> mixedBlocksRanges)
    : TraceSerializer(std::move(traceSerializer))
    , LogTag(std::move(logTag))
    , Tablet(tablet)
    , RequestInfo(std::move(requestInfo))
    , CommitId(commitId)
    , OriginByteRange(originByteRange)
    , AlignedByteRange(alignedByteRange)
    , TotalSize(totalSize)
    , Blocks(std::move(blocks))
    , Bytes(std::move(bytes))
    , Buffer(std::move(buffer))
    , MixedBlocksRanges(std::move(mixedBlocksRanges))
{
    ActivityType = TFileStoreActivities::TABLET_WORKER;
    TABLET_VERIFY(AlignedByteRange.IsAligned());
}

void TReadDataActor::Bootstrap(const TActorContext& ctx)
{
    FILESTORE_TRACK(
        RequestReceived_TabletWorker,
        RequestInfo->CallContext,
        "ReadData");

    ReadBlob(ctx);
    Become(&TThis::StateWork);
}

void TReadDataActor::ReadBlob(const TActorContext& ctx)
{
    using TBlocksByBlob = THashMap<
        TPartialBlobId,
        TVector<TReadBlob::TBlock>,
        TPartialBlobIdHash
    >;

    TBlocksByBlob blocksByBlob;

    ui32 blockOffset = 0;
    for (const auto& block: Blocks) {
        ++blockOffset;

        if (!block.BlobId) {
            continue;
        }

        blocksByBlob[block.BlobId].emplace_back(block.BlobOffset, blockOffset - 1);
    }

    auto request = std::make_unique<TEvIndexTabletPrivate::TEvReadBlobRequest>(
        RequestInfo->CallContext);
    request->Buffer = Buffer;

    auto comparer = [] (const auto& l, const auto& r) {
        return l.BlobOffset < r.BlobOffset;
    };

    for (auto& [blobId, blocks]: blocksByBlob) {
        Sort(blocks, comparer);
        request->Blobs.emplace_back(blobId, std::move(blocks));
    }

    NCloud::Send(ctx, Tablet, std::move(request));
}

void TReadDataActor::HandleReadBlobResponse(
    const TEvIndexTabletPrivate::TEvReadBlobResponse::TPtr& ev,
    const TActorContext& ctx)
{
    const auto* msg = ev->Get();
    ApplyBytes(LogTag, AlignedByteRange, std::move(Bytes), *Buffer);
    ReplyAndDie(ctx, msg->GetError());
}

void TReadDataActor::HandlePoisonPill(
    const TEvents::TEvPoisonPill::TPtr& ev,
    const TActorContext& ctx)
{
    Y_UNUSED(ev);
    ReplyAndDie(ctx, MakeError(E_REJECTED, "request cancelled"));
}

void TReadDataActor::ReplyAndDie(
    const TActorContext& ctx,
    const NProto::TError& error)
{
    {
        // notify tablet
        auto response = std::make_unique<TEvIndexTabletPrivate::TEvReadDataCompleted>(error);
        response->CommitId = CommitId;
        response->MixedBlocksRanges = std::move(MixedBlocksRanges);

        NCloud::Send(ctx, Tablet, std::move(response));
    }

    FILESTORE_TRACK(
        ResponseSent_TabletWorker,
        RequestInfo->CallContext,
        "ReadData");

    if (RequestInfo->Sender != Tablet) {
        // reply to caller
        auto response = std::make_unique<TEvService::TEvReadDataResponse>(error);
        if (SUCCEEDED(error.GetCode())) {
            CopyFileData(
                LogTag,
                OriginByteRange,
                AlignedByteRange,
                TotalSize,
                Buffer->GetContentRef(),
                response->Record.MutableBuffer());
        }

        LOG_DEBUG(ctx, TFileStoreComponents::TABLET_WORKER,
            "%s ReadData: #%lu completed (%s)",
            LogTag.c_str(),
            RequestInfo->CallContext->RequestId,
            FormatError(response->Record.GetError()).c_str());

        BuildTraceInfo(
            TraceSerializer,
            RequestInfo->CallContext,
            response->Record);
        BuildThrottlerInfo(*RequestInfo->CallContext, response->Record);

        NCloud::Reply(ctx, *RequestInfo, std::move(response));
    }

    Die(ctx);
}

STFUNC(TReadDataActor::StateWork)
{
    switch (ev->GetTypeRewrite()) {
        HFunc(TEvents::TEvPoisonPill, HandlePoisonPill);

        HFunc(TEvIndexTabletPrivate::TEvReadBlobResponse, HandleReadBlobResponse);

        default:
            HandleUnexpectedEvent(ev, TFileStoreComponents::TABLET_WORKER);
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////

NProto::TError ValidateRequest(const NProto::TReadDataRequest& request, ui32 blockSize)
{
    const TByteRange range(
        request.GetOffset(),
        request.GetLength(),
        blockSize
    );

    if (auto error = ValidateRange(range); HasError(error)) {
        return error;
    }

    return {};
}

}   // namespace

////////////////////////////////////////////////////////////////////////////////

void TIndexTabletActor::HandleReadData(
    const TEvService::TEvReadDataRequest::TPtr& ev,
    const TActorContext& ctx)
{
    auto validator = [&] (const NProto::TReadDataRequest& request) {
        return ValidateRequest(request, GetBlockSize());
    };

    if (!AcceptRequest<TEvService::TReadDataMethod>(ev, ctx, validator)) {
        return;
    }

    // either rejected or put in the queue
    if (ThrottleIfNeeded<TEvService::TReadDataMethod>(ev, ctx)) {
        return;
    }

    auto* msg = ev->Get();
    const TByteRange byteRange(
        msg->Record.GetOffset(),
        msg->Record.GetLength(),
        GetBlockSize()
    );

    auto requestInfo = CreateRequestInfo(
        ev->Sender,
        ev->Cookie,
        msg->CallContext);

    TByteRange alignedByteRange = byteRange.AlignedSuperRange();
    auto blockBuffer = CreateBlockBuffer(alignedByteRange);

    ExecuteTx<TReadData>(
        ctx,
        std::move(requestInfo),
        msg->Record,
        byteRange,
        alignedByteRange,
        std::move(blockBuffer));
}

void TIndexTabletActor::HandleReadDataCompleted(
    const TEvIndexTabletPrivate::TEvReadDataCompleted::TPtr& ev,
    const TActorContext&)
{
    const auto* msg = ev->Get();

    ReleaseMixedBlocks(msg->MixedBlocksRanges);
    ReleaseCollectBarrier(msg->CommitId);
    WorkerActors.erase(ev->Sender);
}

////////////////////////////////////////////////////////////////////////////////

bool TIndexTabletActor::PrepareTx_ReadData(
    const TActorContext& ctx,
    TTransactionContext& tx,
    TTxIndexTablet::TReadData& args)
{
    auto* session = FindSession(
        args.ClientId,
        args.SessionId,
        args.SessionSeqNo);
    if (!session) {
        args.Error = ErrorInvalidSession(
            args.ClientId,
            args.SessionId,
            args.SessionSeqNo);
        return true;
    }

    auto* handle = FindHandle(args.Handle);
    if (!handle || handle->Session != session) {
        args.Error = ErrorInvalidHandle(args.Handle);
        return true;
    }

    if (!HasFlag(handle->GetFlags(), NProto::TCreateHandleRequest::E_READ)) {
        args.Error = ErrorInvalidHandle(args.Handle);
        return true;
    }

    args.NodeId = handle->GetNodeId();
    args.CommitId = handle->GetCommitId();

    LOG_DEBUG(ctx, TFileStoreComponents::TABLET,
        "%s[%s] ReadNodeData @%lu [%lu] %s",
        LogTag.c_str(),
        session->GetSessionId().c_str(),
        args.Handle,
        args.NodeId,
        args.AlignedByteRange.Describe().c_str());

    if (args.CommitId == InvalidCommitId) {
        args.CommitId = GetCurrentCommitId();
    }

    TIndexTabletDatabase db(tx.DB);

    bool ready = true;
    if (!ReadNode(db, args.NodeId, args.CommitId, args.Node)) {
        ready = false;
    } else {
        TABLET_VERIFY(args.Node);
        // TODO: access check
    }

    TSet<ui32> ranges;
    SplitRange(
        args.AlignedByteRange.FirstBlock(),
        args.AlignedByteRange.BlockCount(),
        BlockGroupSize,
        [&] (ui32 blockOffset, ui32 blocksCount) {
            ranges.insert(GetMixedRangeIndex(
                args.NodeId,
                IntegerCast<ui32>(args.AlignedByteRange.FirstBlock() + blockOffset),
                blocksCount));
        });

    for (ui32 rangeId: ranges) {
        if (!args.MixedBlocksRanges.count(rangeId)) {
            if (LoadMixedBlocks(db, rangeId)) {
                args.MixedBlocksRanges.insert(rangeId);
            } else {
                ready = false;
            }
        }
    }

    return ready;
}

void TIndexTabletActor::ExecuteTx_ReadData(
    const TActorContext& ctx,
    TTransactionContext& tx,
    TTxIndexTablet::TReadData& args)
{
    Y_UNUSED(ctx);
    Y_UNUSED(tx);

    FILESTORE_VALIDATE_TX_ERROR(ReadData, args);

    TReadDataVisitor visitor(LogTag, args);

    FindFreshBlocks(
        visitor,
        args.NodeId,
        args.CommitId,
        args.AlignedByteRange.FirstBlock(),
        args.AlignedByteRange.BlockCount());

    SplitRange(
        args.AlignedByteRange.FirstBlock(),
        args.AlignedByteRange.BlockCount(),
        BlockGroupSize,
        [&] (ui32 blockOffset, ui32 blocksCount) {
            FindMixedBlocks(
                visitor,
                args.NodeId,
                args.CommitId,
                IntegerCast<ui32>(args.AlignedByteRange.FirstBlock() + blockOffset),
                blocksCount);
        });

    FindFreshBytes(
        visitor,
        args.NodeId,
        args.CommitId,
        args.AlignedByteRange);
}

void TIndexTabletActor::CompleteTx_ReadData(
    const TActorContext& ctx,
    TTxIndexTablet::TReadData& args)
{
    if (FAILED(args.Error.GetCode())) {
        auto response = std::make_unique<TEvService::TEvReadDataResponse>(args.Error);
        CompleteResponse<TEvService::TReadDataMethod>(
            response->Record,
            args.RequestInfo->CallContext,
            ctx);

        NCloud::Reply(ctx, *args.RequestInfo, std::move(response));
        return;
    }

    if (!ShouldReadBlobs(args.Blocks)) {
        ApplyBytes(LogTag, args.AlignedByteRange, std::move(args.Bytes), *args.Buffer);

        auto response = std::make_unique<TEvService::TEvReadDataResponse>();
        CopyFileData(
            LogTag,
            args.OriginByteRange,
            args.AlignedByteRange,
            args.Node->Attrs.GetSize(),
            args.Buffer->GetContentRef(),
            response->Record.MutableBuffer());

        CompleteResponse<TEvService::TReadDataMethod>(
            response->Record,
            args.RequestInfo->CallContext,
            ctx);

        ReleaseMixedBlocks(args.MixedBlocksRanges);
        NCloud::Reply(ctx, *args.RequestInfo, std::move(response));
        return;
    }

    AcquireCollectBarrier(args.CommitId);

    auto actor = std::make_unique<TReadDataActor>(
        TraceSerializer,
        LogTag,
        ctx.SelfID,
        args.RequestInfo,
        args.CommitId,
        args.OriginByteRange,
        args.AlignedByteRange,
        args.Node->Attrs.GetSize(),
        std::move(args.Blocks),
        std::move(args.Bytes),
        std::move(args.Buffer),
        std::move(args.MixedBlocksRanges));

    auto actorId = NCloud::Register(ctx, std::move(actor));
    WorkerActors.insert(actorId);
}

}   // namespace NCloud::NFileStore::NStorage
