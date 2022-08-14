cmake_minimum_required(VERSION 3.16)
project (cppli_tests)

cmake_minimum_required(VERSION 3.16)
if(POLICY CMP0092)
  cmake_policy(SET CMP0092 NEW)
endif()

if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
  find_package(Threads)
endif()

option(CPPLI_ENABLE_COVERAGE "Enables coverage reporting" OFF)

set(root_dir "${CMAKE_CURRENT_SOURCE_DIR}/..")
set(tests_dir "${root_dir}/tests")
set(include_dir "${root_dir}/include")
set(vendor_dir "${root_dir}/vendor")

file(GLOB_RECURSE cppli_header_file "${include_dir}/cppli/*.hpp")
file(GLOB_RECURSE source_files "${tests_dir}/*.cpp" "${tests_dir}/*.hpp")

if (NOT TARGET gtest)
	SET(gtest_force_shared_crt on CACHE BOOL
		"Use shared (DLL) run-time lib even when Google Test is built as static lib.")
	add_subdirectory(${vendor_dir}/googletest ${vendor_dir}/googletest EXCLUDE_FROM_ALL)
endif()

add_executable(cppli_tests "${source_files}" "${cppli_header_file}")
target_include_directories(cppli_tests PUBLIC "${include_dir}")
target_include_directories(cppli_tests PUBLIC "${vendor_dir}/googletest/googletest/include")

set_target_properties(cppli_tests
  PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  OUTPUT_NAME_DEBUG "cppli_tests_d"
  OUTPUT_NAME_RELEASE "cppli_tests"
  RUNTIME_OUTPUT_DIRECTORY "${root_dir}/bin"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${root_dir}/bin"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${root_dir}/bin"
)

# Code coverage
if(CPPLI_ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU") 
  set_target_properties(cppli_tests PROPERTIES COMPILE_FLAGS "--coverage")
  target_link_libraries(cppli_tests "gcov")
endif()

target_link_libraries(cppli_tests gtest_main)