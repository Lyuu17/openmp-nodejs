set(SOURCES
    "src/events/CheckpointEventsExtension.cpp"
    "src/events/ClassEventsExtension.cpp"
    "src/events/CustomModelEventsExtension.cpp"
    "src/events/DialogEventsExtension.cpp"
    "src/events/MenuEventsExtension.cpp"
    "src/events/ObjectEventsExtension.cpp"
    "src/events/PickupEventsExtension.cpp"
    "src/events/PlayerEventsExtension.cpp"
    "src/events/TextDrawEventsExtension.cpp"
    "src/events/VehicleEventsExtension.cpp"
    "src/classes/CheckpointExtension.cpp"
    "src/classes/ClassesExtension.cpp"
    "src/classes/MenuExtension.cpp"
    "src/classes/ObjectExtension.cpp"
    "src/classes/PawnExtension.cpp"
    "src/classes/PickupExtension.cpp"
    "src/classes/PlayerExtension.cpp"
    "src/classes/TextDrawExtension.cpp"
    "src/classes/VehicleExtension.cpp"
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
target_link_libraries(openmp-nodejs PRIVATE OMP-SDK node nlohmann_json::nlohmann_json pawn-runtime)
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
