//
// Created by docto on 8/16/2025.
//

#include "crawlexport.h"
#include <fstream>
#include <iostream>

void CrawlExport::exportToJSON(const std::vector<WebCrawler::CrawlResult>& results,
                                const std::string& filename) {

    std::ofstream file(filename); file << "{\n  \"crawl_results\": [\n";

    for (size_t i = 0; i < results.size(); ++i) {
        const auto& result = results[i];
        file << "    {\n";
        file << "      \"url\": \"" << result.url << "\",\n";
        file << "      \"status_code\": " << result.statusCode << ",\n";
        file << "      \"success\": " << (result.success ? "true" : "false") << ",\n";
        file << "      \"links_found\": " << result.extractedLinks.size() << "\n";
        file << "    }" << (i < results.size() - 1 ? "," : "") << "\n";
    }
    file << "  ]\n}\n";
    file.close();
}
void CrawlExport::exportToCSV(const std::vector<WebCrawler::CrawlResult>& results,
                               const std::string& filename) {
    std::ofstream file(filename);
    file << "URL,Status Code,Success,Links Found,Content Length\n";

    for (const auto& result : results) {
        file << "\"" << result.url << "\","
             << result.statusCode << ","
             << (result.success ? "true" : "false") << ","
             << result.extractedLinks.size() << ","
             << result.content.length() << "\n";
    }

    file.close();
}
