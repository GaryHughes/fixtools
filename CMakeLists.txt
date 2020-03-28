cmake_minimum_required(VERSION 3.17.0)
project(fixtools)

# Don't embed build tree paths
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "")

add_subdirectory(fixcat)