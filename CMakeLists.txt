cmake_minimum_required(VERSION 3.2)
project(mdlutils_cpp)

find_package(Threads REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Google test
add_subdirectory(gtest)
include_directories(gtest/googletest-master/googletest/include) # apparently, the include directory has to be reset at this point

include_directories(include)

set(LIB_SOURCE_FILES
        src/libs/timeit.cpp
        src/libs/multithreading/thread_pool.cpp
        src/libs/multithreading/looper.cpp
        src/libs/multithreading/handler.cpp
)

set(TEST_SOURCE_FILES
        src/tests/main.cpp
)

set(GTEST_SOURCE_FILES
        src/gtests/exceptions-tests.cpp
        src/gtests/sorted_list-tests.cpp
        src/gtests/sequence_iterator-tests.cpp
        src/gtests/main-tests.cpp
        src/gtests/simple_accessor-tests.cpp
        src/gtests/getset_accessor-tests.cpp
        src/gtests/range-tests.cpp
        src/gtests/const_vector-tests.cpp
        src/gtests/looper-tests.cpp src/gtests/handler-tests.cpp)

add_library(mdlutils ${LIB_SOURCE_FILES})
target_link_libraries(mdlutils ${CMAKE_THREAD_LIBS_INIT})

add_executable(simple_tests ${TEST_SOURCE_FILES})
add_executable(google_tests ${GTEST_SOURCE_FILES})
target_link_libraries(simple_tests mdlutils)
target_link_libraries(google_tests mdlutils gtest)
