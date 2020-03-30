add_executable(fixcat 
    main.cpp
    options.hpp options.cpp
)

target_link_libraries(fixcat 
    libschema
    libutility
    ${FIX8_LIBS}
    ${BOOST_LIBS}
)