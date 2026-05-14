cmake_minimum_required(VERSION 3.28)
project(Annoyed_Flocks)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip 
)

FetchContent_Declare(
  box2d
  GIT_REPOSITORY https://github.com/erincatto/box2d.git
  GIT_TAG v2.4.1
)

FetchContent_Declare(
  sfml
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 2.6.x 
)

FetchContent_MakeAvailable(googletest box2d sfml) 

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} 
    box2d 
    sfml-graphics 
    sfml-window 
    sfml-system 
    sfml-audio 
    sfml-network
)

set(TEST_SOURCES ${SOURCES})
list(REMOVE_ITEM TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

add_executable(RunUnitTests ${TEST_SOURCES} ${HEADERS} "test_src/RunTests.cpp")
target_include_directories(RunUnitTests PUBLIC include)
target_link_libraries(RunUnitTests gtest gtest_main box2d sfml-graphics sfml-system) 

enable_testing()
add_test(NAME RunUnitTests COMMAND RunUnitTests)
