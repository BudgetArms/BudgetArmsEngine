#pragma once

#include <array>
#include <source_location>
#include <string>

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


    void LogCustom(const std::string& message, const Utils::Color& color, bool bPrintToFile,
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
        static void SetConsoleColor(Utils::Color color);


        [[nodiscard]] Utils::Color GetVerbosityColor(LogVerbosity verbosity) const;
        [[nodiscard]] std::string_view GetVerbosityPrefix(LogVerbosity verbosity) const;

        static Utils::Color m_LogColor;

    private:
        std::filesystem::path m_LogFilePath{ "/" };
        LogVerbosity m_Verbosity{ LogVerbosity::Normal };
        bool m_bIsInitialized{};

        static constexpr Utils::Color m_ErrorColor{ Utils::Color::Magenta };
        static constexpr std::string_view m_ErrorPrefix{ "UNDEFINED_ERROR" };

        static inline const std::unordered_map<LogVerbosity, std::tuple<Utils::Color, std::string_view>> m_VerbosityData
        {
            { LogVerbosity::Error, { Utils::Color::Red, "ERROR" } },
            { LogVerbosity::Warning, { Utils::Color::Orange, "WARNING" } },
            { LogVerbosity::Success, { Utils::Color::LightGreen, "SUCCESS" } },
            { LogVerbosity::Normal, { Utils::Color::White, "LOG" } },
            { LogVerbosity::Verbose, { Utils::Color::Gray, "VERBOSE" } },
            { LogVerbosity::VeryVerbose, { Utils::Color::DarkGray, "VERYVERBOSE" } },
        };
    };
}
