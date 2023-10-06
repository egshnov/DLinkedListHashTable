project(test)
add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Google_Tests_run DLinkedListTests.cpp)

target_link_libraries(Google_Tests_run HashTable_lib)
target_link_libraries(Google_Tests_run gtest gtest_main)