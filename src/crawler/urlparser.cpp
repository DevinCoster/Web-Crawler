//
// Created by docto on 8/11/2025.
//
#include "urlparser.h"
#include <iostream>

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
    return scheme + "://" + host + (port != getDefaultPort(scheme) ? ":" + std::to_string(port) : "") + path;
}

int UrlParser::getDefaultPort(const std::string& scheme) {
    return (scheme == "https") ? 443 : 80;
}
