//
// Created by docto on 8/11/2025.
//

#include "httpclient.h"
#include <iostream>
#include <curl/curl.h>
#include <sstream>
#include <utility>

// Default constructor
HttpClient::HttpClient() : config_{}, curlHandle_(nullptr)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curlHandle_ = curl_easy_init();
    if (!curlHandle_) {
        throw std::runtime_error("Failed to initialize CURL");
    }
}

// Custom constructor
HttpClient::HttpClient(HttpConfig  config) : config_(std::move(config)), curlHandle_(nullptr)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curlHandle_ = curl_easy_init();
    if (!curlHandle_) {
        throw std::runtime_error("Failed to initialize CURL");
    }
}

// Destructor
HttpClient::~HttpClient()
{
    if (curlHandle_) {
        curl_easy_cleanup(curlHandle_);
    }
    // curl_global_cleanup();  // Removed redundant cleanup
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
    }

    std::string responseBody;
    std::map<std::string, std::string> responseHeaders;

    curl_easy_reset(static_cast<CURL*>(curlHandle_));

    curl_easy_setopt(static_cast<CURL*>(curlHandle_), CURLOPT_URL, url.c_str());
    curl_easy_setopt(static_cast<CURL*>(curlHandle_), CURLOPT_USERAGENT, config_.userAgent.c_str());
    curl_easy_setopt(static_cast<CURL*>(curlHandle_), CURLOPT_TIMEOUT, static_cast<long>(config_.timeout.count()));
    curl_easy_setopt(static_cast<CURL*>(curlHandle_), CURLOPT_FOLLOWLOCATION, config_.followRedirects ? 1L : 0L);
    curl_easy_setopt(static_cast<CURL*>(curlHandle_), CURLOPT_MAXREDIRS, static_cast<long>(config_.maxRedirects));
    curl_easy_setopt(static_cast<CURL*>(curlHandle_), CURLOPT_MAXFILESIZE, static_cast<long>(config_.maxContentLength));

    if (method == "HEAD")
    {
        curl_easy_setopt(static_cast<CURL*>(curlHandle_), CURLOPT_NOBODY, 1L);
    }

    curl_easy_setopt(static_cast<CURL*>(curlHandle_), CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(static_cast<CURL*>(curlHandle_), CURLOPT_WRITEDATA, &responseBody);
    curl_easy_setopt(static_cast<CURL*>(curlHandle_), CURLOPT_HEADERFUNCTION, headerCallback);
    curl_easy_setopt(static_cast<CURL*>(curlHandle_), CURLOPT_HEADERDATA, &responseHeaders);

    CURLcode res = curl_easy_perform(static_cast<CURL*>(curlHandle_));

    if (res != CURLE_OK)
    {
        response.success = false;
        response.errorMessage = curl_easy_strerror(res);
        return response;
    }

    long responseCode;
    curl_easy_getinfo(static_cast<CURL*>(curlHandle_), CURLINFO_RESPONSE_CODE, &responseCode);

    response.statusCode = static_cast<int>(responseCode);
    response.body = std::move(responseBody);
    response.headers = std::move(responseHeaders);
    response.success = true;

    return response;
}

// Callback Functions
size_t HttpClient::writeCallback(void* contents, size_t size, size_t nmemb, std::string* response)
{
    size_t totalSize = size * nmemb;
    response->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

size_t HttpClient::headerCallback(void* contents, size_t size, size_t nmemb, std::map<std::string, std::string>* headers)
{
    size_t totalSize = size * nmemb;
    std::string header(static_cast<char*>(contents), totalSize);

    size_t colonPos = header.find(':');
    if (colonPos != std::string::npos && colonPos > 0)
    {
        std::string name = header.substr(0, colonPos);
        std::string value = header.substr(colonPos + 1);

        name.erase(0, name.find_first_not_of(" \t"));
        name.erase(name.find_last_not_of(" \t\r\n") + 1);  // Added \r\n
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t\r\n") + 1);  // Added \r\n

        if (!name.empty() && !value.empty())
        {
            (*headers)[name] = value;
        }
    }

    return totalSize;
}