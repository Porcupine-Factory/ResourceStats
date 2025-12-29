#pragma once
#include <ResourceStats/NetworkResourceStatsControllerBus.h>

#include <Source/AutoGen/NetworkResourceStats.AutoComponent.h>

#include <Clients/ResourceStatsComponent.h>

namespace ResourceStats
{
    class ResourceStatsComponent;

    class NetworkResourceStatsController
        : public NetworkResourceStatsControllerBase
        , public NetworkResourceStatsControllerRequestBus::Handler
    {
        friend class ResourceStatsComponent;

    public:
        NetworkResourceStatsController(NetworkResourceStats& parent);

        void OnActivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;
        void OnDeactivate(Multiplayer::EntityIsMigrating entityIsMigrating) override;

        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);

        //! Common input creation logic for the NetworkInput.
        //! Fill out the input struct and the MultiplayerInputDriver will send the input data over the network
        //!    to ensure it's processed.
        //! @param input  input structure which to store input data for sending to the authority
        //! @param deltaTime amount of time to integrate the provided inputs over
        void CreateInput(Multiplayer::NetworkInput& input, float deltaTime) override;

        //! Common input processing logic for the NetworkInput.
        //! @param input  input structure to process
        //! @param deltaTime amount of time to integrate the provided inputs over
        void ProcessInput(Multiplayer::NetworkInput& input, float deltaTime) override;

        // NetworkResourceStatsControllerRequestBus
        bool GetEnabled() const override;
        void SetEnabled(const bool& new_enabled) override;

#if AZ_TRAIT_SERVER
        void HandleSendHealthDelta(AzNetworking::IConnection* invokingConnection, const AZ::s64& healthDelta) override;
        void HandleSendManaDelta(AzNetworking::IConnection* invokingConnection, const AZ::s64& manaDelta) override;
        void HandleSendStaminaDelta(AzNetworking::IConnection* invokingConnection, const AZ::s64& staminaDelta) override;
#endif

    private:
        // NetworkResourceStatsControllerNotificationBus
        void OnNetworkTickStart(const float& deltaTime, const bool& server, const AZ::EntityId& entity);
        void OnNetworkTickFinish(const float& deltaTime, const bool& server, const AZ::EntityId& entity);

        // Used to initialize Network Properties from initial values in the Resource Stats component
        bool m_init = true;

        // ResourceStatsComponent objects
        ResourceStatsComponent* m_resourceStatsObject = nullptr;

        // Signals when the controller is determined to be autonomous or not
        bool m_autonomousNotDetermined = true;

        // EnableNetworkResourceStats Changed Event
        AZ::Event<bool>::Handler m_enableNetworkResourceStatsChangedEvent;
        void OnEnableNetworkResourceStatsChanged(const bool& enable);
        bool m_disabled = false;
    };
} // namespace ResourceStats
