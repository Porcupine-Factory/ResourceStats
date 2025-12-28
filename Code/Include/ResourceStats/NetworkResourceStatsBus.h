#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace ResourceStats
{
    class NetworkResourceStatsRequests
    {
    public:
        AZ_RTTI(NetworkResourceStatsRequests, "{AD09409B-490D-4754-8CC9-2324E069DCA8}");
        virtual ~NetworkResourceStatsRequests() = default;
        // Put your public methods here
    };

    class NetworkResourceStatsBusTraits : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using NetworkResourceStatsRequestBus = AZ::EBus<NetworkResourceStatsRequests, NetworkResourceStatsBusTraits>;
    using NetworkResourceStatsInterface = AZ::Interface<NetworkResourceStatsRequests>;

} // namespace ResourceStats
