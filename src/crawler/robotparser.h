//
// Created by docto on 8/16/2025.
//
#pragma once
#include <string>
#include <vector>
#include <chrono>

class RobotParser
{
public:
    struct RobotRule
    {
        std::vector<std::string> disallowedPaths{};
        std::vector<std::string> allowedPaths{};
        std::chrono::seconds crawlDelay{1}; // Default crawl delay
    };

    static RobotRule parseRobot(const std::string& robotsTxt, const std::string& userAgent);
    static bool isAllowed(const std::string& url, const RobotRule& rule);
};
