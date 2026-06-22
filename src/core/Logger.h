#pragma once
#include <string>

namespace fb {

enum class LogLevel { Info, Warn, Error };

class Logger {
public:
    static void Log(LogLevel level, const std::string& msg,
                    const char* file, int line);
};

} // namespace fb

#define FB_INFO(msg)  fb::Logger::Log(fb::LogLevel::Info,  (msg), __FILE__, __LINE__)
#define FB_WARN(msg)  fb::Logger::Log(fb::LogLevel::Warn,  (msg), __FILE__, __LINE__)
#define FB_ERROR(msg) fb::Logger::Log(fb::LogLevel::Error, (msg), __FILE__, __LINE__)
