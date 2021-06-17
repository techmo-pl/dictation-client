project(libdictation_client)

file(GLOB GOOGLEAPIS_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/../../submodules/googleapis/gens/google/api/annotations.pb.cc
        ${PROJECT_SOURCE_DIR}/../../submodules/googleapis/gens/google/api/http.pb.cc
        ${PROJECT_SOURCE_DIR}/../../submodules/googleapis/gens/google/longrunning/operations.pb.cc
        ${PROJECT_SOURCE_DIR}/../../submodules/googleapis/gens/google/rpc/status.pb.cc
        )

add_library(${PROJECT_NAME}
        ${GOOGLEAPIS_SOURCE_FILES}
        dictation_asr.grpc.pb.cc dictation_asr.grpc.pb.h
        dictation_asr.pb.cc dictation_asr.pb.h
        dictation_client.cpp dictation_client.h
        VERSION.h
        )

# Define headers for this library. PUBLIC headers are used for
# compiling the library, and will be added to consumers' build
# paths.
target_include_directories(${PROJECT_NAME} PUBLIC
        ${PROJECT_SOURCE_DIR}/../../submodules/googleapis/gens
        )

# Depend on a library that we defined in the top-level file
target_link_libraries(${PROJECT_NAME}
        grpc_pkg
        )

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
