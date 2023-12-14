#include "backend_null.h"

#include "backend.h"

#include <cloud/contrib/vhost/include/vhost/server.h>
#include <cloud/storage/core/libs/diagnostics/logging.h>

namespace NCloud::NBlockStore::NVHostServer {

namespace {

////////////////////////////////////////////////////////////////////////////////

class TNullBackend final: public IBackend
{
private:
    const ILoggingServicePtr Logging;
    TLog Log;

public:
    explicit TNullBackend(ILoggingServicePtr logging);

    void Init(const TOptions& options, vhd_bdev_info& devInfo) override;
    void Start() override;
    void Stop() override;
    void ProcessQueue(vhd_request_queue* queue, TSimpleStats& queueStats) override;
    std::optional<TSimpleStats> GetCompletionStats(TDuration timeout) override;
};

////////////////////////////////////////////////////////////////////////////////

TNullBackend::TNullBackend(ILoggingServicePtr logging)
    : Logging{std::move(logging)}
{
    Log = Logging->CreateLog("NULL");
}

void TNullBackend::Init(const TOptions& options, vhd_bdev_info& devInfo)
{
    Y_UNUSED(options);
    Y_UNUSED(devInfo);

    i64 totalBytes = 0;

    for (auto& chunk: options.Layout) {
        totalBytes += chunk.ByteCount;
    }

    devInfo = {
        .serial = options.Serial.c_str(),
        .socket_path = options.SocketPath.c_str(),
        .block_size = VHD_SECTOR_SIZE,
        .num_queues = options.QueueCount,   // Max count of virtio queues
        .total_blocks = totalBytes / VHD_SECTOR_SIZE,
        .features = options.ReadOnly ? VHD_BDEV_F_READONLY : 0};
}

void TNullBackend::Start()
{}

void TNullBackend::Stop()
{}

void TNullBackend::ProcessQueue(vhd_request_queue* queue, TSimpleStats& queueStats)
{
    Y_UNUSED(queueStats);

    vhd_request req;
    if (!vhd_dequeue_request(queue, &req)) {
        return;
    }

    struct vhd_bdev_io* bio = vhd_get_bdev_io(req.io);
    STORAGE_DEBUG(
        "%s Index=%lu, BlocksCount=%lu, BlockSize=%llu",
        bio->type == VHD_BDEV_READ ? "READ" : "WRITE",
        bio->first_sector,
        bio->total_sectors,
        VHD_SECTOR_SIZE);

    vhd_complete_bio(req.io, VHD_BDEV_SUCCESS);
}

std::optional<TSimpleStats> TNullBackend::GetCompletionStats(TDuration timeout)
{
    Y_UNUSED(timeout);
    return {};
}

}   // namespace

////////////////////////////////////////////////////////////////////////////////

IBackendPtr CreateNullBackend(ILoggingServicePtr logging)
{
    return std::make_shared<TNullBackend>(std::move(logging));
}

}   // namespace NCloud::NBlockStore::NVHostServer
