// ============================================================================
// PHASE 1: Basic Testing and Single-threaded Crawling
// ============================================================================


#include <iostream>

#include "crawler/httpclient.h"
#include "crawler/urlparser.h"
#include "crawler/webcrawler.h"

int main() {
    try {
        // Test existing components
        std::cout << "Testing URL Parser..." << std::endl;
        if (const auto result = UrlParser::parse("https://www.wikipedia.org/"); result.valid) {
            std::cout << "Success! Host: " << result.host << std::endl;
        }

        std::cout << "\nTesting HTTP Client..." << std::endl;
        const HttpClient client;
        if (const auto response = client.get("https://httpbin.org/get"); response.isSuccess()) {
            std::cout << "HTTP Success! Status: " << response.statusCode << std::endl;
            std::cout << "Content length: " << response.body.length() << std::endl;
        }

        // Test web crawler
        std::cout << "\nTesting Web Crawler..." << std::endl;
        CrawlerConfig config;
        config.seedUrls = {"https://www.wikipedia.org/"};
        config.maxPages = 5;
        config.delayBetweenRequests = std::chrono::milliseconds(2000);

        WebCrawler crawler(config);
        crawler.setCrawlCallback([](const WebCrawler::CrawlResult& result) {
            std::cout << "Crawled: " << result.url
                      << " | Status: " << result.statusCode
                      << " | Links: " << result.extractedLinks.size() << std::endl;
        });

        crawler.start();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}