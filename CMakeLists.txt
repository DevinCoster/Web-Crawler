cmake_minimum_required(VERSION 3.10)
project(WebCrawler)

set(CMAKE_CXX_STANDARD 20)  # Need C++20 for string methods, ranges, etc.

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "Vcpkg toolchain file")
endif()

find_package(CURL REQUIRED)

add_executable(WebCrawler
        src/main.cpp
        src/crawler/urlparser.cpp
        src/crawler/urlparser.h
        src/crawler/httpclient.cpp
        src/crawler/httpclient.h
        src/crawler/webcrawler.cpp
        src/crawler/webcrawler.h
        src/crawler/robotparser.cpp
        src/crawler/robotparser.h
        src/content/contentprocessor.cpp
        src/content/contentprocessor.h
        src/export/crawlexport.cpp
        src/export/crawlexport.h
        src/crawler/config/crawlerconfig.h
)

target_link_libraries(WebCrawler CURL::libcurl)