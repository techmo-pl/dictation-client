project(dictation_client)

add_executable(${PROJECT_NAME}
        main.cpp
        )

# Define headers for this library. PUBLIC headers are used for
# compiling the library, and will be added to consumers' build
# paths.
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_SOURCE_DIR}/libdictation-client
        )

# Depend on a library that we defined in the top-level file
target_link_libraries(${PROJECT_NAME}
        libdictation_client
        ssl_pkg
        boost_pkg
        dl_pkg
        )

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")


install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin)
