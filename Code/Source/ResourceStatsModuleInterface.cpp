
#include "ResourceStatsModuleInterface.h"
#include <AzCore/Memory/Memory.h>

#include <ResourceStats/ResourceStatsTypeIds.h>

#include <Clients/ResourceStatsSystemComponent.h>

namespace ResourceStats
{
    AZ_TYPE_INFO_WITH_NAME_IMPL(ResourceStatsModuleInterface, "ResourceStatsModuleInterface", ResourceStatsModuleInterfaceTypeId);
    AZ_RTTI_NO_TYPE_INFO_IMPL(ResourceStatsModuleInterface, AZ::Module);
    AZ_CLASS_ALLOCATOR_IMPL(ResourceStatsModuleInterface, AZ::SystemAllocator);

    ResourceStatsModuleInterface::ResourceStatsModuleInterface()
    {
        // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
        // Add ALL components descriptors associated with this gem to m_descriptors.
        // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
        // This happens through the [MyComponent]::Reflect() function.
        m_descriptors.insert(
            m_descriptors.end(), { ResourceStatsSystemComponent::CreateDescriptor(), ResourceStatsComponent::CreateDescriptor() });

#ifdef NETWORK_RESOURCE_STATS
        //< Register multiplayer components
        CreateComponentDescriptors(m_descriptors);
#endif
    }

    AZ::ComponentTypeList ResourceStatsModuleInterface::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{
            azrtti_typeid<ResourceStatsSystemComponent>(),
        };
    }
} // namespace ResourceStats
