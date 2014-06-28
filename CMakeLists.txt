cmake_minimum_required (VERSION 2.8)
project (RGPFolder)

# check compiler for c++11 capability and enable c++11 if required
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
add_library(rgpfolder SHARED
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Folder.cpp)

if(WIN32)
  include (GenerateExportHeader)
  GENERATE_EXPORT_HEADER(rgpfolder
                         BASE_NAME RGPFOLDER
                         EXPORT_MACRO_NAME RGPFOLDER_EXPORT
                         EXPORT_FILE_NAME RGPFolder_Export.h)

  set_target_properties(rgpfolder PROPERTIES DEFINE_SYMBOL "RGPFOLDER_EXPORTS")
endif()

# create example executable
add_executable(example
               ${CMAKE_CURRENT_SOURCE_DIR}/example/example.cpp)

# link library to example executable
target_link_libraries(example rgpfolder)

# set version info
set_target_properties(rgpfolder PROPERTIES
					  VERSION 0.1
					  SOVERSION 0)

# installation
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/rgp
        DESTINATION include)

install(TARGETS rgpfolder
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)
