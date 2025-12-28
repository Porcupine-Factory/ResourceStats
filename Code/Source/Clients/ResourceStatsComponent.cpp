#include <Clients/ResourceStatsComponent.h>
#include <Multiplayer/NetworkResourceStats.h>

#include <AzCore/Component/ComponentApplicationBus.h>
#include <AzCore/Component/Entity.h>
#include <AzCore/Serialization/EditContext.h>

namespace ResourceStats
{
    void ResourceStatsComponent::Reflect(AZ::ReflectContext* rc)
    {
        if (auto sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            sc->Class<ResourceStatsComponent, AZ::Component>()
                // Resources and Statistics
                ->Field("Health", &ResourceStatsComponent::m_health)
                ->Attribute(AZ::Edit::Attributes::Min, 0)
                ->Field("Mana", &ResourceStatsComponent::m_mana)
                ->Attribute(AZ::Edit::Attributes::Min, 0)
                ->Field("Max Health", &ResourceStatsComponent::m_maxHealth)
                ->Attribute(AZ::Edit::Attributes::Min, 0)
                ->Field("Max Mana", &ResourceStatsComponent::m_maxMana)
                ->Attribute(AZ::Edit::Attributes::Min, 0)
                ->Version(1);

            if (AZ::EditContext* ec = sc->GetEditContext())
            {
                using namespace AZ::Edit::Attributes;
                ec->Class<ResourceStatsComponent>(
                      "Resource Stats", "Keeps track of resources and stats for any entity with this component")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->Attribute(Category, "Resource Stats")
                    ->Attribute(AZ::Edit::Attributes::HelpPageURL, "https://www.youtube.com/watch?v=O7rtXNlCNQQ")

                    ->ClassElement(AZ::Edit::ClassElements::Group, "Resources and Statistics")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, false)
                    ->DataElement(
                        nullptr,
                        &ResourceStatsComponent::m_health,
                        "Health",
                        "Initial health")
                    ->DataElement(
                        nullptr,
                        &ResourceStatsComponent::m_mana,
                        "Mana",
                        "Initial mana")
                    ->DataElement(
                        nullptr,
                        &ResourceStatsComponent::m_maxHealth,
                        "Max Health",
                        "Maximum health")
                    ->DataElement(
                        nullptr,
                        &ResourceStatsComponent::m_maxMana,
                        "Max Mana",
                        "Maximum mana");
            }
        }

        if (auto bc = azrtti_cast<AZ::BehaviorContext*>(rc))
        {
            bc->EBus<ResourceStatsComponentNotificationBus>("ResourceStatsComponentNotificationBus")
                ->Handler<ResourceStatsComponentNotificationHandler>();

            bc->EBus<ResourceStatsComponentRequestBus>("ResourceStatsComponentRequestBus")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Module, "controller")
                ->Attribute(AZ::Script::Attributes::Category, "Resource Stats")
                ->Event("Get Health", &ResourceStatsComponentRequests::GetHealth)
                ->Event("Set Health", &ResourceStatsComponentRequests::SetHealth)
                ->Event("Get Mana", &ResourceStatsComponentRequests::GetMana)
                ->Event("Set Mana", &ResourceStatsComponentRequests::SetMana)
                ->Event("Get Max Health", &ResourceStatsComponentRequests::GetMaxHealth)
                ->Event("Set Max Health", &ResourceStatsComponentRequests::SetMaxHealth)
                ->Event("Get Max Mana", &ResourceStatsComponentRequests::GetMaxMana)
                ->Event("Set Max Mana", &ResourceStatsComponentRequests::SetMaxMana)
                ->Event("Set Health Delta", &ResourceStatsComponentRequests::SetHealthDelta)
                ->Event("Set Mana Delta", &ResourceStatsComponentRequests::SetManaDelta)
                ->Event("Get Is Autonomous Client", &ResourceStatsComponentRequests::GetIsAutonomousClient)
                ->Event("Get Is Server", &ResourceStatsComponentRequests::GetIsServer)
                ->Event("Get Is Host", &ResourceStatsComponentRequests::GetIsHost)
                ->Event("Get Locally Enable NetworkResourceStats", &ResourceStatsComponentRequests::GetLocallyEnableNetworkResourceStats)
                ->Event("Set Locally Enable NetworkResourceStats", &ResourceStatsComponentRequests::SetLocallyEnableNetworkResourceStats);

            bc->Class<ResourceStatsComponent>()->RequestBus("ResourceStatsComponentRequestBus");
        }
    }

    void ResourceStatsComponent::Activate()
    {
        AZ::TickBus::Handler::BusConnect();
        ResourceStatsComponentRequestBus::Handler::BusConnect(GetEntityId());
        NetworkResourceStatsControllerNotificationBus::Handler::BusConnect(GetEntityId());
        AZ::EntityBus::Handler::BusConnect(GetEntityId());
    }

    void ResourceStatsComponent::Deactivate()
    {
        AZ::TickBus::Handler::BusDisconnect();
        NetworkResourceStatsControllerNotificationBus::Handler::BusDisconnect();
        NetworkResourceStatsControllerNotificationBus::Handler::BusDisconnect();
        AZ::EntityBus::Handler::BusDisconnect();
    }

    void ResourceStatsComponent::OnEntityActivated([[maybe_unused]] const AZ::EntityId& entityId)
    {
        // Get access to the NetworkResourceStats object and its member
        const AZ::Entity* entity = GetEntity();
        m_networkResourceStatsObject = entity->FindComponent<NetworkResourceStats>();

        // Determine if the NetworkResourceStats is enabled
        if (m_networkResourceStatsObject != nullptr)
        {
            m_networkResourceStatsControllerObject = static_cast<NetworkResourceStatsController*>(m_networkResourceStatsObject->GetController());
            NetworkResourceStatsControllerRequestBus::BroadcastResult(m_networkResourceStatsEnabled, &NetworkResourceStatsControllerRequestBus::Events::GetEnabled);
        }
    }

    void ResourceStatsComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void ResourceStatsComponent::GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    void ResourceStatsComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("ResourceStatsService"));
    }

    void ResourceStatsComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("ResourceStatsService"));
    }

    void ResourceStatsComponent::OnTick(float deltaTime, AZ::ScriptTimePoint)
    {
        ProcessInput(((deltaTime + m_prevDeltaTime) / 2.f), 0);
        m_prevDeltaTime = deltaTime;
    }

    void ResourceStatsComponent::OnNetworkTickStart(const float& deltaTime, const bool& server, const AZ::EntityId& entity)
    {
        if (!m_isAutonomousClient && !m_isServer && !m_isHost)
        {
            NotAutonomousSoDisconnect();
            return;
        }
        if (entity != GetEntityId())
            return;
        if (!((m_isHost && server) || (m_isServer && !server)))
        {
            ResourceStatsComponentNotificationBus::Broadcast(
                &ResourceStatsComponentNotificationBus::Events::OnNetworkResourceStatsTickStart,
                (deltaTime + m_prevNetworkResourceStatsDeltaTime) / 2.f);
            if (!m_networkResourceStatsEnabled)
                NetworkResourceStatsControllerRequestBus::BroadcastResult(m_networkResourceStatsEnabled, &NetworkResourceStatsControllerRequestBus::Events::GetEnabled);
            ProcessInput(((deltaTime + m_prevNetworkResourceStatsDeltaTime) / 2.f), 2);
        }
    }

    void ResourceStatsComponent::OnNetworkTickFinish(const float& deltaTime, const bool& server, const AZ::EntityId& entity)
    {
        if ((!m_isAutonomousClient && !m_isServer && !m_isHost) || (entity != GetEntityId()))
            return;
        if (!((m_isHost && server) || (m_isServer && !server)))
            ResourceStatsComponentNotificationBus::Broadcast(
                &ResourceStatsComponentNotificationBus::Events::OnNetworkResourceStatsTickFinish,
                (deltaTime + m_prevNetworkResourceStatsDeltaTime) / 2.f);
        m_prevNetworkResourceStatsDeltaTime = deltaTime;
    }

    void ResourceStatsComponent::GetNetworkResourceStatsProperties()
    {
        if (m_networkResourceStatsControllerObject != nullptr)
        {
            m_health = m_networkResourceStatsControllerObject->GetHealth();
            m_mana = m_networkResourceStatsControllerObject->GetMana();
            m_maxHealth = m_networkResourceStatsControllerObject->GetMaxHealth();
            m_maxMana = m_networkResourceStatsControllerObject->GetMaxMana();
        }
    }

    void ResourceStatsComponent::SetNetworkResourceStatsProperties() const
    {
        if (m_networkResourceStatsControllerObject != nullptr)
        {
            m_networkResourceStatsControllerObject->SetHealth(m_health);
            m_networkResourceStatsControllerObject->SetMana(m_mana);
            m_networkResourceStatsControllerObject->SetMaxHealth(m_maxHealth);
            m_networkResourceStatsControllerObject->SetMaxMana(m_maxMana);
        }
    }

    // Frame tick == 0, physics fixed timestep == 1 (not used in ResourceStats), network tick == 2
    void ResourceStatsComponent::ProcessInput(const float& deltaTime, const AZ::u8& tickTimestepNetwork)
    {
        if (tickTimestepNetwork == 2)
        {
            GetNetworkResourceStatsProperties();
            SetNetworkResourceStatsProperties();
        }
    }

    // Event Notification methods for use in scripts
    void ResourceStatsComponent::OnNetworkResourceStatsTickStart([[maybe_unused]] const float& deltaTime)
    {
    }
    void ResourceStatsComponent::OnNetworkResourceStatsTickFinish([[maybe_unused]] const float& deltaTime)
    {
    }

    // Request Bus getter and setter methods for use in scripts
    AZ::u32 ResourceStatsComponent::GetHealth() const
    {
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            return m_networkResourceStatsControllerObject->GetHealth();
        else
            return m_health;
    }
    void ResourceStatsComponent::SetHealth(const AZ::u32 new_health)
    {
        m_health = new_health;
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            m_networkResourceStatsControllerObject->SetHealth(m_health);
    }
    AZ::u32 ResourceStatsComponent::GetMana() const
    {
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            return m_networkResourceStatsControllerObject->GetMana();
        else
            return m_mana;
    }
    void ResourceStatsComponent::SetMana(const AZ::u32 new_mana)
    {
        m_mana = new_mana;
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            m_networkResourceStatsControllerObject->SetMana(m_mana);
    }
    AZ::u32 ResourceStatsComponent::GetMaxHealth() const
    {
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            return m_networkResourceStatsControllerObject->GetMaxHealth();
        else
            return m_maxHealth;
    }
    void ResourceStatsComponent::SetMaxHealth(const AZ::u32 new_maxHealth)
    {
        m_maxHealth = new_maxHealth;
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            m_networkResourceStatsControllerObject->SetMaxHealth(m_maxHealth);
    }
    AZ::u32 ResourceStatsComponent::GetMaxMana() const
    {
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            return m_networkResourceStatsControllerObject->GetMaxMana();
        else
            return m_maxMana;
    }
    void ResourceStatsComponent::SetMaxMana(const AZ::u32 new_maxMana)
    {
        m_maxMana = new_maxMana;
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            m_networkResourceStatsControllerObject->SetMaxMana(m_maxMana);
    }
    void ResourceStatsComponent::SetHealthDelta(const AZ::s64& new_healthDelta)
    {
        if (new_healthDelta < 0 && abs(new_healthDelta) >= m_health)
            m_health = 0;
        else
            m_health = AZStd::min<AZ::u32>((m_health + new_healthDelta), m_maxHealth);
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            m_networkResourceStatsControllerObject->SetHealth(m_health);
    }
    void ResourceStatsComponent::SetManaDelta(const AZ::s64& new_manaDelta)
    {
        if (new_manaDelta < 0 && abs(new_manaDelta) >= m_mana)
            m_mana = 0;
        else
            m_mana = AZStd::min<AZ::u32>((m_mana + new_manaDelta), m_maxMana);
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            m_networkResourceStatsControllerObject->SetMana(m_mana);
    }
    bool ResourceStatsComponent::GetIsAutonomousClient() const
    {
        return m_isAutonomousClient;
    }
    bool ResourceStatsComponent::GetIsServer() const
    {
        return m_isServer;
    }
    bool ResourceStatsComponent::GetIsHost() const
    {
        return m_isHost;
    }
    bool ResourceStatsComponent::GetLocallyEnableNetworkResourceStats() const
    {
        return m_networkResourceStatsEnabled;
    }
    void ResourceStatsComponent::SetLocallyEnableNetworkResourceStats(const bool& new_networkResourceStatsEnabled)
    {
        m_networkResourceStatsEnabled = new_networkResourceStatsEnabled;
        NetworkResourceStatsControllerRequestBus::Event(GetEntityId(), &NetworkResourceStatsControllerRequestBus::Events::SetEnabled, m_networkResourceStatsEnabled);
    }
    void ResourceStatsComponent::IsAutonomousSoConnect()
    {
        AZ::TickBus::Handler::BusConnect();
    }
    void ResourceStatsComponent::NotAutonomousSoDisconnect()
    {
        AZ::TickBus::Handler::BusDisconnect();
    }
} // namespace ResourceStats
