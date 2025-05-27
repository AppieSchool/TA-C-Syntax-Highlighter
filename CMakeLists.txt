cmake_minimum_required(VERSION 3.5)

############################################################
# Set compiler flags
############################################################
# If supported by your compiler, you can add the -Wall, -Wextra, –fstack-protector-all and -g3 flags here.
set(OWN_GXX_FLAGS "-std=c++17")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${OWN_GXX_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OWN_GXX_FLAGS}")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${OWN_GXX_FLAGS}")
project(engine)

# Directive for detailed output of the project assembly
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)

# Set an external name for the executable
set(release_target "engine")
set(debug_target "all_tests")

# gtest
set(GTEST_LIB "gtest")
set(GTEST_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/${GTEST_LIB}/include")
set(GTEST_LIB_DIR "${CMAKE_SOURCE_DIR}/${GTEST_LIB}/lib")

# tests lib
file(GLOB DEBUG_TESTS_FILES "${CMAKE_SOURCE_DIR}/tests/cpp/*.cpp")
add_library(tests STATIC ${DEBUG_TESTS_FILES})
target_include_directories(tests PUBLIC "/tests/cpp/")

# Load all .cpp from classes
file(GLOB_RECURSE CLS_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
# list(REMOVE_ITEM CLS_SOURCES "${CMAKE_SOURCE_DIR}/src/Automaten/ENFA.cpp")
add_library(cls STATIC ${CLS_SOURCES})
target_include_directories(cls PUBLIC "/src")

# Recursively find all folders inside /src
file(GLOB_RECURSE RELEASE_CLS_INCLUDE_DIRS LIST_DIRECTORIES true "${CMAKE_SOURCE_DIR}/src/*")
foreach(dir ${RELEASE_CLS_INCLUDE_DIRS})
	if(IS_DIRECTORY ${dir})
		list(APPEND FILTERED_RELEASE_CLS_INCLUDE_DIRS ${dir})
	endif()
endforeach()

#connect all found folders to /src
target_include_directories(cls PUBLIC
		${FILTERED_RELEASE_CLS_INCLUDE_DIRS}
)
target_include_directories(tests PUBLIC
		${FILTERED_RELEASE_CLS_INCLUDE_DIRS}
		${GTEST_INCLUDE_DIR}
)

# set all additional files for loading in the project
set(EXTRA_SOURCES_RELEASE
		src/ini_configuration.cc
		engine.cc
)

# Connect libraries and customize execution for release_target
add_executable(${release_target} ${EXTRA_SOURCES_RELEASE})
# target_sources(${release_target} PRIVATE ${RELEASE_CPP_FILES})
# Adding a library with classes
target_link_libraries(${release_target} PRIVATE cls)
# indicate where to assemble the project
install( TARGETS ${release_target} DESTINATION ${PROJECT_SOURCE_DIR}/)


set(EXTRA_SOURCES_DEBUG
		src/ini_configuration.cc
)

# Connect libraries and customize execution for release_target
add_executable(${debug_target} ${EXTRA_SOURCES_DEBUG})
# target_sources(${release_target} PRIVATE ${RELEASE_CPP_FILES})
# Adding a library with classes
link_directories(${GTEST_LIB_DIR})
target_link_libraries(${debug_target} PRIVATE
		gtest
		gtest_main
		cls tests
)
# indicate where to assemble the project
install( TARGETS ${debug_target} DESTINATION ${PROJECT_SOURCE_DIR}/)