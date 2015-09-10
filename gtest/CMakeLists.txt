if (NOT IS_DIRECTORY googletest-master/googletest)
	if (CMAKE_SYSTEM_NAME EQUAL "Windows")
		execute_process(COMMAND get_gtest.ps1)
		execute_process(COMMAND echo Should have downloaded)
	else()
		execute_process(COMMAND get_gtest.sh)
		execute_process(COMMAND echo Should have downloaded)
	endif()
endif ()

set(gtest_build_samples, false)
set(gtest_build_tests, false)
add_subdirectory(googletest-master/googletest)

