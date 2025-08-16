//
// Created by docto on 8/11/2025.
//

#pragma once
#include <string>
#include <vector>
#include <chrono>
#include "../httpclient.h"

struct CrawlerConfig
{
    // URL's to start crawling from
    std::vector<std::string> seedUrls;

    // Max pages to crawl
    size_t maxPages = 100;

    // Max depth to crawl
    size_t maxDepth = 4;

    // Delay between requests
    std::chrono::milliseconds delayBetweenRequests{1000};

    // HTTP client
    HttpClient::HttpConfig httpConfig;

    // File extensions to skip
    std::vector<std::string> skipExtensions = {
        ".jpg", ".jpeg", ".png", ".gif", ".bmp", ".svg",
        ".mp4", ".avi", ".mov", ".wmv", ".flv",
        ".mp3", ".wav", ".ogg", ".flac",
        ".pdf", ".docx", ".xlsx", ".pptx",
        ".zip", ".rar", ".tar.gz"
    };

    bool respectRobotsTxt = true;

    // Only do URL's from same domain

    bool sameDomainOnly = true;
};