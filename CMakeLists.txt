cmake_minimum_required(VERSION 3.3.0)
project(ddog-cpp VERSION 0.1.0)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(LDFLAGS,-L/usr/local/opt/openssl/lib)
set(CPPFLAGS,-I/usr/local/opt/openssl/include)
include(CPack)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/whoshuu/cpr.git GIT_TAG c8d33915dbd88ad6c92b258869b03aba06587ff9) # the commit hash for 1.5.0
FetchContent_MakeAvailable(cpr)

set(-DCMAKE_USE_OPENSSL,ON)

# #----------------------
# # Try to find the header
# FIND_PATH(LIBUV_INCLUDE_DIR NAMES uv.h)

# # Try to find the library
# FIND_LIBRARY(LIBUV_LIBRARY NAMES uv libuv)

# # Handle the QUIETLY/REQUIRED arguments, set LIBUV_FOUND if all variables are
# # found
# INCLUDE(FindPackageHandleStandardArgs)
# FIND_PACKAGE_HANDLE_STANDARD_ARGS(LIBUV
#                                   REQUIRED_VARS
#                                   LIBUV_LIBRARY
#                                   LIBUV_INCLUDE_DIR)

# # Hide internal variables
# MARK_AS_ADVANCED(LIBUV_INCLUDE_DIR LIBUV_LIBRARY)

# Set standard variables
# IF(LIBUV_FOUND)
#     SET(LIBUV_INCLUDE_DIRS "${LIBUV_INCLUDE_DIR}")
#     SET(LIBUV_LIBRARIES "${LIBUV_LIBRARY}")
# ENDIF()
# #---------------------


if (APPLE AND NOT DEFINED ENV{OPENSSL_ROOT_DIR})
find_program(HOMEBREW brew)
if (NOT HOMEBREW STREQUAL "HOMEBREW-NOTFOUND")
    execute_process(COMMAND brew --prefix openssl
    OUTPUT_VARIABLE OPENSSL_ROOT_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE)
else()
    message("Homebrew openssl not found")
endif()
endif()

find_package(OpenSSL)

if (OPENSSL_FOUND)
    add_definitions (-DBOOST_BEAST_USE_OPENSSL=1)
    message("OpenSSL found")
else()
    add_definitions (-DBOOST_BEAST_USE_OPENSSL=0)
    message("OpenSSL not found.")
endif()


set(CURL_LIBRARY "-lcurl")
find_package(CURL REQUIRED)

include_directories(${CURL_INCLUDE_DIRS})

#FIND_PACKAGE( Boost 1.74)
#if (Boost_FOUND)
#    INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

    #-------------------------------------------------------------------------------
    #
    # OpenSSL
    #

    #
    #-------------------------------------------------------------------------------
    find_package(RapidJSON)

    #include_directories(${Boost_INCLUDE_DIRS})
    add_executable(${PROJECT_NAME} main.cpp DataDogMetric.cpp DataDogClient.cpp DataDogMonitor.cpp)
    target_link_libraries(${PROJECT_NAME} ${RapidJSON_LIBRARIES} OpenSSL::SSL ${CURL_LIBRARY} cpr::cpr )
    install(TARGETS ddog-cpp DESTINATION bin)
#endif()

