// ============================================================================
// PHASE 1: Basic Testing and Single-threaded Crawling
// ============================================================================


#include <iostream>
#include <vector>
#include <string>
#include "crawler/urlparser.h"
#include "crawler/webcrawler.h"
#include "export/crawlexport.h"

void printUsage() {
    std::cout << "Usage: WebCrawler [options]\n"
              << "  -u, --url <url>          Starting URL to crawl\n"
              << "  -d, --depth <number>     Maximum crawl depth (default: 3)\n"
              << "  -p, --pages <number>     Maximum pages to crawl (default: 100)\n"
              << "  -t, --threads <number>   Number of threads (default: 4)\n"
              << "  -o, --output <file>      Output file (JSON format)\n"
              << "  --delay <ms>             Delay between requests in ms (default: 1000)\n"
              << "  --help                   Show this help message\n"
              << "\nIf parameters are not provided, you will be prompted to enter them.\n";
}

std::string promptForUrl() {
    std::string url;
    std::cout << "Enter the starting URL to crawl: ";
    std::getline(std::cin, url);

    // Trim whitespace
    url.erase(0, url.find_first_not_of(" \t\n\r"));
    url.erase(url.find_last_not_of(" \t\n\r") + 1);

    return url;
}

int promptForInt(const std::string& prompt, int defaultValue, int minValue = 1) {
    std::string input;
    std::cout << prompt << " (default: " << defaultValue << "): ";
    std::getline(std::cin, input);

    // Trim whitespace
    input.erase(0, input.find_first_not_of(" \t\n\r"));
    input.erase(input.find_last_not_of(" \t\n\r") + 1);

    if (input.empty()) {
        return defaultValue;
    }

    try {
        int value = std::stoi(input);
        if (value < minValue) {
            std::cout << "Value too small, using minimum: " << minValue << std::endl;
            return minValue;
        }
        return value;
    } catch (const std::exception&) {
        std::cout << "Invalid input, using default: " << defaultValue << std::endl;
        return defaultValue;
    }
}

std::string promptForOutput() {
    std::string output;
    std::cout << "Enter output filename (leave empty for default 'crawl_results.json'): ";
    std::getline(std::cin, output);

    // Trim whitespace
    output.erase(0, output.find_first_not_of(" \t\n\r"));
    output.erase(output.find_last_not_of(" \t\n\r") + 1);

    return output.empty() ? "crawl_results.json" : output;
}

int main(int argc, char* argv[]) {
    std::string startUrl;
    int maxDepth = -1;  // Use -1 to indicate not set
    int maxPages = -1;
    int numThreads = -1;
    int delay = -1;
    std::string outputFile;

    bool urlProvided = false;
    bool depthProvided = false;
    bool pagesProvided = false;
    bool threadsProvided = false;
    bool delayProvided = false;
    bool outputProvided = false;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-u" || arg == "--url") {
            if (i + 1 < argc) {
                startUrl = argv[++i];
                urlProvided = true;
            }
        } else if (arg == "-d" || arg == "--depth") {
            if (i + 1 < argc) {
                maxDepth = std::stoi(argv[++i]);
                depthProvided = true;
            }
        } else if (arg == "-p" || arg == "--pages") {
            if (i + 1 < argc) {
                maxPages = std::stoi(argv[++i]);
                pagesProvided = true;
            }
        } else if (arg == "-t" || arg == "--threads") {
            if (i + 1 < argc) {
                numThreads = std::stoi(argv[++i]);
                threadsProvided = true;
            }
        } else if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) {
                outputFile = argv[++i];
                outputProvided = true;
            }
        } else if (arg == "--delay") {
            if (i + 1 < argc) {
                delay = std::stoi(argv[++i]);
                delayProvided = true;
            }
        } else if (arg == "--help") {
            printUsage();
            return 0;
        }
    }

    std::cout << "=== Web Crawler Configuration ===\n";

    // Prompt for missing parameters
    if (!urlProvided) {
        startUrl = promptForUrl();
        if (startUrl.empty()) {
            std::cout << "Error: Starting URL is required\n";
            return 1;
        }
    }

    if (!depthProvided) {
        maxDepth = promptForInt("Enter maximum crawl depth", 3, 1);
    }

    if (!pagesProvided) {
        maxPages = promptForInt("Enter maximum pages to crawl", 100, 1);
    }

    if (!threadsProvided) {
        numThreads = promptForInt("Enter number of threads", 4, 1);
    }

    if (!delayProvided) {
        delay = promptForInt("Enter delay between requests (ms)", 1000, 0);
    }

    if (!outputProvided) {
        outputFile = promptForOutput();
    }

    // Validate the URL
    if (!UrlParser::isValidUrl(startUrl)) {
        std::cout << "Error: Invalid URL format: " << startUrl << std::endl;
        return 1;
    }

    try {
        CrawlerConfig config;
        config.seedUrls = {startUrl};
        config.maxPages = maxPages;
        config.maxDepth = maxDepth;
        config.delayBetweenRequests = std::chrono::milliseconds(delay);

        std::vector<WebCrawler::CrawlResult> results;

        WebCrawler crawler(config);
        crawler.setThreadCount(numThreads);

        crawler.setCrawlCallback([&results](const WebCrawler::CrawlResult& result) {
            std::cout << "Crawled: " << result.url
                      << " | Status: " << result.statusCode
                      << " | Links: " << result.extractedLinks.size() << std::endl;
            results.push_back(result);
        });

        std::cout << "\n=== Starting Crawl ===\n";
        std::cout << "URL: " << startUrl << std::endl;
        std::cout << "Max pages: " << maxPages << ", Max depth: " << maxDepth
                  << ", Threads: " << numThreads << ", Delay: " << delay << "ms" << std::endl;
        std::cout << "Output file: " << outputFile << std::endl;

        if (numThreads > 1) {
            crawler.startMultiThreaded();
        } else {
            crawler.start();
        }

        std::cout << "\n=== Crawl Summary ===\n";
        std::cout << "Total pages crawled: " << results.size() << std::endl;

        if (!results.empty()) {
            CrawlExport::exportToJSON(results, outputFile);
            std::cout << "Results exported to: " << outputFile << std::endl;

            // Also create CSV
            std::string csvFile = outputFile;
            if (csvFile.find(".json") != std::string::npos) {
                csvFile.replace(csvFile.find(".json"), 5, ".csv");
            } else {
                csvFile += ".csv";
            }
            CrawlExport::exportToCSV(results, csvFile);
            std::cout << "CSV export saved to: " << csvFile << std::endl;
        } else {
            std::cout << "No pages were successfully crawled." << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}