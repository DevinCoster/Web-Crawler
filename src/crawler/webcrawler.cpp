//
// Created by docto on 8/16/2025.
//

#include "webcrawler.h"
#include <iostream>
#include <thread>

WebCrawler::WebCrawler(const CrawlerConfig& config)
    : config_(config), httpClient_(config.httpConfig) {

    // Extract allowed domains from seed URLs
    for (const auto& seedUrl : config.seedUrls) {
        auto parsed = UrlParser::parse(seedUrl);
        if (parsed.valid) {
            allowedDomains_.insert(parsed.host);
        }
    }
}

void WebCrawler::addSeedUrl(const std::string& url) {
    auto parsed = UrlParser::parse(url);
    if (parsed.valid) {
        urlQueue_.push(url);
        allowedDomains_.insert(parsed.host);
    }
}

void WebCrawler::setCrawlCallback(CrawlCallback callback) {
    crawlCallback_ = std::move(callback);
}

void WebCrawler::start() {
    running_ = true;

    // Add seed URLs to queue
    for (const auto& seedUrl : config_.seedUrls) {
        if (visitedUrls_.find(seedUrl) == visitedUrls_.end()) {
            urlQueue_.push(seedUrl);
        }
    }

    std::cout << "Starting crawl with " << urlQueue_.size() << " seed URLs\n";

    while (running_ && !urlQueue_.empty() && visitedUrls_.size() < config_.maxPages) {
        std::string currentUrl = urlQueue_.front();
        urlQueue_.pop();

        if (visitedUrls_.find(currentUrl) != visitedUrls_.end()) {
            continue;
        }

        processUrl(currentUrl);

        if (config_.delayBetweenRequests.count() > 0) {
            std::this_thread::sleep_for(config_.delayBetweenRequests);
        }
    }

    std::cout << "Crawl completed. Visited " << visitedUrls_.size() << " pages\n";
}

void WebCrawler::stop() {
    running_ = false;
}

void WebCrawler::processUrl(const std::string& url) {
    if (!shouldCrawlUrl(url)) {
        return;
    }

    visitedUrls_.insert(url);
    std::cout << "Crawling: " << url << std::endl;

    CrawlResult result;
    result.url = url;

    try {
        auto response = httpClient_.get(url);

        result.statusCode = response.statusCode;
        result.success = response.success;
        result.errorMessage = response.errorMessage.value_or("");

        if (response.success && response.statusCode == 200) {
            result.content = response.body;
            result.extractedLinks = extractLinks(response.body, url);

            // Add new URLs to queue
            for (const auto& link : result.extractedLinks) {
                if (shouldCrawlUrl(link) && visitedUrls_.find(link) == visitedUrls_.end()) {
                    urlQueue_.push(link);
                }
            }
        }

    } catch (const std::exception& e) {
        result.success = false;
        result.errorMessage = e.what();
    }

    if (crawlCallback_) {
        crawlCallback_(result);
    }
}

std::vector<std::string> WebCrawler::extractLinks(const std::string& html, const std::string& baseUrl) {
    std::vector<std::string> links;

    std::regex linkRegex(R"(href\s*=\s*["']([^"']+)["'])", std::regex_constants::icase);
    std::sregex_iterator iter(html.begin(), html.end(), linkRegex);
    std::sregex_iterator end;

    for (; iter != end; ++iter) {
        std::string href = (*iter)[1].str();

        if (href.rfind("http://", 0) == 0 || href.rfind("https://", 0) == 0) {
            links.push_back(href);
        } else if (!href.empty() && href[0] == '/') {
            auto baseParsed = UrlParser::parse(baseUrl);
            if (baseParsed.valid) {
                links.push_back(baseParsed.scheme + "://" + baseParsed.host + href);
            }
        }
    }

    return links;
}

bool WebCrawler::shouldCrawlUrl(const std::string& url) const {
    auto parsed = UrlParser::parse(url);

    if (!parsed.valid) return false;

    // Check domain restrictions
    if (config_.sameDomainOnly && allowedDomains_.find(parsed.host) == allowedDomains_.end()) {
        return false;
    }

    // Check file extensions
    for (const auto& ext : config_.skipExtensions) {
        const std::string& path = parsed.path;
        if (path.size() >= ext.size() &&
            path.compare(path.size() - ext.size(), ext.size(), ext) == 0) {
            return false;
        }
    }

    return true;
}

size_t WebCrawler::getQueueSize() const {
    return urlQueue_.size();
}

size_t WebCrawler::getVisitedCount() const {
    return visitedUrls_.size();
}