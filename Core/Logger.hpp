#pragma once

#include <array>
#include <source_location>
#include <string>

#include "Color.hpp"
#include "Utils.hpp"
#include "Singletons/Singleton.hpp"


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


    void LogCustom(const std::string& message, const Color& color, bool bPrintToFile,
                   const std::source_location& location = std::source_location::current());

    void LogError(const std::string& message,
                  const std::source_location& location = std::source_location::current());
    void LogWarning(const std::string& message,
                    const std::source_location& location = std::source_location::current());
    void LogSuccess(const std::string& message,
                    const std::source_location& location = std::source_location::current());
    void LogNormal(const std::string& message,
                   const std::source_location& location = std::source_location::current());
    void LogVerbose(const std::string& message,
                    const std::source_location& location = std::source_location::current());
    void LogVeryVerbose(const std::string& message,
                        const std::source_location& location = std::source_location::current());

    // Do Not Use
    void PrintLog(const std::string& message, LogVerbosity verbosity, const std::source_location& location);

    void DebugPrintAllColors();

    class Logger : public Singleton<Logger>
    {
    public:
        void Initialize(LogVerbosity verbosity);
        static void SetConsoleColor(Color color);


        [[nodiscard]] Color GetVerbosityColor(LogVerbosity verbosity) const;
        [[nodiscard]] std::string_view GetVerbosityPrefix(LogVerbosity verbosity) const;

        static Color m_LogColor;

    private:
        std::filesystem::path m_LogFilePath{ "/" };
        LogVerbosity m_Verbosity{ LogVerbosity::Normal };
        bool m_bIsInitialized{};

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
