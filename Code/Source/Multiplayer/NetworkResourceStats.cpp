#include <Multiplayer/NetworkResourceStats.h>

namespace ResourceStats
{
    NetworkResourceStatsController::NetworkResourceStatsController(NetworkResourceStats& parent)
        : NetworkResourceStatsControllerBase(parent)
        , m_enableNetworkResourceStatsChangedEvent(
              [this](bool enable)
              {
                  OnEnableNetworkResourceStatsChanged(enable);
              })
    {
    }

    void NetworkResourceStatsController::OnActivate([[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
        NetworkResourceStatsControllerRequestBus::Handler::BusConnect(GetEntityId());

        // Subscribe to EnableNetworkResourceStats change events
        EnableNetworkResourceStatsAddEvent(m_enableNetworkResourceStatsChangedEvent);

        // Get access to the ResourceStatsComponent object and its members
        const AZ::Entity* entity = GetParent().GetEntity();
        m_resourceStatsObject = entity->FindComponent<ResourceStatsComponent>();
        m_resourceStatsObject->m_networkResourceStatsEnabled = GetEnableNetworkResourceStats();

        if (IsNetEntityRoleAutonomous())
        {
            m_autonomousNotDetermined = false;
            m_resourceStatsObject->IsAutonomousSoConnect();
            if (IsNetEntityRoleAuthority())
                m_resourceStatsObject->m_isHost = true;
            else
                m_resourceStatsObject->m_isAutonomousClient = true;
        }
    }

    void NetworkResourceStatsController::OnDeactivate([[maybe_unused]] Multiplayer::EntityIsMigrating entityIsMigrating)
    {
    }

    void NetworkResourceStatsController::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        required.push_back(AZ_CRC_CE("ResourceStatsService"));
    }

    void NetworkResourceStatsController::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    void NetworkResourceStatsController::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("NetworkResourceStatsService"));
    }

    void NetworkResourceStatsController::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("NetworkResourceStatsService"));
    }

    void NetworkResourceStatsController::CreateInput([[maybe_unused]] Multiplayer::NetworkInput& input, [[maybe_unused]] float deltaTime)
    {
    }

    void NetworkResourceStatsController::ProcessInput([[maybe_unused]] Multiplayer::NetworkInput& input, float deltaTime)
    {
        if (m_disabled)
            return;

        if (m_init)
        {
            m_init = false;
            SetHealth(m_resourceStatsObject->m_health);
            SetMaxHealth(m_resourceStatsObject->m_maxHealth);
            SetMana(m_resourceStatsObject->m_mana);
            SetMaxMana(m_resourceStatsObject->m_maxMana);
            SetStrength(m_resourceStatsObject->m_strength);
            SetMaxStrength(m_resourceStatsObject->m_maxStrength);
            SetStamina(m_resourceStatsObject->m_stamina);
            SetMaxStamina(m_resourceStatsObject->m_maxStamina);
            SetStaminaCooldownTime(m_resourceStatsObject->m_staminaCooldownTime);
        }

        // Disconnect from various buses when the NetworkResourceStatsController is not autonomous, and only do this once
        if (m_autonomousNotDetermined)
        {
            m_resourceStatsObject->NotAutonomousSoDisconnect();
            if (IsNetEntityRoleAuthority())
                m_resourceStatsObject->m_isServer = true;
            m_autonomousNotDetermined = false;
        }

        NetworkResourceStatsControllerNotificationBus::Broadcast(
            &NetworkResourceStatsControllerNotificationBus::Events::OnNetworkTickStart,
            deltaTime,
            m_resourceStatsObject->m_isServer,
            GetEntityId());

        // Do work every network tick here

        NetworkResourceStatsControllerNotificationBus::Broadcast(
            &NetworkResourceStatsControllerNotificationBus::Events::OnNetworkTickFinish,
            deltaTime,
            m_resourceStatsObject->m_isServer,
            GetEntityId());
    }

#if AZ_TRAIT_SERVER
    void NetworkResourceStatsController::HandleSendHealthDelta(
        [[maybe_unused]] AzNetworking::IConnection* invokingConnection, const AZ::s64& healthDelta)
    {
        m_resourceStatsObject->SetHealthDelta(healthDelta);
    }
    void NetworkResourceStatsController::HandleSendManaDelta(
        [[maybe_unused]] AzNetworking::IConnection* invokingConnection, const AZ::s64& manaDelta)
    {
        m_resourceStatsObject->SetManaDelta(manaDelta);
    }
    void NetworkResourceStatsController::HandleSendStrengthDelta(
        [[maybe_unused]] AzNetworking::IConnection* invokingConnection, const AZ::s64& strengthDelta)
    {
        m_resourceStatsObject->SetStrengthDelta(strengthDelta);
    }
    void NetworkResourceStatsController::HandleSendStaminaDelta(
        [[maybe_unused]] AzNetworking::IConnection* invokingConnection, const AZ::s64& staminaDelta)
    {
        // This method only does something when the First Person Controller is involved
#ifdef FIRST_PERSON_CONTROLLER
        m_resourceStatsObject->SetStaminaDelta(staminaDelta);
#endif
    }
#endif

    // Event Notification methods for use in scripts
    void NetworkResourceStatsController::OnNetworkTickStart(
        [[maybe_unused]] const float& deltaTime, [[maybe_unused]] const bool& server, [[maybe_unused]] const AZ::EntityId& entity)
    {
    }
    void NetworkResourceStatsController::OnNetworkTickFinish(
        [[maybe_unused]] const float& deltaTime, [[maybe_unused]] const bool& server, [[maybe_unused]] const AZ::EntityId& entity)
    {
    }

    void NetworkResourceStatsController::OnEnableNetworkResourceStatsChanged(const bool& enable)
    {
        m_disabled = !enable;
        m_resourceStatsObject->m_networkResourceStatsEnabled = enable;
    }

    // Request Bus getter and setter methods for use in scripts
    bool NetworkResourceStatsController::GetEnabled() const
    {
        return !m_disabled;
    }
    void NetworkResourceStatsController::SetEnabled(const bool& new_enabled)
    {
        m_disabled = !new_enabled;
        m_resourceStatsObject->m_networkResourceStatsEnabled = new_enabled;
    }
} // namespace ResourceStats
