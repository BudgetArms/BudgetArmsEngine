#pragma once

#include <source_location>
#include <string>

#include "Core/Utils.hpp"


namespace bae
{
    enum class LogVerbosity
    {
        Fatal,
        Error,
        Warning,
        Success,
        Normal,
        Verbose,
        VeryVerbose,
        All = VeryVerbose
    };


    class Logger
    {
    public:
        virtual ~Logger() = default;

        Logger(const Logger&)            = delete;
        Logger(Logger&&)                 = delete;
        Logger& operator=(const Logger&) = delete;
        Logger& operator=(Logger&&)      = delete;


        static void Initialize(LogVerbosity verbosity);

        static void LogCustom(const std::string& message, const std::string& prefix, const Color& color,
                              bool bPrintToFile,
                              const std::source_location& location = std::source_location::current());

        static void LogFatal(const std::string& message,
                             const std::source_location& location = std::source_location::current());
        static void LogError(const std::string& message,
                             const std::source_location& location = std::source_location::current());
        static void LogWarning(const std::string& message,
                               const std::source_location& location = std::source_location::current());
        static void LogSuccess(const std::string& message,
                               const std::source_location& location = std::source_location::current());
        static void LogNormal(const std::string& message,
                              const std::source_location& location = std::source_location::current());
        static void LogVerbose(const std::string& message,
                               const std::source_location& location = std::source_location::current());
        static void LogVeryVerbose(const std::string& message,
                                   const std::source_location& location = std::source_location::current());


        [[nodiscard]] static Color GetVerbosityColor(LogVerbosity verbosity);
        [[nodiscard]] static std::string_view GetVerbosityPrefix(LogVerbosity verbosity);

        static void SetConsoleColor(const Color& color);

        [[nodiscard]] static Color GetLogColor();
        static void SetLogColor(const Color& color);

        static LogVerbosity GetLogVerbosity();


        static void DebugPrintAllColors();
        static void DebugPrintAllVerbosity();

    protected:
        Logger() = default;

        [[nodiscard]] static std::string SanitizeFunctionNameSignature(const std::string& functionName);

        static void ForceSetConsoleColor([[maybe_unused]] const Color& color);

        static void PrintLog(const std::string& message, LogVerbosity verbosity,
                             const std::source_location& location);

        static void PrintLog(const std::string& message, const std::string_view& prefix, const Color& color,
                             const std::source_location& location);

        static void PrintLoggerError(const std::string& message,
                                     const std::source_location& location = std::source_location::current());


        inline static bool m_bIsInitialized{};
        inline static LogVerbosity m_LogVerbosity{ LogVerbosity::Normal };
        inline static Color m_LogColor{ Color::White };
        inline static std::filesystem::path m_LogFilePath{ "/" };

        static constexpr Color m_LoggerErrorColor{ Color::Magenta };
        static constexpr std::string_view m_LoggerErrorPrefix{ "LOGGER_ERROR" };

        inline static std::unordered_map<LogVerbosity, std::tuple<Color, std::string_view>> m_VerbosityData
        {
            { LogVerbosity::Fatal, { Color::DarkRed, "FATAL" } },
            { LogVerbosity::Error, { Color::Red, "ERROR" } },
            { LogVerbosity::Warning, { Color::Orange, "WARNING" } },
            { LogVerbosity::Success, { Color::LightGreen, "SUCCESS" } },
            { LogVerbosity::Normal, { Color::White, "LOG" } },
            { LogVerbosity::Verbose, { Color::Gray, "VERBOSE" } },
            { LogVerbosity::VeryVerbose, { Color::DarkGray, "VERYVERBOSE" } },
        };
    };
}
