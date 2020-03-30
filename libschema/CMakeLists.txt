set(FIX8_COMPILER /usr/local/bin/f8c)

set(FIX8_SCHEMA_FIX44 ${CMAKE_SOURCE_DIR}/libschema/FIX44.xml)
set(FIX8_SCHEMA_FIX50SP2 ${CMAKE_SOURCE_DIR}/libschema/FIX50SP2.xml)
set(FIX8_SCHEMA_FIXT11 ${CMAKE_SOURCE_DIR}/libschema/FIXT11.xml)
   
set(FIX8_SCHEMA_FIX50SP2_SOURCES
    FIX50SP2_types.hpp 
    FIX50SP2_router.hpp 
    FIX50SP2_classes.hpp 
    FIX50SP2_types.cpp 
    FIX50SP2_traits.cpp
    FIX50SP2_classes.cpp
)

set(FIX8_SCHEMA_FIX44_SOURCES
    FIX44_types.hpp 
    FIX44_router.hpp 
    FIX44_classes.hpp 
    FIX44_types.cpp 
    FIX44_traits.cpp
    FIX44_classes.cpp
)

add_custom_command(
    OUTPUT ${FIX8_SCHEMA_FIX50SP2_SOURCES}
    COMMAND ${FIX8_COMPILER} --verbose --prefix FIX50SP2 --norealm --namespace FIX50SP2 --fixt ${FIX8_SCHEMA_FIXT11} ${FIX8_SCHEMA_FIX50SP2}
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)

add_custom_command(
    OUTPUT ${FIX8_SCHEMA_FIX44_SOURCES}
    COMMAND ${FIX8_COMPILER} --verbose --prefix FIX44 --norealm --namespace FIX44 ${FIX8_SCHEMA_FIX44}
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)

add_library(schema ${FIX8_SCHEMA_FIX50SP2_SOURCES} ${FIX8_SCHEMA_FIX44_SOURCES})
