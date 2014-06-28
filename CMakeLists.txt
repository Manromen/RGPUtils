cmake_minimum_required (VERSION 2.8)
project (RGPConfig)

# check compiler for c++11 capability
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") # Clang
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "3.3")
    message(WARNING "Your compiler version may not support all used C++11 features!")
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -stdlib=libc++")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") # GCC
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.8.1")
    message(WARNING "Your compiler version may not support all used C++11 features!")
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel") # Intel
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "14.0")
    message(WARNING "Your compiler version may not support all used C++11 features!")
  endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC") # MS Visual Compiler
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "18.0")
    message(WARNING "Your compiler version may not support all used C++11 features!")
  endif()
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# create library
add_library(rgpconfig SHARED
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Config.cpp)

if(WIN32)
  include (GenerateExportHeader)
  GENERATE_EXPORT_HEADER(rgpconfig
                         BASE_NAME RGPCONFIG
                         EXPORT_MACRO_NAME RGPCONFIG_EXPORT
                         EXPORT_FILE_NAME RGPConfig_Export.h)

  set_target_properties(rgpconfig PROPERTIES DEFINE_SYMBOL "RGPCONFIG_EXPORTS")
endif()

# create example executable
add_executable(example
               ${CMAKE_CURRENT_SOURCE_DIR}/example/example.cpp)
# copy example.conf to build folder
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/example/example.conf DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

# link library to example executable
target_link_libraries(example rgpconfig)

# set version info
set_target_properties(rgpconfig PROPERTIES
					  VERSION 0.1
					  SOVERSION 0)

# installation 
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/rgp
        DESTINATION include)

install(TARGETS rgpconfig
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)
