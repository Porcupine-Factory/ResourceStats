
#pragma once

#include <ResourceStats/ResourceStatsTypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace ResourceStats
{
    class ResourceStatsRequests
    {
    public:
        AZ_RTTI(ResourceStatsRequests, ResourceStatsRequestsTypeId);
        virtual ~ResourceStatsRequests() = default;
        // Put your public methods here
    };

    class ResourceStatsBusTraits : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using ResourceStatsRequestBus = AZ::EBus<ResourceStatsRequests, ResourceStatsBusTraits>;
    using ResourceStatsInterface = AZ::Interface<ResourceStatsRequests>;

} // namespace ResourceStats
