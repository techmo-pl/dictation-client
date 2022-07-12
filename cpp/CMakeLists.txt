# Main build system for Dictation ASR gRPC client.

cmake_minimum_required(VERSION 3.5)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

project(dictation-client VERSION 2.4.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)



# ********************************************* #
# *************** Dependencies **************** #
# ********************************************* #

# *************** DL **************** #
add_library(dl_pkg INTERFACE IMPORTED)
set_property(TARGET dl_pkg
        PROPERTY INTERFACE_LINK_LIBRARIES ${CMAKE_DL_LIBS})

# *************** OpenSSL **************** #
set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)
add_library(ssl_pkg INTERFACE IMPORTED)
set_property(TARGET ssl_pkg
        PROPERTY INTERFACE_LINK_LIBRARIES ${OPENSSL_LIBRARIES})

# *************** GRPC **************** #
set(Grpc_ROOT /opt/grpc_v1.38.1)
find_package(Grpc REQUIRED)
add_library(grpc_pkg INTERFACE IMPORTED)
set_property(TARGET grpc_pkg
        PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Grpc_INCLUDE_DIRS})
set_property(TARGET grpc_pkg
        PROPERTY INTERFACE_LINK_LIBRARIES ${Grpc_LIBRARIES})

# *************** Boost **************** #
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
set(BOOST_ROOT /opt/boost_1_74_0)
find_package(Boost 1.74.0 REQUIRED system filesystem log locale program_options regex thread wserialization serialization)
add_library(boost_pkg INTERFACE IMPORTED)
set_property(TARGET boost_pkg
        PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})
set_property(TARGET boost_pkg
        PROPERTY INTERFACE_LINK_LIBRARIES ${Boost_LIBRARIES})



# ********************************************* #
# ************** Compiler Flags *************** #
# ********************************************* #

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Wno-sign-compare -Wno-unused-local-typedefs -Wno-unused-variable -Wno-deprecated-declarations")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")
set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -march=native")


message("CMAKE_CXX_FLAGS_DEBUG is ${CMAKE_CXX_FLAGS_DEBUG}")
message("CMAKE_CXX_FLAGS_RELEASE is ${CMAKE_CXX_FLAGS_RELEASE}")
message("CMAKE_CXX_FLAGS_RELWITHDEBINFO is ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
message("CMAKE_CXX_FLAGS_MINSIZEREL is ${CMAKE_CXX_FLAGS_MINSIZEREL}")
message("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
            "MinSizeRel" "RelWithDebInfo")
endif()



# ********************************************* #
# ***************** Projects ****************** #
# ********************************************* #
add_subdirectory(libdictation-client)
add_subdirectory(dictation-client)
