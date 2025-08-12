//
// Created by docto on 8/11/2025.
//

#pragma once
#include <string>
#include <regex>
#include <optional>
#include <vector>

class UrlParser
{
public:
    struct ParsedUrl
    {
        std::string scheme;
        std::string host;
        int port = -1;  // Default to -1 for no port
        std::string path;
        std::string query;
        std::string fragment;
        bool valid = false;

        // Utility method to reconstruct URL
        [[nodiscard]] std::string toString() const;
    };
    [[nodiscard]] static ParsedUrl parse(const std::string& url);
    [[nodiscard]] static std::string normalize(const std::string& url);
    [[nodiscard]] static bool isValidUrl(const std::string& url);
    [[nodiscard]] static std::string makeAbsolute(const std::string& base, const std::string& relative);
    [[nodiscard]] static std::string extractDomain(const std::string& url);
    [[nodiscard]] static bool isSameDomain(const std::string& url1, const std::string& url2);
    [[nodiscard]] static std::string removeFragment(const std::string& url);

private:
    static std::string normalizePath(const std::string& path);
    static int getDefaultPort(const std::string& scheme);
    static bool isValidScheme(const std::string& scheme);
    static bool isValidHost(const std::string& host);
    static std::string toLowerCase(const std::string& str);
    static std::vector<std::string> splitPath(const std::string& path);
    static std::string joinPath(const std::vector<std::string>& segments);
    static std::string normalize(const std::string& url, bool removeFragment = false);
    static std::string isValidUrl(const std::string& url, bool removeFragment = false);

    static const std::regex URL_REGEX;

};
