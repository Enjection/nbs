#include "datashard_impl.h"
#include "datashard_pipeline.h"

#include "contrib/ydb/core/tx/datashard/datashard_write_operation.h"
#include <contrib/ydb/core/tablet/tablet_exception.h>

namespace NKikimr {
namespace NDataShard {

class TCheckWriteUnit: public TExecutionUnit {
public:
    TCheckWriteUnit(TDataShard &dataShard, TPipeline &pipeline);
    ~TCheckWriteUnit() override;

    bool IsReadyToExecute(TOperation::TPtr op) const override;
    EExecutionStatus Execute(TOperation::TPtr op,
                             TTransactionContext &txc,
                             const TActorContext &ctx) override;
    void Complete(TOperation::TPtr op,
                  const TActorContext &ctx) override;

private:
};

TCheckWriteUnit::TCheckWriteUnit(TDataShard &dataShard,
                                   TPipeline &pipeline)
    : TExecutionUnit(EExecutionUnitKind::CheckWrite, false, dataShard, pipeline)
{
}

TCheckWriteUnit::~TCheckWriteUnit()
{
}

bool TCheckWriteUnit::IsReadyToExecute(TOperation::TPtr) const
{
    return true;
}

EExecutionStatus TCheckWriteUnit::Execute(TOperation::TPtr op,
                                           TTransactionContext &,
                                           const TActorContext &ctx)
{
    Y_ABORT_UNLESS(op->IsDataTx() || op->IsReadTable());
    Y_ABORT_UNLESS(!op->IsAborted());

    if (CheckRejectDataTx(op, ctx)) {
        op->Abort(EExecutionUnitKind::FinishProposeWrite);

        return EExecutionStatus::Executed;
    }

    TWriteOperation* writeOp = dynamic_cast<TWriteOperation*>(op.Get());
    Y_VERIFY_S(writeOp, "cannot cast operation of kind " << op->GetKind());
    auto writeTx = writeOp->WriteTx();
    Y_ABORT_UNLESS(writeTx);
    Y_ABORT_UNLESS(writeTx->Ready() || writeTx->RequirePrepare());

    // Check if we are out of space and tx wants to update user
    // or system table.
    if (DataShard.IsAnyChannelYellowStop()
        && (writeTx->HasWrites() || !op->IsImmediate())) {
        TString err = TStringBuilder()
            << "Cannot perform transaction: out of disk space at tablet "
            << DataShard.TabletID() << " txId " << op->GetTxId();

        DataShard.IncCounter(COUNTER_PREPARE_OUT_OF_SPACE);

        writeOp->SetError(NKikimrDataEvents::TEvWriteResult::STATUS_INTERNAL_ERROR, err);
        op->Abort(EExecutionUnitKind::FinishProposeWrite);

        LOG_LOG_S_THROTTLE(DataShard.GetLogThrottler(TDataShard::ELogThrottlerType::CheckWriteUnit_Execute), ctx, NActors::NLog::PRI_ERROR, NKikimrServices::TX_DATASHARD, err);

        return EExecutionStatus::Executed;
    }

    {
        for (const auto& key : writeTx->TxInfo().Keys) {
            if (key.IsWrite && DataShard.IsUserTable(key.Key->TableId)) {
                ui64 keySize = 0;
                for (const auto& cell : key.Key->Range.From) {
                    keySize += cell.Size();
                }
                if (keySize > NLimits::MaxWriteKeySize) {
                    TString err = TStringBuilder()
                        << "Operation " << *op << " writes key of " << keySize
                        << " bytes which exceeds limit " << NLimits::MaxWriteKeySize
                        << " bytes at " << DataShard.TabletID();

                    writeOp->SetError(NKikimrDataEvents::TEvWriteResult::STATUS_BAD_REQUEST, err);
                    op->Abort(EExecutionUnitKind::FinishProposeWrite);

                    LOG_ERROR_S(ctx, NKikimrServices::TX_DATASHARD, err);

                    return EExecutionStatus::Executed;
                }
                for (const auto& col : key.Key->Columns) {
                    if (col.Operation == TKeyDesc::EColumnOperation::Set ||
                        col.Operation == TKeyDesc::EColumnOperation::InplaceUpdate)
                    {
                        if (col.ImmediateUpdateSize > NLimits::MaxWriteValueSize) {
                            TString err = TStringBuilder()
                                << "Transaction write column value of " << col.ImmediateUpdateSize
                                << " bytes is larger than the allowed threshold";

                            writeOp->SetError(NKikimrDataEvents::TEvWriteResult::STATUS_BAD_REQUEST, err);
                            op->Abort(EExecutionUnitKind::FinishProposeWrite);

                            LOG_ERROR_S(ctx, NKikimrServices::TX_DATASHARD, err);

                            return EExecutionStatus::Executed;
                        }
                    }
                }

                if (DataShard.IsSubDomainOutOfSpace()) {
                    switch (key.Key->RowOperation) {
                        case TKeyDesc::ERowOperation::Read:
                        case TKeyDesc::ERowOperation::Erase:
                            // Read and erase are allowed even when we're out of disk space
                            break;

                        default: {
                            // Updates are not allowed when database is out of space
                            TString err = "Cannot perform writes: database is out of disk space";

                            DataShard.IncCounter(COUNTER_PREPARE_OUT_OF_SPACE);

                            writeOp->SetError(NKikimrDataEvents::TEvWriteResult::STATUS_INTERNAL_ERROR, err);
                            op->Abort(EExecutionUnitKind::FinishProposeWrite);

                            LOG_LOG_S_THROTTLE(DataShard.GetLogThrottler(TDataShard::ELogThrottlerType::CheckWriteUnit_Execute), ctx, NActors::NLog::PRI_ERROR, NKikimrServices::TX_DATASHARD, err);

                            return EExecutionStatus::Executed;
                        }
                    }
                }
            }
        }
    }

    if (!op->IsImmediate()) {
        if (!Pipeline.AssignPlanInterval(op)) {
            TString err = TStringBuilder() << "Can't propose tx " << op->GetTxId() << " at blocked shard " << DataShard.TabletID();

            writeOp->SetError(NKikimrDataEvents::TEvWriteResult::STATUS_INTERNAL_ERROR, err);
            op->Abort(EExecutionUnitKind::FinishProposeWrite);

            LOG_NOTICE_S(ctx, NKikimrServices::TX_DATASHARD, err);

            return EExecutionStatus::Executed;
        }

        writeOp->SetWriteResult(NEvents::TDataEvents::TEvWriteResult::BuildPrepared(writeOp->WriteTx()->TabletId(), op->GetTxId(), {op->GetMinStep(), op->GetMaxStep(), {}}));
        LOG_DEBUG_S(ctx, NKikimrServices::TX_DATASHARD, "Prepared " << *op << " at " << DataShard.TabletID());
    }

    return EExecutionStatus::Executed;
}

void TCheckWriteUnit::Complete(TOperation::TPtr, const TActorContext &)
{
}

THolder<TExecutionUnit> CreateCheckWriteUnit(TDataShard &dataShard, TPipeline &pipeline)
{
    return THolder(new TCheckWriteUnit(dataShard, pipeline));
}

} // namespace NDataShard
} // namespace NKikimr
