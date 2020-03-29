add_executable(fixcat 
    main.cpp
    options.hpp options.cpp
)

target_link_libraries(fixcat 
    schema 
    ${FIX8_LIBS}
    ${BOOST_LIBS}
)