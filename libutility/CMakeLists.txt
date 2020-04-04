add_library(utility
    read_file.hpp read_file.cpp
    print_fix_message.hpp print_fix_message.cpp
)

target_link_libraries(utility
    ${FIX8_LIBS}
    ${BOOST_LIBS}
)
