#pragma once

#include <source_location>
#include <string>

#include "Utils.hpp"


namespace bae
{
    enum class LogVerbosity
    {
        Error,
        Warning,
        Success,
        Normal,
        Verbose,
        VeryVerbose,
        All = VeryVerbose
    };


    [[maybe_unused]] void DebugPrintAllColors();
    [[maybe_unused]] void DebugPrintAllVerbosity();

    class Logger
    {
    public:
        virtual ~Logger() = default;

        Logger(const Logger&)            = delete;
        Logger(Logger&&)                 = delete;
        Logger& operator=(const Logger&) = delete;
        Logger& operator=(Logger&&)      = delete;


        static void Initialize(LogVerbosity verbosity);
        static void SetConsoleColor(const Color& color);


        static void LogCustom(const std::string& message, const Color& color, bool bPrintToFile,
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

        static Color m_LogColor;
        static LogVerbosity GetVerbosity();

    protected:
        Logger() = default;

        [[nodiscard]] static std::string SanitizeFunctionNameSignature(const std::string& functionName);

        static
        void PrintLog(const std::string& message, LogVerbosity verbosity,
                      const std::source_location& location);

        std::filesystem::path m_LogFilePath{ "/" };
        static LogVerbosity m_Verbosity;
        static bool m_bIsInitialized;

        static constexpr Color m_ErrorColor{ Color::Magenta };
        static constexpr std::string_view m_ErrorPrefix{ "UNDEFINED_ERROR" };

        static inline const std::unordered_map<LogVerbosity, std::tuple<Color, std::string_view>> m_VerbosityData
        {
            { LogVerbosity::Error, { Color::Red, "ERROR" } },
            { LogVerbosity::Warning, { Color::Orange, "WARNING" } },
            { LogVerbosity::Success, { Color::LightGreen, "SUCCESS" } },
            { LogVerbosity::Normal, { Color::White, "LOG" } },
            { LogVerbosity::Verbose, { Color::Gray, "VERBOSE" } },
            { LogVerbosity::VeryVerbose, { Color::DarkGray, "VERYVERBOSE" } },
        };
    };
}
