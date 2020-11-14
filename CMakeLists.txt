cmake_minimum_required(VERSION 3.3.0)
project(ddog-cpp VERSION 0.1.0)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(LDFLAGS,-L/usr/local/opt/openssl/lib)
set(CPPFLAGS,-I/usr/local/opt/openssl/include)
include(CPack)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake) # Include Conan-generated file
conan_basic_setup(TARGETS) # Introduce Conan-generated targets

add_executable(${PROJECT_NAME} main.cpp DataDogMetric.cpp DataDogClient.cpp DataDogMonitor.cpp DataDogEvent.cpp)
target_link_libraries(${PROJECT_NAME} CONAN_PKG::cpr CONAN_PKG::rapidjson )
install(TARGETS ddog-cpp DESTINATION bin)


