add_library(libutility
    read_file.hpp read_file.cpp
)

target_link_libraries(libutility
    ${BOOST_LIBS}
)
