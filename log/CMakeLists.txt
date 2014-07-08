cmake_minimum_required (VERSION 2.8)
project (RGPLog)

# check compiler for c++11 capability and activate c++11 if there are special flags required
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") # Clang
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "3.3")
    message(WARNING "Your compiler version may not support all used C++11 features!")
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") # GCC
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.8.1")
    message(WARNING "Your compiler version may not support all used C++11 features!")
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel") # Intel
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "14.0")
    message(WARNING "Your compiler version may not support all used C++11 features!")
  endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC") # MS Visual Compiler
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "18.0")
    message(WARNING "Your compiler version may not support all used C++11 features!")
  endif()
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/)

# create library
add_library(rgplog SHARED
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Log.cpp)

if(WIN32)
  include (GenerateExportHeader)
  GENERATE_EXPORT_HEADER(rgplog
                         BASE_NAME RGPLOG
                         EXPORT_MACRO_NAME RGPLOG_EXPORT
                         EXPORT_FILE_NAME RGPLog_Export.h)

  set_target_properties(rgplog PROPERTIES DEFINE_SYMBOL "RGPLOG_EXPORTS")
endif()

# create example executable
add_executable(example ${CMAKE_CURRENT_SOURCE_DIR}/example/example.cpp)

# link library to test executable
target_link_libraries(example rgplog)

# set version info
set_target_properties(rgplog PROPERTIES
                      VERSION 1.1
                      SOVERSION 0)

# installation 
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/rgp
        DESTINATION include)

install(TARGETS rgplog
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)
