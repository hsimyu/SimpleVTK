file(GLOB TEST_FIXTURE_SRC test_fixture.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(TEST_SRC "src")
subdirs(${TEST_SRC})