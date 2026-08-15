#pragma once

#include <AzCore/Component/ComponentBus.h>
#include <AzCore/RTTI/BehaviorContext.h>

namespace ResourceStats
{
    class NetworkResourceStatsControllerRequests : public AZ::ComponentBus
    {
    public:
        ~NetworkResourceStatsControllerRequests() override = default;

        virtual bool GetEnabled() const = 0;
        virtual void SetEnabled(const bool) = 0;
    };

    using NetworkResourceStatsControllerRequestBus = AZ::EBus<NetworkResourceStatsControllerRequests>;

    class NetworkResourceStatsControllerNotifications : public AZ::ComponentBus
    {
    public:
        virtual void OnNetworkTickStart(const float, const bool, const AZ::EntityId&) = 0;
        virtual void OnNetworkTickFinish(const float, const bool, const AZ::EntityId&) = 0;
    };

    using NetworkResourceStatsControllerNotificationBus = AZ::EBus<NetworkResourceStatsControllerNotifications>;

    class NetworkResourceStatsControllerNotificationHandler
        : public NetworkResourceStatsControllerNotificationBus::Handler
        , public AZ::BehaviorEBusHandler
    {
    public:
        AZ_EBUS_BEHAVIOR_BINDER(
            NetworkResourceStatsControllerNotificationHandler,
            "{9baf0c0c-c98d-4164-9bcb-5038f1e9cc1d}",
            AZ::SystemAllocator,
            OnNetworkTickStart,
            OnNetworkTickFinish);

        void OnNetworkTickStart(const float deltaTime, const bool server, const AZ::EntityId& entityId) override
        {
            Call(FN_OnNetworkTickStart, deltaTime, server, entityId);
        }
        void OnNetworkTickFinish(const float deltaTime, const bool server, const AZ::EntityId& entityId) override
        {
            Call(FN_OnNetworkTickFinish, deltaTime, server, entityId);
        }
    };
} // namespace ResourceStats
