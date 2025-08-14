cmake_minimum_required(VERSION 3.10)
project(WebCrawler)

set(CMAKE_CXX_STANDARD 17)

# Set vcpkg toolchain if not already set
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "Vcpkg toolchain file")
endif()

find_package(CURL REQUIRED)

add_executable(WebCrawler src/main.cpp
        src/crawler/urlparser.cpp
        src/crawler/urlparser.h
        src/crawler/httpclient.cpp
        src/crawler/httpclient.h
        src/crawler/config/crawlerconfig.h)

target_link_libraries(WebCrawler CURL::libcurl)