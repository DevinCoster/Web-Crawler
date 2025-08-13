// ============================================================================
// PHASE 1: Basic Testing and Single-threaded Crawling
// ============================================================================


#include <iostream>

#include "crawler/httpclient.h"
#include "crawler/urlparser.h"

int main() {
    std::cout << "Testing URL Parser..." << std::endl;

    if (const auto result = UrlParser::parse("https://example.com/path"); result.valid) {
        std::cout << "Success! Host: " << result.host << std::endl;
    }

    std::cout << "\nTesting HTTP Client..." << std::endl;

    const HttpClient client;

    if (const auto response = client.get("https://httpbin.org/get"); response.isSuccess()) {
        std::cout << "HTTP Success! Status: " << response.statusCode << std::endl;
        std::cout << "Content length: " << response.body.length() << std::endl;
    } else {
        std::cout << "HTTP Failed: " << response.errorMessage.value_or("Unknown error") << std::endl;
    }

    return 0;
};