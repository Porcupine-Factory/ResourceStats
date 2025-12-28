
#include <ResourceStats/ResourceStatsTypeIds.h>
#include <ResourceStatsModuleInterface.h>
#include "ResourceStatsEditorSystemComponent.h"

namespace ResourceStats
{
    class ResourceStatsEditorModule
        : public ResourceStatsModuleInterface
    {
    public:
        AZ_RTTI(ResourceStatsEditorModule, ResourceStatsEditorModuleTypeId, ResourceStatsModuleInterface);
        AZ_CLASS_ALLOCATOR(ResourceStatsEditorModule, AZ::SystemAllocator);

        ResourceStatsEditorModule()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
            // This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(m_descriptors.end(), {
                ResourceStatsEditorSystemComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList {
                azrtti_typeid<ResourceStatsEditorSystemComponent>(),
            };
        }
    };
}// namespace ResourceStats

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME, _Editor), ResourceStats::ResourceStatsEditorModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_ResourceStats_Editor, ResourceStats::ResourceStatsEditorModule)
#endif
