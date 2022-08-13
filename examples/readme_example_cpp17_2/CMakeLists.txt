cmake_minimum_required(VERSION 3.16)
project (readme_example_cpp17_2)

cmake_minimum_required(VERSION 3.16)
if(POLICY CMP0092)
  cmake_policy(SET CMP0092 NEW)
endif()

project (MoltenEditor)

if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
  find_package(Threads)
endif()

set(root_dir "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(example_dir "${root_dir}/examples")
set(this_example_dir "${example_dir}/readme_example_cpp17_2")
set(include_dir "${root_dir}/include")

file(GLOB_RECURSE cppli_header_file "${include_dir}/cppli/*.hpp")
file(GLOB_RECURSE source_file "${this_example_dir}/*.cpp")

add_executable(readme_example_cpp17_2 "${source_file}" "${cppli_header_file}")
target_include_directories(readme_example_cpp17_2 PUBLIC "${include_dir}")

set_target_properties(readme_example_cpp17_2
  PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  OUTPUT_NAME_DEBUG "readme_example_cpp17_2_d"
  OUTPUT_NAME_RELEASE "readme_example_cpp17_2"
  RUNTIME_OUTPUT_DIRECTORY "${root_dir}/bin"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${root_dir}/bin"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${root_dir}/bin"
)
