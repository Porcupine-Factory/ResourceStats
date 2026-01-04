
#include "ResourceStatsSystemComponent.h"

#include <ResourceStats/ResourceStatsTypeIds.h>

#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#if __has_include(<Source/AutoGen/AutoComponentTypes.h>)
#include <Source/AutoGen/AutoComponentTypes.h>
#endif

namespace ResourceStats
{
    AZ_COMPONENT_IMPL(ResourceStatsSystemComponent, "ResourceStatsSystemComponent", ResourceStatsSystemComponentTypeId);

    void ResourceStatsSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<ResourceStatsSystemComponent, AZ::Component>()->Version(0);

            if (AZ::EditContext* ec = serializeContext->GetEditContext())
            {
                ec->Class<ResourceStatsSystemComponent>("ResourceStats", "[Description of functionality provided by this System Component]")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("System"))
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true);

#ifdef NETWORK_RESOURCE_STATS
                ec->Class<ResourceStatsSystemComponent>(
                      "NetworkResourceStats", "[Description of functionality provided by this System Component]")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("System"))
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true);
#endif
            }
        }
    }

    void ResourceStatsSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("ResourceStatsService"));
#ifdef NETWORK_RESOURCE_STATS
        provided.push_back(AZ_CRC_CE("NetworkResourceStatsService"));
#endif
    }

    void ResourceStatsSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("ResourceStatsService"));
#ifdef NETWORK_RESOURCE_STATS
        incompatible.push_back(AZ_CRC_CE("NetworkResourceStatsService"));
#endif
    }

    void ResourceStatsSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void ResourceStatsSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    ResourceStatsSystemComponent::ResourceStatsSystemComponent()
    {
        if (ResourceStatsInterface::Get() == nullptr)
        {
            ResourceStatsInterface::Register(this);
        }
    }

    ResourceStatsSystemComponent::~ResourceStatsSystemComponent()
    {
        if (ResourceStatsInterface::Get() == this)
        {
            ResourceStatsInterface::Unregister(this);
        }
    }

    void ResourceStatsSystemComponent::Init()
    {
    }

    void ResourceStatsSystemComponent::Activate()
    {
        ResourceStatsRequestBus::Handler::BusConnect();
        AZ::TickBus::Handler::BusConnect();
#ifdef NETWORK_RESOURCE_STATS
        NetworkResourceStatsRequestBus::Handler::BusConnect();
        // Register multiplayer components
        RegisterMultiplayerComponents();
#endif
    }

    void ResourceStatsSystemComponent::Deactivate()
    {
        AZ::TickBus::Handler::BusDisconnect();
#ifdef NETWORK_RESOURCE_STATS
        NetworkResourceStatsRequestBus::Handler::BusDisconnect();
#endif
        ResourceStatsRequestBus::Handler::BusDisconnect();
    }

    void ResourceStatsSystemComponent::OnTick([[maybe_unused]] float deltaTime, [[maybe_unused]] AZ::ScriptTimePoint time)
    {
    }

} // namespace ResourceStats
