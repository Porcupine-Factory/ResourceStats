#pragma once

#include <AzCore/Component/ComponentBus.h>
#include <AzCore/RTTI/BehaviorContext.h>

#if __has_include(<FirstPersonController/FirstPersonControllerComponentBus.h>)
#include <FirstPersonController/FirstPersonControllerComponentBus.h>
#endif

namespace ResourceStats
{
    class ResourceStatsComponentRequests : public AZ::ComponentBus
    {
    public:
        ~ResourceStatsComponentRequests() override = default;

        virtual AZ::u32 GetHealth() const = 0;
        virtual void SetHealth(const AZ::u32) = 0;
        virtual AZ::u32 GetMaxHealth() const = 0;
        virtual void SetMaxHealth(const AZ::u32) = 0;
        virtual void SetHealthDelta(const AZ::s64) = 0;
        virtual AZ::u32 GetMana() const = 0;
        virtual void SetMana(const AZ::u32) = 0;
        virtual AZ::u32 GetMaxMana() const = 0;
        virtual void SetMaxMana(const AZ::u32) = 0;
        virtual void SetManaDelta(const AZ::s64) = 0;
        virtual AZ::u32 GetStrength() const = 0;
        virtual void SetStrength(const AZ::u32) = 0;
        virtual AZ::u32 GetMaxStrength() const = 0;
        virtual void SetMaxStrength(const AZ::u32) = 0;
        virtual void SetStrengthDelta(const AZ::s64) = 0;
        virtual AZ::u32 GetAttack() const = 0;
        virtual void SetAttack(const AZ::u32) = 0;
        virtual AZ::u32 GetMaxAttack() const = 0;
        virtual void SetMaxAttack(const AZ::u32) = 0;
        virtual void SetAttackDelta(const AZ::s64) = 0;
        virtual AZ::u32 GetArmor() const = 0;
        virtual void SetArmor(const AZ::u32) = 0;
        virtual AZ::u32 GetMaxArmor() const = 0;
        virtual void SetMaxArmor(const AZ::u32) = 0;
        virtual void SetArmorDelta(const AZ::s64) = 0;
#ifdef FIRST_PERSON_CONTROLLER
        virtual AZ::u32 GetStamina() const = 0;
        virtual void SetStamina(const AZ::u32) = 0;
        virtual AZ::u32 GetMaxStamina() const = 0;
        virtual void SetMaxStamina(const AZ::u32) = 0;
        virtual void SetStaminaDelta(const AZ::s64) = 0;
        virtual AZ::u32 GetStaminaCooldownTime() const = 0;
        virtual void SetStaminaCooldownTime(const AZ::u32 staminaCooldownTime) = 0;
        virtual float GetStaminaPercentage() const = 0;
#endif
        virtual bool GetIsAutonomousClient() const = 0;
        virtual bool GetIsServer() const = 0;
        virtual bool GetIsHost() const = 0;
        virtual bool GetLocallyEnableNetworkResourceStats() const = 0;
        virtual void SetLocallyEnableNetworkResourceStats(const bool networkResourceStatsEnabled) = 0;
        virtual void IsAutonomousSoConnect() = 0;
        virtual void NotAutonomousSoDisconnect() = 0;
    };

    using ResourceStatsComponentRequestBus = AZ::EBus<ResourceStatsComponentRequests>;

    class ResourceStatsComponentNotifications : public AZ::ComponentBus
    {
    public:
        virtual void OnNetworkResourceStatsTickStart(const float) = 0;
        virtual void OnNetworkResourceStatsTickFinish(const float) = 0;
    };

    using ResourceStatsComponentNotificationBus = AZ::EBus<ResourceStatsComponentNotifications>;

    class ResourceStatsComponentNotificationHandler
        : public ResourceStatsComponentNotificationBus::Handler
        , public AZ::BehaviorEBusHandler
    {
    public:
        AZ_EBUS_BEHAVIOR_BINDER(
            ResourceStatsComponentNotificationHandler,
            "{ac4001c0-d9a3-4df4-b393-60e190f5d0d8}",
            AZ::SystemAllocator,
            OnNetworkResourceStatsTickStart,
            OnNetworkResourceStatsTickFinish);

        void OnNetworkResourceStatsTickStart(const float deltaTime) override
        {
            Call(FN_OnNetworkResourceStatsTickStart, deltaTime);
        }
        void OnNetworkResourceStatsTickFinish(const float deltaTime) override
        {
            Call(FN_OnNetworkResourceStatsTickFinish, deltaTime);
        }
    };
} // namespace ResourceStats
