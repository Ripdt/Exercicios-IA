file(GLOB SOURCES "*.cpp")

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(KnightTour ${SOURCES})
set_target_properties( KnightTour PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/ )