file(GLOB SOURCES "*.cpp")

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(QueenProblem ${SOURCES})
set_target_properties( QueenProblem PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/ )