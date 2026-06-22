#include "Logger.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <filesystem>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace fb {

void Logger::Log(LogLevel level, const std::string& msg, const char* file, int line) {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm;
#ifdef _WIN32
    localtime_s(&local_tm, &time_t_now);
#else
    localtime_r(&time_t_now, &local_tm);
#endif

    std::string levelStr;
    std::string colorCode;
    std::string resetCode = "\033[0m";

    switch (level) {
        case LogLevel::Info:
            levelStr = "INFO";
            colorCode = "\033[32m";
            break;
        case LogLevel::Warn:
            levelStr = "WARN";
            colorCode = "\033[33m";
            break;
        case LogLevel::Error:
            levelStr = "ERROR";
            colorCode = "\033[31m";
            break;
    }

    std::string filename = std::filesystem::path(file).filename().string();

    std::ostream& out = (level == LogLevel::Error) ? std::cerr : std::cout;

#ifdef _MSC_VER
    // Skip ANSI codes on MSVC unless explicitly enabled (omitted for now)
    out << "[" << std::put_time(&local_tm, "%H:%M:%S") << "] "
        << "[" << levelStr << "] "
        << "(" << filename << ":" << line << ") "
        << msg << std::endl;
#else
    out << colorCode << "[" << std::put_time(&local_tm, "%H:%M:%S") << "] "
        << "[" << levelStr << "] "
        << "(" << filename << ":" << line << ") "
        << msg << resetCode << std::endl;
#endif
}

} // namespace fb
