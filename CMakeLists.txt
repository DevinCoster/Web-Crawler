cmake_minimum_required(VERSION 4.0)
project(WebCrawler)

set(CMAKE_CXX_STANDARD 17)

add_executable(WebCrawler src/main.cpp
        src/crawler/urlparser.cpp
        src/crawler/urlparser.h
        src/crawler/httpclient.cpp
        src/crawler/httpclient.h
        src/crawler/config/crawlerconfig.h)
