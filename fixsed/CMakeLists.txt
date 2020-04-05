add_executable(fixsed
    main.cpp
    options.hpp options.cpp
)

target_link_libraries(fixsed
    schema
    utility
    ${FIX8_LIBS}
    ${BOOST_LIBS}
)