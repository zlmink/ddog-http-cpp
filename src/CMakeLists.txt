cmake_minimum_required(VERSION 3.15)

add_library(ddog_http SHARED
    client.cpp
    event.cpp
    metric.cpp
    monitor.cpp
    )

add_library(ddog_http::ddog_http ALIAS ddog_http)

#target_link_libraries(cpr PUBLIC CURL::libcurl) # todo should be private, but first dependencys in ssl_options need to be removed

# Set version for shared libraries.
set_target_properties(ddog_http
     PROPERTIES
     VERSION ${${PROJECT_NAME}_VERSION}
     SOVERSION ${${PROJECT_NAME}_VERSION_MAJOR})

install(TARGETS ddog_http)