set(SOURCES
    "src/components/PlayerComponent.cpp"
    "src/components/VehicleComponent.cpp"
    "src/main.cpp"
    "src/NodejsComponent.cpp"
    "src/Resource.cpp"
    "src/ResourceManager.cpp"
    "src/Utils.cpp"
)

add_library(openmp-nodejs SHARED ${SOURCES})
target_include_directories(openmp-nodejs PRIVATE include)
target_link_libraries(openmp-nodejs PRIVATE OMP-SDK node nlohmann_json::nlohmann_json)
set_target_properties(openmp-nodejs PROPERTIES PREFIX "")

install(TARGETS openmp-nodejs RUNTIME COMPONENT openmp-nodejs)
install(IMPORTED_RUNTIME_ARTIFACTS node RUNTIME COMPONENT openmp-nodejs)
