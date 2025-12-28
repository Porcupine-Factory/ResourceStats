
#include <ResourceStats/ResourceStatsTypeIds.h>
#include <ResourceStatsModuleInterface.h>
#include "ResourceStatsSystemComponent.h"

namespace ResourceStats
{
    class ResourceStatsModule
        : public ResourceStatsModuleInterface
    {
    public:
        AZ_RTTI(ResourceStatsModule, ResourceStatsModuleTypeId, ResourceStatsModuleInterface);
        AZ_CLASS_ALLOCATOR(ResourceStatsModule, AZ::SystemAllocator);
    };
}// namespace ResourceStats

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME), ResourceStats::ResourceStatsModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_ResourceStats, ResourceStats::ResourceStatsModule)
#endif
