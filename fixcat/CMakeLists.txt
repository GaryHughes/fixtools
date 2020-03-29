add_executable(fixcat main.cpp)

target_link_libraries(fixcat schema ${FIX8_LIBS})