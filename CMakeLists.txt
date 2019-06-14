cmake_minimum_required(VERSION 3.5)
project(examples)

if(DEFINED ENV{CMAKE_INSTALL_PREFIX})
else()
    set(CMAKE_INSTALL_PREFIX $ENV{HOME}/.cmake_install)
endif()

include(cmake/GitHooks.cmake)
include(cmake/Utilities.cmake)
include(cmake/GeneralConfig.cmake)

# list(APPEND CMAKE_PREFIX_PATH $ENV{CMAKE_INSTALL_PREFIX} $ENV{HOME}/.cmake_install)
message(STATUS "cmake prefix path: ${CMAKE_PREFIX_PATH}")

include_directories(BEFORE
    ${PROJECT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/include
    ${CMAKE_INSTALL_PREFIX}/include/cubao_bundles
    # ~/.cmake_install/include/cubao_bundles
)
print_include_directories()

link_directories(${CMAKE_INSTALL_PREFIX}/lib)

configure_file(
    ${PROJECT_SOURCE_DIR}/version.h.in
    ${PROJECT_BINARY_DIR}/version.h
)

include(cmake/SetEnv.cmake)
set(HEADER_ROOT "${PROJECT_SOURCE_DIR}/include")
file(GLOB_RECURSE HEADERS include/*)
file(GLOB_RECURSE SOURCES src/*.cpp)
include(cmake/LibraryConfig.cmake)
target_link_libraries(${PROJECT_NAME}
    cubao_bundles
    h3
    s2
)
print_all_linked_libraries(${PROJECT_NAME})

file(GLOB SRCS examples/example*.cpp)
foreach(src ${SRCS})
    string(REGEX REPLACE "(^.*/|.cpp$)" "" exe ${src})
    add_executable(${exe} ${src})
    target_link_libraries(${exe} ${PROJECT_NAME})
endforeach(src)

include(cmake/InstallConfig.cmake)

