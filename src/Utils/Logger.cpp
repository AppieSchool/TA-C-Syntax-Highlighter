//
// Created by abdir on 16/05/2025.
//

#include "Logger.h"
#include <iostream>

void Logger::log(LogLevel level, const std::string& message) {
    // Check if DEBUG_MODE is enabled
    // Only print ERROR messages if DEBUG_MODE is off
    if (level != LogLevel::ERROR && !DEBUG_MODE) {
        return;
    }
    switch (level) {
        case LogLevel::INFO:    std::cout << "[INFO] "    << message << std::endl; break;
        case LogLevel::DEBUG:   std::cout << "[DEBUG] "   << message << std::endl; break;
        case LogLevel::WARNING: std::cerr << "[WARNING] " << message << std::endl; break;
        case LogLevel::ERROR:   std::cerr << "[ERROR] "   << message << std::endl; break;
        case LogLevel::SUCCESS: std::cout << "[SUCCESS] " << message << std::endl; break;
        default: std::cerr << "[UNKNOWN] " << message << std::endl; break;
    }
}
