cmake_minimum_required(VERSION 3.16)
project (cppli)

include(../cmake/check_for_cpp_support.cmake)

check_for_cpp17_support(has_cpp17_support)
if(has_cpp17_support)
	add_subdirectory(readme_example_cpp17_1)
	add_subdirectory(readme_example_cpp17_2)
else()
	message(WARNING "Ignoring c++17 examples, due to missing c++17 compiler support.")
endif()

check_for_cpp20_support(has_cpp20_support)
if(has_cpp20_support)
	add_subdirectory(readme_example_cpp20)
else()
	message(WARNING "Ignoring c++20 examples, due to missing c++20 compiler support.")
endif()