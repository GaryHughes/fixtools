cmake_minimum_required(VERSION 3.17.0)

project(fixtools)

# Don't embed build tree paths
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "")

include_directories(
    "/usr/local/include"
    ${CMAKE_BINARY_DIR}
)

link_directories("/usr/local/lib")

if(APPLE)

    set(POCO_LIBS 
        PocoNet.dylib)

    set(FIX8_LIBS
        fix8.dylib
        ${POCO_LIBS})

else()

    set(POCO_LIBS
        PocoNet.so)

    set(FIX8_LIBS
        fix8.so
        ${POCO_LIBS})

endif()

add_subdirectory(schema)
add_subdirectory(fixcat)