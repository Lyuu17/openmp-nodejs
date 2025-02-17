set(SOURCES
    "src/components/events/CheckpointEventsComponent.cpp"
    "src/components/events/ClassEventsComponent.cpp"
    "src/components/events/DialogEventsComponent.cpp"
    "src/components/events/MenuEventsComponent.cpp"
    "src/components/events/PickupEventsComponent.cpp"
    "src/components/events/PlayerEventsComponent.cpp"
    "src/components/events/VehicleEventsComponent.cpp"
    "src/components/CheckpointComponent.cpp"
    "src/components/ClassesComponent.cpp"
    "src/components/MenuComponent.cpp"
    "src/components/PickupComponent.cpp"
    "src/components/PlayerComponent.cpp"
    "src/components/VehicleComponent.cpp"
    "src/CommonFunctions.cpp"
    "src/main.cpp"
    "src/NodejsComponent.cpp"
    "src/Resource.cpp"
    "src/ResourceManager.cpp"
    "src/Utils.cpp"
    "src/Winfixes.cpp"
)

add_library(openmp-nodejs SHARED ${SOURCES})
target_include_directories(openmp-nodejs PRIVATE include)
target_link_libraries(openmp-nodejs PRIVATE OMP-SDK node nlohmann_json::nlohmann_json)
set_target_properties(openmp-nodejs PROPERTIES PREFIX "")

if ((MSVC) AND (MSVC_VERSION GREATER_EQUAL 1914))
    # https://gitlab.kitware.com/cmake/cmake/-/issues/18837
	target_compile_options(openmp-nodejs PUBLIC "/Zc:__cplusplus")
endif()

if (WIN32)
target_link_libraries(openmp-nodejs PRIVATE minhook)
endif()

install(TARGETS openmp-nodejs RUNTIME COMPONENT openmp-nodejs)
install(IMPORTED_RUNTIME_ARTIFACTS node RUNTIME COMPONENT openmp-nodejs)
