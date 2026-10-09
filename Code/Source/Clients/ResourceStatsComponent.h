#pragma once
#ifdef NETWORKRESOURCESTATS
#include <ResourceStats/NetworkResourceStatsControllerBus.h>
#endif
#include <ResourceStats/ResourceStatsComponentBus.h>

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
#ifdef NETWORKRESOURCESTATS
        , public NetworkResourceStatsControllerNotificationBus::Handler
#endif
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
        void SetHealth(const AZ::u32 health) override;
        AZ::u32 GetMaxHealth() const override;
        void SetMaxHealth(const AZ::u32 maxHealth) override;
        void SetHealthDelta(const AZ::s64 healthDelta) override;
        AZ::u32 GetMana() const override;
        void SetMana(const AZ::u32 mana) override;
        AZ::u32 GetMaxMana() const override;
        void SetMaxMana(const AZ::u32 maxMana) override;
        void SetManaDelta(const AZ::s64 manaDelta) override;
        AZ::u32 GetStrength() const override;
        void SetStrength(const AZ::u32 strength) override;
        AZ::u32 GetMaxStrength() const override;
        void SetMaxStrength(const AZ::u32 maxStrength) override;
        void SetStrengthDelta(const AZ::s64 strengthDelta) override;
        AZ::u32 GetAttack() const override;
        void SetAttack(const AZ::u32 attack) override;
        AZ::u32 GetMaxAttack() const override;
        void SetMaxAttack(const AZ::u32 maxAttack) override;
        void SetAttackDelta(const AZ::s64 attackDelta) override;
        AZ::u32 GetArmor() const override;
        void SetArmor(const AZ::u32 armor) override;
        AZ::u32 GetMaxArmor() const override;
        void SetMaxArmor(const AZ::u32 maxArmor) override;
        void SetArmorDelta(const AZ::s64 armorDelta) override;
#ifdef FIRST_PERSON_CONTROLLER
        AZ::u32 GetStamina() const override;
        void SetStamina(const AZ::u32 stamina) override;
        AZ::u32 GetMaxStamina() const override;
        void SetMaxStamina(const AZ::u32 maxStamina) override;
        void SetStaminaDelta(const AZ::s64 staminaDelta) override;
        AZ::u32 GetStaminaCooldownTime() const override;
        void SetStaminaCooldownTime(const AZ::u32 staminaCooldownTime) override;
        float GetStaminaPercentage() const override;
#endif
        bool GetIsAutonomousClient() const override;
        bool GetIsServer() const override;
        bool GetIsHost() const override;
        bool GetLocallyEnableNetworkResourceStats() const override;
        void SetLocallyEnableNetworkResourceStats(const bool networkResourceStatsEnabled) override;
        void IsAutonomousSoConnect() override;
        void NotAutonomousSoDisconnect() override;

    private:
        // TickBus interface
        void OnTick(float deltaTime, AZ::ScriptTimePoint) override;

        // Called on each tick
        void ProcessInput(const float deltaTime, const AZ::u8 tickTimestepNetwork);

        // NetworkResourceStatsControllerNotificationBus
        void OnNetworkTickStart(const float deltaTime, const bool server, const AZ::EntityId& entityId);
        void OnNetworkTickFinish(const float deltaTime, const bool server, const AZ::EntityId& entityId);

        // Various methods used to implement the Resource Stats functionality
        void GetNetworkResourceStatsProperties();
        void SetNetworkResourceStatsProperties() const;

#ifdef FIRST_PERSON_CONTROLLER
        void InitStamina();
        void UpdateStamina();
#endif

        // NetworkResourceStats object
#ifdef NETWORKRESOURCESTATS
        NetworkResourceStats* m_networkResourceStatsObject = nullptr;
        NetworkResourceStatsController* m_networkResourceStatsControllerObject = nullptr;
#else
        bool* m_networkResourceStatsObject = nullptr;
        bool* m_networkResourceStatsControllerObject = nullptr;
#endif

        // Various resources / stats
        AZ::u32 m_health = 100;
        AZ::u32 m_maxHealth = 100;
        AZ::u32 m_mana = 100;
        AZ::u32 m_maxMana = 100;
        AZ::u32 m_strength = 100;
        AZ::u32 m_maxStrength = 100;
        AZ::u32 m_attack = 100;
        AZ::u32 m_maxAttack = 100;
        AZ::u32 m_armor = 100;
        AZ::u32 m_maxArmor = 100;
#ifdef FIRST_PERSON_CONTROLLER
        // Stamina units are in milliseconds of sprinting time
        AZ::u32 m_stamina = 20000;
        AZ::u32 m_maxStamina = 20000;
        AZ::u32 m_staminaCooldownTime = 10000;
        bool m_staminaInitialized = false;
#endif

        // Previous values tracked for notification events
        AZ::u32 m_prevHealth = 100;

        // Networking related variables
        bool m_networkResourceStatsEnabled = false;
        bool m_isServer = false;
        bool m_isHost = false;
        bool m_isAutonomousClient = false;
    };
} // namespace ResourceStats
