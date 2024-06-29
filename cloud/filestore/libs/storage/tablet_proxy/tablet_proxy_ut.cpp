#include "tablet_proxy.h"

#include <cloud/filestore/libs/storage/testlib/ss_proxy_client.h>
#include <cloud/filestore/libs/storage/testlib/tablet_proxy_client.h>
#include <cloud/filestore/libs/storage/testlib/test_env.h>

#include <library/cpp/testing/unittest/registar.h>

#include <unordered_set>

namespace NCloud::NFileStore::NStorage {

using namespace NActors;

using namespace NKikimr;

////////////////////////////////////////////////////////////////////////////////

Y_UNIT_TEST_SUITE(TIndexTabletProxyTest)
{
    Y_UNIT_TEST(ShouldHandleRequests)
    {
        TTestEnv env;
        env.CreateSubDomain("nfs");

        ui32 nodeIdx = env.CreateNode("nfs");

        TSSProxyClient ssProxy(env.GetStorageConfig(), env.GetRuntime(), nodeIdx);
        ssProxy.CreateFileStore("test", 1000);

        TIndexTabletProxyClient tabletProxy(env.GetRuntime(), nodeIdx);
        tabletProxy.WaitReady("test");
    }

    Y_UNIT_TEST(ShouldDetectRemoteTabletDeath)
    {
        TTestEnvConfig cfg {.StaticNodes = 1, .DynamicNodes = 2};
        TTestEnv env(cfg);
        env.CreateSubDomain("nfs");

        auto& runtime = env.GetRuntime();
        ui32 nodeIdx1 = env.CreateNode("nfs");

        TSSProxyClient ssProxy(env.GetStorageConfig(), runtime, nodeIdx1);
        ssProxy.CreateFileStore("test", 1000);

        auto response = ssProxy.DescribeFileStore("test");
        auto fsTabletId = response->
            PathDescription.
            GetFileStoreDescription().
            GetIndexTabletId();

        std::unordered_set<TActorId> connections;
        runtime.SetEventFilter([&] (auto& runtime, auto& event) {
            Y_UNUSED(runtime);
            switch (event->GetTypeRewrite()) {
                case TEvTabletPipe::EvClientConnected: {
                    auto* msg =
                        event->template Get<TEvTabletPipe::TEvClientConnected>();
                    if (msg->TabletId == fsTabletId) {
                        connections.emplace(event->Recipient);
                    }
                    break;
                }
                case TEvTabletPipe::EvClientDestroyed: {
                    auto* msg =
                        event->template Get<TEvTabletPipe::TEvClientDestroyed>();
                    if (msg->TabletId == fsTabletId) {
                        connections.erase(event->Recipient);
                    }
                    break;
                }
            }
            return false;
        });

        TIndexTabletProxyClient tabletProxy1(env.GetRuntime(), nodeIdx1);
        tabletProxy1.WaitReady("test");

        ui32 nodeIdx2 = env.CreateNode("nfs");

        TIndexTabletProxyClient tabletProxy2(env.GetRuntime(), nodeIdx2);
        tabletProxy2.WaitReady("test");

        UNIT_ASSERT_LE(2, connections.size());

        RebootTablet(runtime, fsTabletId, tabletProxy1.GetSender(), nodeIdx1);
        runtime.DispatchEvents(TDispatchOptions{
            .CustomFinalCondition = [&]()
            {
                return connections.empty();
            }});
    }

    Y_UNIT_TEST(ShouldNotDie)
    {
        TTestEnv env;
        env.CreateSubDomain("nfs");

        ui32 nodeIdx = env.CreateNode("nfs");

        TSSProxyClient ssProxy(
            env.GetStorageConfig(),
            env.GetRuntime(),
            nodeIdx);
        ssProxy.CreateFileStore("test", 1000);

        TIndexTabletProxyClient tabletProxy(env.GetRuntime(), nodeIdx);
        tabletProxy.WaitReady("test");
        tabletProxy.SendRequest(
            MakeIndexTabletProxyServiceId(),
            std::make_unique<TEvents::TEvPoisonPill>());
        env.GetRuntime().DispatchEvents({}, TDuration::MilliSeconds(100));
        tabletProxy.WaitReady("test");
    }
}

}   // namespace NCloud::NFileStore::NStorage
