//
// Created by docto on 8/11/2025.
//

#include "httpclient.h"
#include <iostream>
#include <curl/curl.h>
#include <sstream>

// Default constructor
HttpClient::HttpClient() : config_{}, curlHandle_(nullptr)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curlHandle_ = curl_easy_init();
    if (!curlHandle_) {
        throw std::runtime_error("Failed to initializ CURL");
    }
}

// Custom constructor
HttpClient::HttpClient(const HttpConfig& config) : config_(config), curlHandle_(nullptr)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curlHandle_ = curl_easy_init();
    if (!curlHandle_) {
        throw std::runtime_error("Failed to initializ CURL");
    }
}

// Destructor
HttpClient::~HttpClient()
{
    if (curlHandle_) {
        curl_easy_cleanup(curlHandle_);
    }
    curl_global_cleanup();
}

// The GET Request
HttpClient::HttpResponse HttpClient::get(const std::string& url) const
{
    return performRequest(url, "GET");
}
// The HEAD Request
HttpClient::HttpResponse HttpClient::head(const std::string& url) const
{
    return performRequest(url, "HEAD");
}

// The HTTP request
HttpClient::HttpResponse HttpClient::performRequest(const std::string& url, const std::string& method) const
{
    HttpResponse response;
    response.timestamp = std::chrono::system_clock::now();

    if (!curlHandle_)
    {
        response.success = false;
        response.errorMessage = "CURL handle is not initialized";
        return response;
        //Left off here. Need to do curl handles, methods, options, callbacks, and more requests. But good work so far!
    }}