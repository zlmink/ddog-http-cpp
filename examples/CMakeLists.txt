cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(LDFLAGS,-L/usr/local/opt/openssl/lib)
set(CPPFLAGS,-I/usr/local/opt/openssl/include)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(EXAMPLES
    main)

# target_include_directories(
#     PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
#     PUBLIC ${CMAKE_SOURCE_DIR}/lib
# )
#message(STATUS "CMAKE dir: [${CMAKE_BINARY_DIR}]")
#link_directories(${CMAKE_BINARY_DIR}/lib)

#add_library(DDOG_LIB STATIC IMPORTED)
#set_property(TARGET DDOG_LIB PROPERTY IMPORTED_LOCATION ${CMAKE_BINARY_DIR}}/lib/libddog_http.a)

# find_library (
#     DDOG_LIB
#     NAMES libddog_http.a # what to look for
#     HINTS "${CMAKE_SOURCE_DIR}/lib" # where to look
#     NO_DEFAULT_PATH # do not search system default paths
# )

# check if we found the library
message(STATUS "DDOG_LIB: [${DDOG_LIB}]")

# if (NOT DDOG_LIB)
#     message(SEND_ERROR "Did not find lib ddog_http")
# endif

foreach (example ${EXAMPLES})
    add_executable(${example} ${example}.cpp)
    target_link_libraries(${example} ddog_http::ddog_http)
endforeach()

#install(TARGETS ddog-cpp DESTINATION bin)
add_custom_target(examples ALL DEPENDS ${EXAMPLES})


