if (NOT IS_DIRECTORY googletest-master/googletest)
    execute_process(COMMAND get_gtest.sh)
    execute_process(COMMAND echo Should have downloaded)
endif ()

set(gtest_build_samples, false)
set(gtest_build_tests, false)
add_subdirectory(googletest-master/googletest)

