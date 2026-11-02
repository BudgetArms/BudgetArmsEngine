#include "Logger.hpp"

#include <iostream>
#include <print>


using namespace bae;

Utils::Color Logger::m_LogColor = Utils::Color::White;


void LogCustom(const std::string& message, const Utils::Color color, const bool bPrintToFile,
               const std::source_location& location)
{
    Logger::SetConsoleColor(color);
    std::cout
            << "[" << "CUSTOM" << "]"
            << std::setw(5) << " "
            << location.function_name()
            << std::setw(5) << " "
            << " Line " << location.line() << ": "
            << std::setw(5) << " "
            << message << '\n';

    Logger::SetConsoleColor(Logger::m_LogColor);

    bPrintToFile;
}

void bae::LogError(const std::string& message, const std::source_location& location)
{
    PrintLog(message, LogVerbosity::Error, location);
}

void bae::LogWarning(const std::string& message, const std::source_location& location)
{
    PrintLog(message, LogVerbosity::Warning, location);
}

void bae::LogSuccess(const std::string& message, const std::source_location& location)
{
    PrintLog(message, LogVerbosity::Success, location);
}

void bae::LogNormal(const std::string& message, const std::source_location& location)
{
    PrintLog(message, LogVerbosity::Normal, location);
}

void bae::LogVerbose(const std::string& message, const std::source_location& location)
{
    PrintLog(message, LogVerbosity::Verbose, location);
}

void bae::LogVeryVerbose(const std::string& message, const std::source_location& location)
{
    PrintLog(message, LogVerbosity::VeryVerbose, location);
}


void bae::PrintLog(const std::string& message, const LogVerbosity verbosity, const std::source_location& location)
{
    Logger::SetConsoleColor(Logger::GetInstance().GetVerbosityColor(verbosity));
    const std::string_view prefix = Logger::GetInstance().GetVerbosityPrefix(verbosity);
  
    std::println("{:<15}{:<40} {:<4} {}",
                 std::format("[{}]", prefix),
                 location.function_name(),
                 std::format("Line {}:", location.line()),
                 message);

    Logger::SetConsoleColor(Logger::m_LogColor);
}

void Logger::Initialize(const LogVerbosity verbosity)
{
    if(m_bIsInitialized)
    {
        LogError("Logger is already initialized");
        std::abort();
    }

    m_bIsInitialized = true;
    m_Verbosity      = verbosity;
}

void Logger::SetConsoleColor(const Utils::Color color)
{
    std::cout << "\033[38;2;" << (255.f * color.R) << ";" << (255.f * color.G) << ";" << (255.f * color.B) << "m";
}

Utils::Color Logger::GetVerbosityColor(const LogVerbosity verbosity) const
{
    if(!m_bIsInitialized)
    {
        LogError("Logger is not initialized");
        return m_ErrorColor;
    }

    if(!m_VerbosityData.contains(verbosity))
    {
        LogError("Verbosity Is Not Found");
        return m_ErrorColor;
    }

    auto verbosityDataIt = m_VerbosityData.find(verbosity);
    if(verbosityDataIt == m_VerbosityData.end())
    {
        LogError("Failed To Find Verbosity Color");
        return m_ErrorColor;
    }

    auto& [color, _] = verbosityDataIt->second;
    return color;
}

std::string_view Logger::GetVerbosityPrefix(const LogVerbosity verbosity) const
{
    if(!m_bIsInitialized)
    {
        LogError("Logger is not initialized");
        return m_ErrorPrefix;
    }

    if(!m_VerbosityData.contains(verbosity))
    {
        LogError("Verbosity Is Not Found");
        return m_ErrorPrefix;
    }

    auto verbosityDataIt = m_VerbosityData.find(verbosity);
    if(verbosityDataIt == m_VerbosityData.end())
    {
        LogError("Failed To Find Verbosity Color");
        return m_ErrorPrefix;
    }

    auto& [_, prefix] = verbosityDataIt->second;
    return prefix;
}
