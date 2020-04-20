add_executable(fixsed
    main.cpp
    options.hpp options.cpp
    pipeline.hpp pipeline.cpp
)

target_link_libraries(fixsed
    schema
    utility
    ${FIX8_LIBS}
    ${BOOST_LIBS}
)