//
// Created by docto on 8/16/2025.
//

#include "contentprocessor.h"
#include <regex>
#include <algorithm>
#include <sstream>

ContentProcessor::PageData ContentProcessor::extractPageData(const std::string& html) {
    PageData data;
    data.title = extractTitle(html);
    data.cleanText = extractText(html);
    data.keywords = extractKeywords(html);
    data.wordcount = std::count_if(data.cleanText.begin(), data.cleanText.end(), [](const char c) { return std::isspace(c); }) + 1;

    // Extract metadata
    const std::regex metaRegex(R"(<meta\s+name=["']([^"']+)["']\s+content=["']([^"']+)["'])", std::regex_constants::icase);
    std::sregex_iterator iter(html.begin(), html.end(), metaRegex);

    for (const std::sregex_iterator end; iter != end; ++iter) {
        data.metadata[(*iter)[1].str()] = (*iter)[2].str();
    }

    return data;
}

std::string ContentProcessor::extractTitle(const std::string& html)
{
    const std::regex titleRegex(R"(<title[^>]*>([^<]+)</title>)", std::regex_constants::icase);

    if (std::smatch match; std::regex_search(html, match, titleRegex)) // Use regex_search, not regex_match
    {
        return match[1].str();
    }

    return "";
}

std::string ContentProcessor::extractText(const std::string& html) {

    // Remove HTML tags
    std::string text = std::regex_replace(html, std::regex(R"(<[^>]+>)"), " ");

    // Remove extra whitespace
    text = std::regex_replace(text, std::regex(R"(\s+)"), " ");

    // Trim leading and trailing whitespace
    text.erase(0, text.find_first_not_of(" \t\n\r"));
    text.erase(text.find_last_not_of(" \t\n\r") + 1);
    return text;
}

std::vector <std::string> ContentProcessor::extractKeywords(const std::string& text)
{
    std::vector <std::string> keywords;
    std::istringstream stream(text);
    std::string word;

    while (stream >> word)
    {
        if (word.length() > 3) // Filter out short words
        {
            // Convert to lowercase
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            keywords.push_back(word);
        }
    }

    return keywords;
}
