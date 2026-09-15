#include <Clients/ResourceStatsComponent.h>
#ifdef NETWORKRESOURCESTATS
#include <Multiplayer/NetworkResourceStats.h>
#endif

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
                ->Field("Max Health", &ResourceStatsComponent::m_maxHealth)
                ->Attribute(AZ::Edit::Attributes::Min, 0)
                ->Field("Mana", &ResourceStatsComponent::m_mana)
                ->Attribute(AZ::Edit::Attributes::Min, 0)
                ->Field("Max Mana", &ResourceStatsComponent::m_maxMana)
                ->Attribute(AZ::Edit::Attributes::Min, 0)
                ->Field("Strength", &ResourceStatsComponent::m_strength)
                ->Attribute(AZ::Edit::Attributes::Min, 0)
                ->Field("Max Strength", &ResourceStatsComponent::m_maxStrength)
                ->Attribute(AZ::Edit::Attributes::Min, 0)
                ->Field("Attack", &ResourceStatsComponent::m_attack)
                ->Attribute(AZ::Edit::Attributes::Min, 0)
                ->Field("Max Attack", &ResourceStatsComponent::m_maxAttack)
                ->Attribute(AZ::Edit::Attributes::Min, 0)
                ->Field("Armor", &ResourceStatsComponent::m_armor)
                ->Attribute(AZ::Edit::Attributes::Min, 0)
                ->Field("Max Armor", &ResourceStatsComponent::m_maxArmor)
                ->Attribute(AZ::Edit::Attributes::Min, 0)
#ifdef FIRST_PERSON_CONTROLLER
                ->Field("Stamina", &ResourceStatsComponent::m_stamina)
                ->Attribute(AZ::Edit::Attributes::Min, 0)
                ->Attribute(AZ::Edit::Attributes::Suffix, " ms")
                ->Field("Max Stamina", &ResourceStatsComponent::m_maxStamina)
                ->Attribute(AZ::Edit::Attributes::Min, 0)
                ->Attribute(AZ::Edit::Attributes::Suffix, " ms")
                ->Field("Stamina Cooldown Time", &ResourceStatsComponent::m_staminaCooldownTime)
                ->Attribute(AZ::Edit::Attributes::Min, 0)
                ->Attribute(AZ::Edit::Attributes::Suffix, " ms")
#endif
                ->Version(1);

            if (AZ::EditContext* ec = sc->GetEditContext())
            {
                using namespace AZ::Edit::Attributes;
                ec->Class<ResourceStatsComponent>("Resource Stats", "Keeps track of resources and stats for any entity with this component")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->Attribute(Category, "Resource Stats")

                    ->ClassElement(AZ::Edit::ClassElements::Group, "Resources and Statistics")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, false)
                    ->DataElement(nullptr, &ResourceStatsComponent::m_health, "Health", "Initial health")
                    ->DataElement(nullptr, &ResourceStatsComponent::m_maxHealth, "Max Health", "Maximum health")
                    ->DataElement(nullptr, &ResourceStatsComponent::m_mana, "Mana", "Initial mana")
                    ->DataElement(nullptr, &ResourceStatsComponent::m_maxMana, "Max Mana", "Maximum mana")
                    ->DataElement(nullptr, &ResourceStatsComponent::m_strength, "Strength", "Initial strength")
                    ->DataElement(nullptr, &ResourceStatsComponent::m_maxStrength, "Max Strength", "Maximum strength")
                    ->DataElement(nullptr, &ResourceStatsComponent::m_attack, "Attack", "Initial Attack")
                    ->DataElement(nullptr, &ResourceStatsComponent::m_maxAttack, "Max Attack", "Maximum Attack")
                    ->DataElement(nullptr, &ResourceStatsComponent::m_armor, "Armor", "Initial Armor")
                    ->DataElement(nullptr, &ResourceStatsComponent::m_maxArmor, "Max Armor", "Maximum Armor")
#ifdef FIRST_PERSON_CONTROLLER
                    ->DataElement(
                        nullptr,
                        &ResourceStatsComponent::m_stamina,
                        "Stamina",
                        "Initial stamina, in units of milliseconds of sprinting time")
                    ->DataElement(
                        nullptr,
                        &ResourceStatsComponent::m_maxStamina,
                        "Max Stamina",
                        "Maximum stamina, in units of milliseconds of sprinting time")
                    ->DataElement(
                        nullptr,
                        &ResourceStatsComponent::m_staminaCooldownTime,
                        "Stamina Cooldown Time",
                        "Stamina cooldown time, The time in milliseconds required to wait before sprinting or using Stamina "
                        "once Max Stamina has been reached or Stamina hits 0. Also Determines the regeneration rate.")
#endif
                    ;
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
                ->Event("Get Max Health", &ResourceStatsComponentRequests::GetMaxHealth)
                ->Event("Set Max Health", &ResourceStatsComponentRequests::SetMaxHealth)
                ->Event("Set Health Delta", &ResourceStatsComponentRequests::SetHealthDelta)
                ->Event("Get Mana", &ResourceStatsComponentRequests::GetMana)
                ->Event("Set Mana", &ResourceStatsComponentRequests::SetMana)
                ->Event("Get Max Mana", &ResourceStatsComponentRequests::GetMaxMana)
                ->Event("Set Max Mana", &ResourceStatsComponentRequests::SetMaxMana)
                ->Event("Set Mana Delta", &ResourceStatsComponentRequests::SetManaDelta)
                ->Event("Get Strength", &ResourceStatsComponentRequests::GetStrength)
                ->Event("Set Strength", &ResourceStatsComponentRequests::SetStrength)
                ->Event("Get Max Strength", &ResourceStatsComponentRequests::GetMaxStrength)
                ->Event("Set Max Strength", &ResourceStatsComponentRequests::SetMaxStrength)
                ->Event("Set Strength Delta", &ResourceStatsComponentRequests::SetStrengthDelta)
                ->Event("Get Attack", &ResourceStatsComponentRequests::GetAttack)
                ->Event("Set Attack", &ResourceStatsComponentRequests::SetAttack)
                ->Event("Get Max Attack", &ResourceStatsComponentRequests::GetMaxAttack)
                ->Event("Set Max Attack", &ResourceStatsComponentRequests::SetMaxAttack)
                ->Event("Set Attack Delta", &ResourceStatsComponentRequests::SetAttackDelta)
                ->Event("Get Armor", &ResourceStatsComponentRequests::GetArmor)
                ->Event("Set Armor", &ResourceStatsComponentRequests::SetArmor)
                ->Event("Get Max Armor", &ResourceStatsComponentRequests::GetMaxArmor)
                ->Event("Set Max Armor", &ResourceStatsComponentRequests::SetMaxArmor)
                ->Event("Set Armor Delta", &ResourceStatsComponentRequests::SetArmorDelta)
#ifdef FIRST_PERSON_CONTROLLER
                ->Event("Get Stamina", &ResourceStatsComponentRequests::GetStamina)
                ->Event("Set Stamina", &ResourceStatsComponentRequests::SetStamina)
                ->Event("Get Max Stamina", &ResourceStatsComponentRequests::GetMaxStamina)
                ->Event("Set Max Stamina", &ResourceStatsComponentRequests::SetMaxStamina)
                ->Event("Set Stamina Delta", &ResourceStatsComponentRequests::SetStaminaDelta)
                ->Event("Get Stamina Cooldown Time", &ResourceStatsComponentRequests::GetStaminaCooldownTime)
                ->Event("Set Stamina Cooldown Time", &ResourceStatsComponentRequests::SetStaminaCooldownTime)
                ->Event("Get Stamina Percentage", &ResourceStatsComponentRequests::GetStaminaPercentage)
#endif
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
#ifdef NETWORKRESOURCESTATS
        NetworkResourceStatsControllerNotificationBus::Handler::BusConnect(GetEntityId());
#endif
        AZ::EntityBus::Handler::BusConnect(GetEntityId());
    }

    void ResourceStatsComponent::Deactivate()
    {
        AZ::EntityBus::Handler::BusDisconnect();
#ifdef NETWORKRESOURCESTATS
        NetworkResourceStatsControllerNotificationBus::Handler::BusDisconnect();
#endif
        ResourceStatsComponentRequestBus::Handler::BusDisconnect();
        AZ::TickBus::Handler::BusDisconnect();
    }

    void ResourceStatsComponent::OnEntityActivated([[maybe_unused]] const AZ::EntityId& entityId)
    {
#ifdef NETWORKRESOURCESTATS
        // Get access to the NetworkResourceStats object and its member
        const AZ::Entity* entity = GetEntity();
        m_networkResourceStatsObject = entity->FindComponent<NetworkResourceStats>();

        // Determine if the NetworkResourceStats is enabled
        if (m_networkResourceStatsObject != nullptr)
        {
            m_networkResourceStatsControllerObject =
                aznumeric_cast<NetworkResourceStatsController*>(m_networkResourceStatsObject->GetController());
            NetworkResourceStatsControllerRequestBus::BroadcastResult(
                m_networkResourceStatsEnabled, &NetworkResourceStatsControllerRequestBus::Events::GetEnabled);
        }
#endif
    }

    void ResourceStatsComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void ResourceStatsComponent::GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
#ifdef FIRST_PERSON_CONTROLLER
        dependent.push_back(AZ_CRC_CE("FirstPersonControllerService"));
#ifdef NETWORKRESOURCESTATS
        dependent.push_back(AZ_CRC_CE("NetworkFPCService"));
#endif
#endif
    }

    void ResourceStatsComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("ResourceStatsService"));
        provided.push_back(AZ_CRC_CE("ResourceStatsComponent"));
    }

    void ResourceStatsComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("ResourceStatsService"));
    }

    void ResourceStatsComponent::OnTick(float deltaTime, AZ::ScriptTimePoint)
    {
        ProcessInput(deltaTime, 0);
    }

    void ResourceStatsComponent::OnNetworkTickStart(const float deltaTime, const bool server, const AZ::EntityId& entityId)
    {
        if (!m_isAutonomousClient && !m_isServer && !m_isHost)
        {
            NotAutonomousSoDisconnect();
            return;
        }
        if (entityId != GetEntityId())
            return;
        if (!((m_isHost && server) || (m_isServer && !server)))
        {
            ResourceStatsComponentNotificationBus::Broadcast(
                &ResourceStatsComponentNotificationBus::Events::OnNetworkResourceStatsTickStart, deltaTime);
#ifdef NETWORKRESOURCESTATS
            if (!m_networkResourceStatsEnabled)
                NetworkResourceStatsControllerRequestBus::BroadcastResult(
                    m_networkResourceStatsEnabled, &NetworkResourceStatsControllerRequestBus::Events::GetEnabled);
#endif
            ProcessInput(deltaTime, 2);
        }
    }

#ifdef FIRST_PERSON_CONTROLLER
    void ResourceStatsComponent::InitStamina()
    {
        if (m_networkResourceStatsEnabled && m_networkResourceStatsObject != nullptr)
        {
#ifdef NETWORKRESOURCESTATS
            m_stamina = m_networkResourceStatsControllerObject->GetStamina();
            m_maxStamina = m_networkResourceStatsControllerObject->GetMaxStamina();
            m_staminaCooldownTime = m_networkResourceStatsControllerObject->GetStaminaCooldownTime();
#endif
        }
        FirstPersonController::FirstPersonControllerComponentRequestBus::Event(
            GetEntityId(),
            &FirstPersonController::FirstPersonControllerComponentRequests::SetSprintHeldTime,
            (m_maxStamina - m_stamina) / 1000.f);
        FirstPersonController::FirstPersonControllerComponentRequestBus::Event(
            GetEntityId(), &FirstPersonController::FirstPersonControllerComponentRequests::SetSprintMaxTime, m_maxStamina / 1000.f);
        FirstPersonController::FirstPersonControllerComponentRequestBus::Event(
            GetEntityId(),
            &FirstPersonController::FirstPersonControllerComponentRequests::SetSprintTotalCooldownTime,
            m_staminaCooldownTime / 1000.f);
        m_staminaInitialized = true;
    }

    void ResourceStatsComponent::UpdateStamina()
    {
        if (m_staminaInitialized)
        {
            float sprintMaxTime;
            FirstPersonController::FirstPersonControllerComponentRequestBus::EventResult(
                sprintMaxTime, GetEntityId(), &FirstPersonController::FirstPersonControllerComponentRequests::GetSprintMaxTime);
            m_maxStamina = aznumeric_cast<AZ::u32>(sprintMaxTime * 1000.f);
            float sprintHeldTime;
            FirstPersonController::FirstPersonControllerComponentRequestBus::EventResult(
                sprintHeldTime, GetEntityId(), &FirstPersonController::FirstPersonControllerComponentRequests::GetSprintHeldTime);
            m_stamina = (m_maxStamina - aznumeric_cast<AZ::u32>(sprintHeldTime * 1000.f));
            float sprintCooldownTime;
            FirstPersonController::FirstPersonControllerComponentRequestBus::EventResult(
                sprintCooldownTime,
                GetEntityId(),
                &FirstPersonController::FirstPersonControllerComponentRequests::GetSprintTotalCooldownTime);
            m_staminaCooldownTime = aznumeric_cast<AZ::u32>(sprintCooldownTime * 1000.f);
        }
    }
#endif

    void ResourceStatsComponent::OnNetworkTickFinish(const float deltaTime, const bool server, const AZ::EntityId& entityId)
    {
        if ((!m_isAutonomousClient && !m_isServer && !m_isHost) || (entityId != GetEntityId()))
            return;
        if (!((m_isHost && server) || (m_isServer && !server)))
            ResourceStatsComponentNotificationBus::Broadcast(
                &ResourceStatsComponentNotificationBus::Events::OnNetworkResourceStatsTickFinish, deltaTime);
    }

    void ResourceStatsComponent::GetNetworkResourceStatsProperties()
    {
        if (m_networkResourceStatsControllerObject != nullptr)
        {
#ifdef NETWORKRESOURCESTATS
            m_health = m_networkResourceStatsControllerObject->GetHealth();
            m_maxHealth = m_networkResourceStatsControllerObject->GetMaxHealth();
            m_mana = m_networkResourceStatsControllerObject->GetMana();
            m_maxMana = m_networkResourceStatsControllerObject->GetMaxMana();
            m_strength = m_networkResourceStatsControllerObject->GetStrength();
            m_maxStrength = m_networkResourceStatsControllerObject->GetMaxStrength();
            m_attack = m_networkResourceStatsControllerObject->GetAttack();
            m_maxAttack = m_networkResourceStatsControllerObject->GetMaxAttack();
            m_armor = m_networkResourceStatsControllerObject->GetArmor();
            m_maxArmor = m_networkResourceStatsControllerObject->GetMaxArmor();
#ifdef FIRST_PERSON_CONTROLLER
            m_stamina = m_networkResourceStatsControllerObject->GetStamina();
            m_maxStamina = m_networkResourceStatsControllerObject->GetMaxStamina();
            m_staminaCooldownTime = m_networkResourceStatsControllerObject->GetStaminaCooldownTime();
#endif
#endif
        }
    }

    void ResourceStatsComponent::SetNetworkResourceStatsProperties() const
    {
        if (m_networkResourceStatsControllerObject != nullptr && (m_isServer || m_isHost))
        {
#ifdef NETWORKRESOURCESTATS
            m_networkResourceStatsControllerObject->SetHealth(m_health);
            m_networkResourceStatsControllerObject->SetMaxHealth(m_maxHealth);
            m_networkResourceStatsControllerObject->SetMana(m_mana);
            m_networkResourceStatsControllerObject->SetMaxMana(m_maxMana);
            m_networkResourceStatsControllerObject->SetStrength(m_strength);
            m_networkResourceStatsControllerObject->SetMaxStrength(m_maxStrength);
            m_networkResourceStatsControllerObject->SetAttack(m_attack);
            m_networkResourceStatsControllerObject->SetMaxAttack(m_maxAttack);
            m_networkResourceStatsControllerObject->SetArmor(m_armor);
            m_networkResourceStatsControllerObject->SetMaxArmor(m_maxArmor);
#ifdef FIRST_PERSON_CONTROLLER
            m_networkResourceStatsControllerObject->SetStamina(m_stamina);
            m_networkResourceStatsControllerObject->SetMaxStamina(m_maxStamina);
#endif
#endif
        }
    }

    // Frame tick == 0, physics fixed timestep == 1 (not used in ResourceStats), network tick == 2
    void ResourceStatsComponent::ProcessInput([[maybe_unused]] const float deltaTime, const AZ::u8 tickTimestepNetwork)
    {
#ifdef FIRST_PERSON_CONTROLLER
        if (!m_staminaInitialized && !m_networkResourceStatsEnabled)
        {
            // Prioritize the initial stamina values in this component over the ones found in First Person Controller
            InitStamina();
        }
#endif
        if (tickTimestepNetwork == 2)
        {
#ifdef FIRST_PERSON_CONTROLLER
            // Prioritize the initial stamina values in this component over the ones found in First Person Controller
            if (!m_staminaInitialized)
                InitStamina();
#endif
            GetNetworkResourceStatsProperties();
        }
#ifdef FIRST_PERSON_CONTROLLER
        UpdateStamina();
#endif
        if (tickTimestepNetwork == 2)
        {
            SetNetworkResourceStatsProperties();
        }
    }

    // Request Bus getter and setter methods for use in scripts
    AZ::u32 ResourceStatsComponent::GetHealth() const
    {
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            return m_networkResourceStatsControllerObject->GetHealth();
        else
#endif
            return m_health;
    }
    void ResourceStatsComponent::SetHealth(const AZ::u32 health)
    {
        m_health = health;
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            m_networkResourceStatsControllerObject->SetHealth(m_health);
#endif
    }
    AZ::u32 ResourceStatsComponent::GetMaxHealth() const
    {
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            return m_networkResourceStatsControllerObject->GetMaxHealth();
        else
#endif
            return m_maxHealth;
    }
    void ResourceStatsComponent::SetMaxHealth(const AZ::u32 maxHealth)
    {
        m_maxHealth = maxHealth;
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            m_networkResourceStatsControllerObject->SetMaxHealth(m_maxHealth);
#endif
    }
    void ResourceStatsComponent::SetHealthDelta(const AZ::s64 healthDelta)
    {
        if (healthDelta > 0 && healthDelta > AZStd::numeric_limits<AZ::u32>::max() - m_health)
            m_health = AZStd::numeric_limits<AZ::u32>::max();
        else
        {
            const AZ::s64 currentHealth = aznumeric_cast<AZ::s64>(m_health);
            const AZ::s64 maxHealth = aznumeric_cast<AZ::s64>(m_maxHealth);
            const AZ::s64 newHealth = AZStd::clamp(currentHealth + healthDelta, 0LL, maxHealth);
            m_health = aznumeric_cast<AZ::u32>(newHealth);
        }
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            m_networkResourceStatsControllerObject->SetHealth(m_health);
#endif
    }
    AZ::u32 ResourceStatsComponent::GetMana() const
    {
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            return m_networkResourceStatsControllerObject->GetMana();
        else
#endif
            return m_mana;
    }
    void ResourceStatsComponent::SetMana(const AZ::u32 mana)
    {
        m_mana = mana;
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            m_networkResourceStatsControllerObject->SetMana(m_mana);
#endif
    }
    AZ::u32 ResourceStatsComponent::GetMaxMana() const
    {
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            return m_networkResourceStatsControllerObject->GetMaxMana();
        else
#endif
            return m_maxMana;
    }
    void ResourceStatsComponent::SetMaxMana(const AZ::u32 maxMana)
    {
        m_maxMana = maxMana;
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            m_networkResourceStatsControllerObject->SetMaxMana(m_maxMana);
#endif
    }
    void ResourceStatsComponent::SetManaDelta(const AZ::s64 manaDelta)
    {
        if (manaDelta > 0 && manaDelta > AZStd::numeric_limits<AZ::u32>::max() - m_mana)
            m_mana = AZStd::numeric_limits<AZ::u32>::max();
        else
        {
            const AZ::s64 currentMana = aznumeric_cast<AZ::s64>(m_mana);
            const AZ::s64 maxMana = aznumeric_cast<AZ::s64>(m_maxMana);
            const AZ::s64 newMana = AZStd::clamp(currentMana + manaDelta, 0LL, maxMana);
            m_mana = aznumeric_cast<AZ::u32>(newMana);
        }
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            m_networkResourceStatsControllerObject->SetMana(m_mana);
#endif
    }
    AZ::u32 ResourceStatsComponent::GetStrength() const
    {
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            return m_networkResourceStatsControllerObject->GetStrength();
        else
#endif
            return m_strength;
    }
    void ResourceStatsComponent::SetStrength(const AZ::u32 strength)
    {
        m_strength = strength;
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            m_networkResourceStatsControllerObject->SetStrength(m_strength);
#endif
    }
    AZ::u32 ResourceStatsComponent::GetMaxStrength() const
    {
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            return m_networkResourceStatsControllerObject->GetMaxStrength();
        else
#endif
            return m_maxStrength;
    }
    void ResourceStatsComponent::SetMaxStrength(const AZ::u32 maxStrength)
    {
        m_maxStrength = maxStrength;
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            m_networkResourceStatsControllerObject->SetMaxStrength(m_maxStrength);
#endif
    }
    void ResourceStatsComponent::SetStrengthDelta(const AZ::s64 strengthDelta)
    {
        if (strengthDelta > 0 && strengthDelta > AZStd::numeric_limits<AZ::u32>::max() - m_strength)
            m_strength = AZStd::numeric_limits<AZ::u32>::max();
        else
        {
            const AZ::s64 currentStrength = aznumeric_cast<AZ::s64>(m_strength);
            const AZ::s64 maxStrength = aznumeric_cast<AZ::s64>(m_maxStrength);
            const AZ::s64 newStrength = AZStd::clamp(currentStrength + strengthDelta, 0LL, maxStrength);
            m_strength = aznumeric_cast<AZ::u32>(newStrength);
        }
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            m_networkResourceStatsControllerObject->SetStrength(m_strength);
#endif
    }
    AZ::u32 ResourceStatsComponent::GetAttack() const
    {
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            return m_networkResourceStatsControllerObject->GetAttack();
        else
#endif
            return m_attack;
    }
    void ResourceStatsComponent::SetAttack(const AZ::u32 attack)
    {
        m_attack = attack;
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            m_networkResourceStatsControllerObject->SetAttack(m_attack);
#endif
    }
    AZ::u32 ResourceStatsComponent::GetMaxAttack() const
    {
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            return m_networkResourceStatsControllerObject->GetMaxAttack();
        else
#endif
            return m_maxAttack;
    }
    void ResourceStatsComponent::SetMaxAttack(const AZ::u32 maxAttack)
    {
        m_maxAttack = maxAttack;
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            m_networkResourceStatsControllerObject->SetMaxAttack(m_maxAttack);
#endif
    }
    void ResourceStatsComponent::SetAttackDelta(const AZ::s64 attackDelta)
    {
        if (attackDelta > 0 && attackDelta > AZStd::numeric_limits<AZ::u32>::max() - m_attack)
            m_attack = AZStd::numeric_limits<AZ::u32>::max();
        else
        {
            const AZ::s64 currentAttack = aznumeric_cast<AZ::s64>(m_attack);
            const AZ::s64 maxAttack = aznumeric_cast<AZ::s64>(m_maxAttack);
            const AZ::s64 newAttack = AZStd::clamp(currentAttack + attackDelta, 0LL, maxAttack);
            m_attack = aznumeric_cast<AZ::u32>(newAttack);
        }
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            m_networkResourceStatsControllerObject->SetAttack(m_attack);
#endif
    }
    AZ::u32 ResourceStatsComponent::GetArmor() const
    {
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            return m_networkResourceStatsControllerObject->GetArmor();
        else
#endif
            return m_armor;
    }
    void ResourceStatsComponent::SetArmor(const AZ::u32 armor)
    {
        m_armor = armor;
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            m_networkResourceStatsControllerObject->SetArmor(m_armor);
#endif
    }
    AZ::u32 ResourceStatsComponent::GetMaxArmor() const
    {
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            return m_networkResourceStatsControllerObject->GetMaxArmor();
        else
#endif
            return m_maxArmor;
    }
    void ResourceStatsComponent::SetMaxArmor(const AZ::u32 maxArmor)
    {
        m_maxArmor = maxArmor;
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            m_networkResourceStatsControllerObject->SetMaxArmor(m_maxArmor);
#endif
    }
    void ResourceStatsComponent::SetArmorDelta(const AZ::s64 armorDelta)
    {
        if (armorDelta > 0 && armorDelta > AZStd::numeric_limits<AZ::u32>::max() - m_armor)
            m_armor = AZStd::numeric_limits<AZ::u32>::max();
        else
        {
            const AZ::s64 currentArmor = aznumeric_cast<AZ::s64>(m_armor);
            const AZ::s64 maxArmor = aznumeric_cast<AZ::s64>(m_maxArmor);
            const AZ::s64 newArmor = AZStd::clamp(currentArmor + armorDelta, 0LL, maxArmor);
            m_armor = aznumeric_cast<AZ::u32>(newArmor);
        }
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            m_networkResourceStatsControllerObject->SetArmor(m_armor);
#endif
    }
#ifdef FIRST_PERSON_CONTROLLER
    AZ::u32 ResourceStatsComponent::GetStamina() const
    {
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            return m_networkResourceStatsControllerObject->GetStamina();
        else
#endif
            return m_stamina;
    }
    void ResourceStatsComponent::SetStamina(const AZ::u32 stamina)
    {
        m_stamina = stamina;
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            m_networkResourceStatsControllerObject->SetStamina(m_stamina);
#endif
        FirstPersonController::FirstPersonControllerComponentRequestBus::Event(
            GetEntityId(),
            &FirstPersonController::FirstPersonControllerComponentRequests::SetSprintHeldTime,
            (m_maxStamina - m_stamina) / 1000.f);
    }
    AZ::u32 ResourceStatsComponent::GetMaxStamina() const
    {
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            return m_networkResourceStatsControllerObject->GetMaxStamina();
        else
#endif
            return m_maxStamina;
    }
    void ResourceStatsComponent::SetMaxStamina(const AZ::u32 maxStamina)
    {
        m_maxStamina = maxStamina;
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            m_networkResourceStatsControllerObject->SetMaxStamina(m_maxStamina);
#endif
        FirstPersonController::FirstPersonControllerComponentRequestBus::Event(
            GetEntityId(), &FirstPersonController::FirstPersonControllerComponentRequests::SetSprintMaxTime, m_maxStamina / 1000.f);
    }
    void ResourceStatsComponent::SetStaminaDelta(const AZ::s64 staminaDelta)
    {
        if (staminaDelta > 0 && staminaDelta > AZStd::numeric_limits<AZ::u32>::max() - m_stamina)
            m_stamina = AZStd::numeric_limits<AZ::u32>::max();
        else
        {
            const AZ::s64 currentStamina = aznumeric_cast<AZ::s64>(m_stamina);
            const AZ::s64 maxStamina = aznumeric_cast<AZ::s64>(m_maxStamina);
            const AZ::s64 newStamina = AZStd::clamp(currentStamina + staminaDelta, 0LL, maxStamina);
            m_stamina = aznumeric_cast<AZ::u32>(newStamina);
        }
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            m_networkResourceStatsControllerObject->SetStamina(m_stamina);
#endif
        FirstPersonController::FirstPersonControllerComponentRequestBus::Event(
            GetEntityId(),
            &FirstPersonController::FirstPersonControllerComponentRequests::SetSprintHeldTime,
            (m_maxStamina - m_stamina) / 1000.f);
    }
    AZ::u32 ResourceStatsComponent::GetStaminaCooldownTime() const
    {
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            return m_networkResourceStatsControllerObject->GetStaminaCooldownTime();
        else
#endif
            return m_staminaCooldownTime;
    }
    void ResourceStatsComponent::SetStaminaCooldownTime(const AZ::u32 staminaCooldownTime)
    {
        m_staminaCooldownTime = staminaCooldownTime;
#ifdef NETWORKRESOURCESTATS
        if (m_networkResourceStatsEnabled && m_networkResourceStatsControllerObject != nullptr)
            m_networkResourceStatsControllerObject->SetStaminaCooldownTime(m_staminaCooldownTime);
#endif
        FirstPersonController::FirstPersonControllerComponentRequestBus::Event(
            GetEntityId(),
            &FirstPersonController::FirstPersonControllerComponentRequests::SetSprintTotalCooldownTime,
            m_staminaCooldownTime / 1000.f);
    }
    float ResourceStatsComponent::GetStaminaPercentage() const
    {
        // The stamina percentage is simply taken from the First Person Controller
        float staminaPercentage;
        FirstPersonController::FirstPersonControllerComponentRequestBus::EventResult(
            staminaPercentage, GetEntityId(), &FirstPersonController::FirstPersonControllerComponentRequests::GetStaminaPercentage);
        return staminaPercentage;
    }
#endif
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
    void ResourceStatsComponent::SetLocallyEnableNetworkResourceStats(const bool networkResourceStatsEnabled)
    {
        m_networkResourceStatsEnabled = networkResourceStatsEnabled;
#ifdef NETWORKRESOURCESTATS
        NetworkResourceStatsControllerRequestBus::Event(
            GetEntityId(), &NetworkResourceStatsControllerRequestBus::Events::SetEnabled, m_networkResourceStatsEnabled);
#endif
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
