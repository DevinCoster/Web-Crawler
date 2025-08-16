//
// Created by docto on 8/16/2025.
//

#pragma once
#include <vector>
#include "../crawler/webcrawler.h"

class CrawlExport
{
public:
    static void exportToJSON(const std::vector<WebCrawler::CrawlResult>& results, const std::string& filename);
    static void exportToCSV(const std::vector<WebCrawler::CrawlResult>& results, const std::string& filename);
    static void generateResults(const std::vector<WebCrawler::CrawlResult>& results, const std::string& filename);
};