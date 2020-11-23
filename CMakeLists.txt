cmake_minimum_required(VERSION 3.15)
project(ddog_http VERSION 0.1.0)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(LDFLAGS,-L/usr/local/opt/openssl/lib)
set(CPPFLAGS,-I/usr/local/opt/openssl/include)
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
include(CPack)
include(GNUInstallDirs)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake) # Include Conan-generated file
conan_basic_setup(TARGETS) # Introduce Conan-generated targets

add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(examples)

#add_executable(${PROJECT_NAME} main.cpp src/metric.cpp src/client.cpp src/monitor.cpp src/event.cpp)
target_link_libraries(${PROJECT_NAME} CONAN_PKG::cpr CONAN_PKG::rapidjson )
option(${BUILD_SHARED_LIBS} "Build libraries as shared libraries" ON)
#install(TARGETS ddog-cpp DESTINATION bin)


