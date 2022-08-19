# Set default compiler options.
function(set_default_compile_options target)
  if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")  
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      target_compile_options(${target} PRIVATE -Wmove)
    endif()   
    target_compile_options(${target} PRIVATE -Werror -Wall -Wextra -Wno-long-long)    
    if("${CMAKE_BUILD_TYPE}" MATCHES Debug)
      target_compile_options(${target} PRIVATE -g)
    else()
      target_compile_options(${target} PRIVATE -O3)
    endif()

  elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(${target} PRIVATE /W4 /WX /w44062)
  endif()

  get_target_property(compilerFlags ${target} COMPILE_OPTIONS)

  message("Setting \"target_compile_options\": \"${compilerFlags}\" for ${CMAKE_CXX_COMPILER_ID}, target ${target}.")

endfunction(set_default_compile_options)