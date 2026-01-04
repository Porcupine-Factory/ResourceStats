
if(TARGET Gem::AMultiplayerPhysX5.Unified.Static)
add_compile_definitions(-DNETWORK_RESOURCE_STATS)
set(FILES
    Include/ResourceStats/ResourceStatsBus.h
    Include/ResourceStats/ResourceStatsTypeIds.h
    Include/ResourceStats/ResourceStatsComponentBus.h
    Include/ResourceStats/NetworkResourceStatsBus.h
    Include/ResourceStats/NetworkResourceStatsControllerBus.h
)
else()
set(FILES
    Include/ResourceStats/ResourceStatsBus.h
    Include/ResourceStats/ResourceStatsTypeIds.h
    Include/ResourceStats/ResourceStatsComponentBus.h
)
endif()
