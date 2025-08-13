cmake_minimum_required(VERSION 4.0)
project(WebCrawler)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CURL REQUIRED libcurl)

add_executable(WebCrawler src/main.cpp
        src/crawler/urlparser.cpp
        src/crawler/urlparser.h
        src/crawler/httpclient.cpp
        src/crawler/httpclient.h
        src/crawler/config/crawlerconfig.h)

target_link_libraries(WebCrawler ${CURL_LIBRARIES})
target_include_directories(WebCrawler PRIVATE ${CURL_INCLUDE_DIRS})