# We are inside: app/cmake/src_cmake
# Move two levels up → app
set(APP_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../..)

set(SRC_DIR ${APP_ROOT}/code/src)
set(INCLUDE_DIR ${APP_ROOT}/code/include)
set(JSON_DIR ${APP_ROOT}/external/libraries/JSON)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    ${SRC_DIR}/*.cpp
)

add_executable(SensorMonitoringHub ${SRC_FILES})
set_target_properties(SensorMonitoringHub PROPERTIES OUTPUT_NAME "SMH")
target_include_directories(SensorMonitoringHub PRIVATE
    ${INCLUDE_DIR}
    ${JSON_DIR}
)
find_package(Threads REQUIRED)
target_link_libraries(SensorMonitoringHub PRIVATE Threads::Threads)