cmake_minimum_required(VERSION 4.0)
project(prod)

set(CMAKE_CXX_STANDARD 20)


find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Eigen3 REQUIRED NO_MODULE)
add_executable(prod main.cpp
        keys.h
        realtime.h)


target_link_libraries(prod PRIVATE
        OpenSSL::SSL
        OpenSSL::Crypto
        nlohmann_json::nlohmann_json
        CURL::libcurl
        Eigen3::Eigen
)