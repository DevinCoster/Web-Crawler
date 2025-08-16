//
// Created by docto on 8/11/2025.
//
#include "urlparser.h"
#include <iostream>
#include <algorithm>
#include <sstream>

// Static regex definition
const std::regex UrlParser::URL_REGEX{R"(^(https?):\/\/([^\/\s:]+)(?::(\d+))?(\/[^\s\?#]*)?(?:\?([^\s#]*))?(?:#([^\s]*))?$)"};

UrlParser::ParsedUrl UrlParser::parse(const std::string& url) {
    ParsedUrl result;
    std::smatch matches;

    if (std::regex_match(url, matches, URL_REGEX)) {
        result.scheme = matches[1].str();
        result.host = matches[2].str();
        result.port = matches[3].matched ? std::stoi(matches[3].str()) : getDefaultPort(result.scheme);
        result.path = matches[4].matched ? matches[4].str() : "/";
        result.query = matches[5].str();
        result.fragment = matches[6].str();
        result.valid = true;
    }

    return result;
}

std::string UrlParser::ParsedUrl::toString() const {
    std::string url = scheme + "://" + host;
    if (port != UrlParser::getDefaultPort(scheme) && port != -1) {
        url += ":" + std::to_string(port);
    }
    url += path;
    return url;
}

int UrlParser::getDefaultPort(const std::string& scheme) {
    return (scheme == "https") ? 443 : 80;
}

std::string UrlParser::normalize(const std::string& url)
{
    auto parsed = parse(url);
    if (!parsed.valid) return url;

    parsed.scheme = toLowerCase(parsed.scheme);
    parsed.host = toLowerCase(parsed.host);

    parsed.path = normalizePath(parsed.path);
    return parsed.toString();
}


std::string UrlParser::extractDomain(const std::string& url) {
    auto parsed = parse(url);
    return parsed.valid ? parsed.host : "";
}

bool UrlParser::isSameDomain(const std::string& url1, const std::string& url2) {
    return extractDomain(url1) == extractDomain(url2);
}

std::string UrlParser::removeFragment(const std::string& url) {
    size_t hashPos = url.find('#');
    return hashPos != std::string::npos ? url.substr(0, hashPos) : url;
}

bool UrlParser::isValidUrl(const std::string& url) {
    auto parsed = parse(url);
    return parsed.valid;
}

std::string UrlParser::makeAbsolute(const std::string& base, const std::string& relative) {
    if (relative.rfind("http://", 0) == 0 || relative.rfind("https://", 0) == 0) {
        return relative; // Already absolute
    }

    auto baseParsed = parse(base);
    if (!baseParsed.valid) return relative;

    if (!relative.empty() && relative[0] == '/') {
        return baseParsed.scheme + "://" + baseParsed.host + relative;
    }

    // Handle relative paths (simple implementation)
    std::string basePath = baseParsed.path;
    if (basePath.back() != '/') {
        size_t lastSlash = basePath.find_last_of('/');
        if (lastSlash != std::string::npos) {
            basePath = basePath.substr(0, lastSlash + 1);
        }
    }

    return baseParsed.scheme + "://" + baseParsed.host + basePath + relative;
}

// Helper methods
std::string UrlParser::toLowerCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string UrlParser::normalizePath(const std::string& path) {
    if (path.empty()) return "/";

    auto segments = splitPath(path);
    std::vector<std::string> normalized;

    for (const auto& segment : segments) {
        if (segment == "." || segment.empty()) {
            continue; // Skip current directory references
        } else if (segment == "..") {
            if (!normalized.empty()) {
                normalized.pop_back(); // Go up one directory
            }
        } else {
            normalized.push_back(segment);
        }
    }

    return joinPath(normalized);
}

std::vector<std::string> UrlParser::splitPath(const std::string& path) {
    std::vector<std::string> segments;
    std::stringstream ss(path);
    std::string segment;

    while (std::getline(ss, segment, '/')) {
        if (!segment.empty()) {
            segments.push_back(segment);
        }
    }

    return segments;
}

std::string UrlParser::joinPath(const std::vector<std::string>& segments) {
    if (segments.empty()) return "/";

    std::string result = "/";
    for (size_t i = 0; i < segments.size(); ++i) {
        if (i > 0) result += "/";
        result += segments[i];
    }

    return result;
}

bool UrlParser::isValidScheme(const std::string& scheme) {
    return scheme == "http" || scheme == "https";
}

bool UrlParser::isValidHost(const std::string& host) {
    return !host.empty() && host.find(' ') == std::string::npos;
}
