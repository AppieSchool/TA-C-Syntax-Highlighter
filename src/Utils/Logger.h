//
// Created by abdir on 16/05/2025.
//

#ifndef ENGINE_LOGGER_H
#define ENGINE_LOGGER_H

// Zet DEBUG_MODE op true om debugberichten te tonen, false om ze te verbergen
#define DEBUG_MODE true
#include <string>

enum class LogLevel { INFO, DEBUG, WARNING, ERROR, SUCCESS };

class Logger {
public:
    static void log(LogLevel level, const std::string& message);
};

#endif //ENGINE_LOGGER_H

