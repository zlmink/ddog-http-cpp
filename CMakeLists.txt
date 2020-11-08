cmake_minimum_required(VERSION 3.3.0)
project(ddog-cpp VERSION 0.1.0)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(LDFLAGS,-L/usr/local/opt/openssl/lib)
set(CPPFLAGS,-I/usr/local/opt/openssl/include)
include(CPack)

set(CURL_LIBRARY "-lcurl")
find_package(CURL REQUIRED)

include_directories(${CURL_INCLUDE_DIR})


#FIND_PACKAGE( Boost 1.74)
#if (Boost_FOUND)
#    INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

    #-------------------------------------------------------------------------------
    #
    # OpenSSL
    #

    if (APPLE AND NOT DEFINED ENV{OPENSSL_ROOT_DIR})
    find_program(HOMEBREW brew)
    if (NOT HOMEBREW STREQUAL "HOMEBREW-NOTFOUND")
        execute_process(COMMAND brew --prefix openssl
        OUTPUT_VARIABLE OPENSSL_ROOT_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    endif()
    endif()

    find_package(OpenSSL)

    if (OPENSSL_FOUND)
        add_definitions (-DBOOST_BEAST_USE_OPENSSL=1)

    else()
        add_definitions (-DBOOST_BEAST_USE_OPENSSL=0)
        message("OpenSSL not found.")
    endif()

    #
    #-------------------------------------------------------------------------------
    find_package(RapidJSON)

    #include_directories(${Boost_INCLUDE_DIRS})
    add_executable(${PROJECT_NAME} main.cpp DataDogMetric.cpp)
    target_link_libraries(${PROJECT_NAME} OpenSSL::SSL ${CURL_LIBRARIES} ${RapidJSON_LIBRARIES})
    install(TARGETS ddog-cpp DESTINATION bin)
#endif()

