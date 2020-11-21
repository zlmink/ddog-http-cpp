cmake_minimum_required(VERSION 3.15)

target_include_directories(ddog_http PUBLIC  
    $<INSTALL_INTERFACE:include>    
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

target_sources(ddog_http PRIVATE
     # Header files (useful in IDEs)
    ddog_http/client.hpp
    ddog_http/event.hpp
    ddog_http/metric.hpp
    ddog_http/monitor.hpp
)

install(DIRECTORY ddog_http DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})