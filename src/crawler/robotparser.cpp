//
// Created by docto on 8/16/2025.
//

#include "robotparser.h"
#include <sstream>
#include <algorithm>
#include <vector>
#include <chrono>

RobotParser::RobotRule RobotParser::parseRobot(const std::string& robotsTxt, const std::string& userAgent) {
    RobotRule rule;
    std::istringstream stream(robotsTxt);
    std::string line;
    bool userAgentSection = false;

    while (std::getline(stream, line)) {
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (line.empty() || line[0] == '#') {
            continue; // Skip empty lines and comments
        }

        if (line.rfind("User-agent:", 0) == 0) {
            std::string agent = line.substr(11);
            if (agent == userAgent || agent == "*") {
                userAgentSection = true;
            } else {
                userAgentSection = false;
            }
        } else if (userAgentSection) {
            if (line.rfind("Disallow:", 0) == 0) {
                rule.disallowedPaths.push_back(line.substr(10));
            } else if (line.rfind("Allow:", 0) == 0) {
                rule.allowedPaths.push_back(line.substr(7));
            } else if (line.rfind("Crawl-delay:", 0) == 0) {
                rule.crawlDelay = std::chrono::seconds(std::stoi(line.substr(12)));
            }
        }
    }

    return rule;
}

bool RobotParser::isAllowed(const std::string& url, const RobotRule& rule) {
    // Check if the URL matches any disallowed paths
    for (const auto& path : rule.disallowedPaths) {
        if (url.find(path) != std::string::npos) {
            return false; // URL is disallowed
        }
    }

    // Check if the URL matches any allowed paths
    for (const auto& path : rule.allowedPaths) {
        if (url.find(path) != std::string::npos) {
            return true; // URL is allowed
        }
    }

    return true; // Default to allowed if no rules match
}