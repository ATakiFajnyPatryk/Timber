cmake_minimum_required(VERSION 3.10.2)
project(Timber)

set(CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

set(SFML_INCLUDE_DIR "/home/Libraries/SFML-2.5.1-Compiled/include")
set(SFML_LIBRARY_DIR "/home/Libraries/SFML-2.5.1-Compiled/lib")
set(SFML_DIR "/home/Libraries/SFML-2.5.1-Compiled/lib/cmake/SFML")

link_directories(SFML_LIBRARY_DIR)
include_directories(SFML_INCLUDE_DIR)

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)

if(SFML_FOUND)
    message(STATUS "SFML_INCLUDE_DIR: ${SFML_INCLUDE_DIR}")
    message(STATUS "SFML_LIBRARIES: ${SFML_LIBRARIES}")
    message(STATUS "SFML_VERSION: ${SFML_VERSION}")
endif()

file(COPY fonts DESTINATION ${CMAKE_BINARY_DIR})
file(COPY graphics DESTINATION ${CMAKE_BINARY_DIR})
file(COPY sound DESTINATION ${CMAKE_BINARY_DIR})

set(SOURCE_FILES Timber.cpp)

add_executable(Timber ${SOURCE_FILES})
target_link_libraries (Timber sfml-graphics sfml-window sfml-system sfml-audio)