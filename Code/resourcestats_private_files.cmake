# Determine if the Multiplayer gem is enabled
set(MULTIPLAYER_GEM_ENABLED FALSE)

# Check project.json for the Multiplayer gem
if(NOT MULTIPLAYER_GEM_ENABLED AND EXISTS "${CMAKE_SOURCE_DIR}/project.json")
    file(READ "${CMAKE_SOURCE_DIR}/project.json" _project_json_content)
    if(_project_json_content MATCHES "\"Multiplayer\"" OR
       _project_json_content MATCHES "\"Multiplayer==" OR
       _project_json_content MATCHES "\"Multiplayer>=" OR
       _project_json_content MATCHES "\"Multiplayer~=" OR
       _project_json_content MATCHES "\"Multiplayer>" OR
       _project_json_content MATCHES "\"Multiplayer<")
        set(MULTIPLAYER_GEM_ENABLED TRUE)
    endif()
endif()

# Fallback to checking enabled_gems.cmake
if(NOT MULTIPLAYER_GEM_ENABLED AND EXISTS "${CMAKE_SOURCE_DIR}/Code/enabled_gems.cmake")
    file(READ "${CMAKE_SOURCE_DIR}/Code/enabled_gems.cmake" _enabled_gems_content)
    if(_enabled_gems_content MATCHES "\"Multiplayer\"" OR _enabled_gems_content MATCHES "[ \t\r\n]Multiplayer[ \t\r\n]")
        set(MULTIPLAYER_GEM_ENABLED TRUE)
    endif()
endif()

# When building the engine workspace directly, standard gems are included
if(NOT MULTIPLAYER_GEM_ENABLED AND EXISTS "${CMAKE_SOURCE_DIR}/engine.json")
    if(EXISTS "${CMAKE_SOURCE_DIR}/Gems/Multiplayer")
        set(MULTIPLAYER_GEM_ENABLED TRUE)
    endif()
endif()

if(MULTIPLAYER_GEM_ENABLED)
set(FILES
    Source/ResourceStatsModuleInterface.cpp
    Source/ResourceStatsModuleInterface.h
    Source/Clients/ResourceStatsSystemComponent.cpp
    Source/Clients/ResourceStatsSystemComponent.h
    Source/Clients/ResourceStatsComponent.cpp
    Source/Clients/ResourceStatsComponent.h
    Source/Multiplayer/NetworkResourceStats.cpp
    Source/Multiplayer/NetworkResourceStats.h

    Source/AutoGen/NetworkResourceStats.AutoComponent.xml
)
else()
set(FILES
    Source/ResourceStatsModuleInterface.cpp
    Source/ResourceStatsModuleInterface.h
    Source/Clients/ResourceStatsSystemComponent.cpp
    Source/Clients/ResourceStatsSystemComponent.h
    Source/Clients/ResourceStatsComponent.cpp
    Source/Clients/ResourceStatsComponent.h
)
endif()
