
#include <AzCore/Serialization/SerializeContext.h>
#include "ResourceStatsEditorSystemComponent.h"

#include <ResourceStats/ResourceStatsTypeIds.h>

namespace ResourceStats
{
    AZ_COMPONENT_IMPL(ResourceStatsEditorSystemComponent, "ResourceStatsEditorSystemComponent",
        ResourceStatsEditorSystemComponentTypeId, BaseSystemComponent);

    void ResourceStatsEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<ResourceStatsEditorSystemComponent, ResourceStatsSystemComponent>()
                ->Version(0);
        }
    }

    ResourceStatsEditorSystemComponent::ResourceStatsEditorSystemComponent() = default;

    ResourceStatsEditorSystemComponent::~ResourceStatsEditorSystemComponent() = default;

    void ResourceStatsEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("ResourceStatsEditorService"));
    }

    void ResourceStatsEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("ResourceStatsEditorService"));
    }

    void ResourceStatsEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void ResourceStatsEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void ResourceStatsEditorSystemComponent::Activate()
    {
        ResourceStatsSystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
    }

    void ResourceStatsEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        ResourceStatsSystemComponent::Deactivate();
    }

} // namespace ResourceStats
