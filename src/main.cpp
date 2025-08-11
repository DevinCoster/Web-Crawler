// ============================================================================
// PHASE 1: Basic Testing and Single-threaded Crawling
// ============================================================================


#include <iostream>
#include "crawler/urlparser.h"

int main() {
    std::cout << "Testing URL Parser..." << std::endl;

    if (const auto result = UrlParser::parse("https://example.com/path"); result.valid) {
        std::cout << "Success! Host: " << result.host << std::endl;
    } else {
        std::cout << "Failed to parse URL" << std::endl;
    }

    return 0;
}