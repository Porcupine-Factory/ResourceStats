#pragma once
#include <ResourceStats/ResourceStatsComponentBus.h>
#include <ResourceStats/NetworkResourceStatsControllerBus.h>

#include <AzCore/Component/Component.h>
#include <AzCore/Component/EntityBus.h>
#include <AzCore/Component/TickBus.h>

namespace ResourceStats
{
    class NetworkResourceStats;
    class NetworkResourceStatsController;

    class ResourceStatsComponent
        : public AZ::Component
        , public AZ::TickBus::Handler
        , public NetworkResourceStatsControllerNotificationBus::Handler
        , public ResourceStatsComponentRequestBus::Handler
        , public AZ::EntityBus::Handler
    {
        friend class NetworkResourceStatsController;

    public:
        AZ_COMPONENT(ResourceStatsComponent, "{74b0eff5-72c9-4053-992b-2a4d56ea1d65}");

        static void Reflect(AZ::ReflectContext* rc);

        // AZ::Component interface implementation
        void Activate() override;
        void Deactivate() override;

        // AZ::EntityBus interface
        void OnEntityActivated(const AZ::EntityId& entityId) override;

        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);

        // ResourceStatsComponentRequestBus
        AZ::u32 GetHealth() const override;
        void SetHealth(const AZ::u32 new_health) override;
        AZ::u32 GetMana() const override;
        void SetMana(const AZ::u32 new_mana) override;
        AZ::u32 GetMaxHealth() const override;
        void SetMaxHealth(const AZ::u32 new_maxHealth) override;
        AZ::u32 GetMaxMana() const override;
        void SetMaxMana(const AZ::u32 new_maxMana) override;
        void SetHealthDelta(const AZ::s64& new_healthDelta) override;
        void SetManaDelta(const AZ::s64& new_manaDelta) override;
        bool GetIsAutonomousClient() const override;
        bool GetIsServer() const override;
        bool GetIsHost() const override;
        bool GetLocallyEnableNetworkResourceStats() const override;
        void SetLocallyEnableNetworkResourceStats(const bool& new_networkResourceStatsEnabled) override;
        void IsAutonomousSoConnect() override;
        void NotAutonomousSoDisconnect() override;

        // ResourceStatsComponentNotificationBus
        void OnNetworkResourceStatsTickStart(const float& deltaTime);
        void OnNetworkResourceStatsTickFinish(const float& deltaTime);

    private:
        // TickBus interface
        void OnTick(float deltaTime, AZ::ScriptTimePoint) override;

        // Called on each tick
        void ProcessInput(const float& deltaTime, const AZ::u8& tickTimestepNetwork);

        // NetworkResourceStatsControllerNotificationBus
        void OnNetworkTickStart(const float& deltaTime, const bool& server, const AZ::EntityId& entity);
        void OnNetworkTickFinish(const float& deltaTime, const bool& server, const AZ::EntityId& entity);

        // Various methods used to implement the Resource Stats functionality
        void SetNetworkResourceStatsProperties() const;
        void GetNetworkResourceStatsProperties();

        // NetworkResourceStats object
        NetworkResourceStats* m_networkResourceStatsObject = nullptr;
        NetworkResourceStatsController* m_networkResourceStatsControllerObject = nullptr;

        // Stores the previous NetworkResourceStats tick deltaTime
        float m_prevDeltaTime = 1.f / 60.f;
        float m_prevNetworkResourceStatsDeltaTime = 0.033f;

        // Various resources / stats
        AZ::u32 m_health = 100;
        AZ::u32 m_mana = 100;
        AZ::u32 m_maxHealth = 100;
        AZ::u32 m_maxMana = 100;

        // Networking related variables
        bool m_networkResourceStatsEnabled = false;
        bool m_isServer = false;
        bool m_isHost = false;
        bool m_isAutonomousClient = false;
    };
} // namespace ResourceStats
