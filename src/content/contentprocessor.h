//
// Created by docto on 8/16/2025.
//

#pragma once
#include <string>
#include <vector>
#include <map>

class ContentProcessor
{
public:
    // Process HTML content and extract text
    struct PageData {
        std::string title;
        std::string description;
        std::vector<std::string> keywords;
        std::string cleanText;
        std::map<std::string, std::string> metadata; // Additional metadata
        size_t wordcount = 0;
    };

    static PageData extractPageData(const std::string& html);
    static std::string extractTitle(const std::string& html);
    static std::string extractText(const std::string& html);
    static std::vector<std::string> extractKeywords(const std::string& text);
};