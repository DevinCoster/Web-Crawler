//
// Created by docto on 8/16/2025.
//

#pragma once

#include "httpclient.h"
#include "urlparser.h"
#include "config/crawlerconfig.h"
#include <queue>
#include <unordered_set>
#include <vector>
#include <functional>
#include <regex>

class WebCrawler {
public:
    struct CrawlResult {
        std::string url;
        int statusCode = 0;
        std::string content;
        std::vector<std::string> extractedLinks;
        bool success = false;
        std::string errorMessage;
    };

    using CrawlCallback = std::function<void(const CrawlResult&)>;

    explicit WebCrawler(const CrawlerConfig& config);

    void addSeedUrl(const std::string& url);
    void setCrawlCallback(CrawlCallback callback);
    void start();
    void stop();

    size_t getQueueSize() const;
    size_t getVisitedCount() const;

private:
    CrawlerConfig config_;
    HttpClient httpClient_;

    std::queue<std::string> urlQueue_;
    std::unordered_set<std::string> visitedUrls_;
    std::unordered_set<std::string> allowedDomains_;

    CrawlCallback crawlCallback_;
    bool running_ = false;

    static std::vector<std::string> extractLinks(const std::string& html, const std::string& baseUrl);
    bool shouldCrawlUrl(const std::string& url) const;
    void processUrl(const std::string& url);
};