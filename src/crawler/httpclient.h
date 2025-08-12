//
// Created by docto on 8/11/2025.
//

#pragma once
#include <string>
#include <map>
#include <chrono>
#include <optional>

class HttpClient {
public:
    struct HttpResponse {
        int statusCode = 0;
        std::string body;
        std::map<std::string, std::string> headers;
        std::chrono::system_clock::time_point timestamp;
        bool success = false;
        std::optional<std::string> errorMessage;

        [[nodiscard]] bool isSuccess() const { return success && statusCode >= 200 && statusCode < 300; }
        [[nodiscard]] bool isRedirect() const { return statusCode >= 300 && statusCode < 400; }
    };

    struct HttpConfig {
        std::chrono::seconds timeout = std::chrono::seconds(30);
        std::string userAgent = "WebCrawler/1.0";
        int maxRedirects = 5;
        bool followRedirects = true;
        size_t maxContentLength = 10 * 1024 * 1024;
    };

    HttpClient();  // Default constructor
    explicit HttpClient(const HttpConfig& config);  // Constructor with config
    ~HttpClient();

    [[nodiscard]] HttpResponse get(const std::string& url) const;
    [[nodiscard]] HttpResponse head(const std::string& url) const;

private:
    HttpConfig config_;
    void* curlHandle_ = nullptr;

    HttpResponse performRequest(const std::string& url, const std::string& method) const;
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* response);
    static size_t headerCallback(void* contents, size_t size, size_t nmemb, std::map<std::string, std::string>* headers);
};

//Lesson learned, Capitlization is important in C++, fuck this language. Also employeers if you are seeing this, this is problem solving!