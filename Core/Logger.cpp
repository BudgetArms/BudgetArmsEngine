#include "Logger.hpp"

#include <array>
#include <iostream>
#include <print>
#include <regex>

#include "HelperFunctions.hpp"


using namespace bae;


Color Logger::m_LogColor         = Color::White;
LogVerbosity Logger::m_Verbosity = LogVerbosity::Normal;
bool Logger::m_bIsInitialized    = false;

LogVerbosity Logger::GetVerbosity()
{
    return m_Verbosity;
}


void Logger::LogCustom(const std::string& message, const Color& color, const bool bPrintToFile,
                       const std::source_location& location)
{
    if(!m_bIsInitialized)
    {
        return;
    }

    SetConsoleColor(color);
    constexpr std::string_view prefix = "CUSTOM";

    std::println("{:<20}{:<50} {:<10} {}",
                 std::format("[{}]", prefix),
                 SanitizeFunctionNameSignature(location.function_name()),
                 std::format("Line {}:", location.line()),
                 message);

    SetConsoleColor(m_LogColor);
    if(bPrintToFile)
        return;
}


void Logger::LogError(const std::string& message, const std::source_location& location)
{
    PrintLog(message, LogVerbosity::Error, location);
}

void Logger::LogWarning(const std::string& message, const std::source_location& location)
{
    PrintLog(message, LogVerbosity::Warning, location);
}

void Logger::LogSuccess(const std::string& message, const std::source_location& location)
{
    PrintLog(message, LogVerbosity::Success, location);
}

void Logger::LogNormal(const std::string& message, const std::source_location& location)
{
    PrintLog(message, LogVerbosity::Normal, location);
}

void Logger::LogVerbose(const std::string& message, const std::source_location& location)
{
    PrintLog(message, LogVerbosity::Verbose, location);
}

void Logger::LogVeryVerbose(const std::string& message, const std::source_location& location)
{
    PrintLog(message, LogVerbosity::VeryVerbose, location);
}

std::string Logger::SanitizeFunctionNameSignature(const std::string& functionName)
{
    #ifdef _MSC_VER

    constexpr std::array<std::string_view, 5> msvcCallingConventions =
    {
        "__cdecl",
        "__stdcall",
        "__fastcall",
        "__thiscall",
        "__vectorcall"
    };

    std::string normalFunctionName = functionName;
    for(auto msvcCallingConvention : msvcCallingConventions)
    {
        normalFunctionName = Utils::ReplaceText(normalFunctionName, msvcCallingConvention, "");
    }

    // replace (void)
    normalFunctionName = Utils::ReplaceText(normalFunctionName, "(void)", "");

    return normalFunctionName;

    #else

    return functionName;
    #endif
}

void Logger::PrintLog(const std::string& message, const LogVerbosity verbosity, const std::source_location& location)
{
    SetConsoleColor(GetVerbosityColor(verbosity));
    const std::string_view prefix = GetVerbosityPrefix(verbosity);

    std::println("{:<20}{:<50} {:<10} {}",
                 std::format("[{}]", prefix),
                 SanitizeFunctionNameSignature(location.function_name()),
                 std::format("Line {}:", location.line()),
                 message);

    SetConsoleColor(m_LogColor);
}


void bae::DebugPrintAllColors()
{
    std::println();
    Logger::LogCustom("Testing Colors", Color::White, false);
    Logger::LogCustom("Testing Colors", Color::Black, false);
    Logger::LogCustom("Testing Colors", Color::DarkRed, false);
    Logger::LogCustom("Testing Colors", Color::Red, false);
    Logger::LogCustom("Testing Colors", Color::Green, false);
    Logger::LogCustom("Testing Colors", Color::Lime, false);
    Logger::LogCustom("Testing Colors", Color::LightGreen, false);
    Logger::LogCustom("Testing Colors", Color::DarkGreen, false);
    Logger::LogCustom("Testing Colors", Color::Blue, false);
    Logger::LogCustom("Testing Colors", Color::Yellow, false);
    Logger::LogCustom("Testing Colors", Color::Orange, false);
    Logger::LogCustom("Testing Colors", Color::Magenta, false);
    Logger::LogCustom("Testing Colors", Color::Cyan, false);
    Logger::LogCustom("Testing Colors", Color::Purple, false);
    Logger::LogCustom("Testing Colors", Color::Gray, false);
    Logger::LogCustom("Testing Colors", Color::DarkGray, false);
    Logger::LogCustom("Testing Colors", Color::Transparent, false);
    std::println();
}

void bae::DebugPrintAllVerbosity()
{
    std::println();
    Logger::LogSuccess("Testing Success");
    Logger::LogWarning("Testing Warning");
    Logger::LogError("Testing Error");
    Logger::LogNormal("Testing Normal");
    Logger::LogVerbose("Testing Verbose");
    Logger::LogVeryVerbose("Testing VeryVerbose");
    std::println();
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

void Logger::SetConsoleColor(const Color& color)
{
    const int r = static_cast<int>(color.R * 255);
    const int g = static_cast<int>(color.G * 255);
    const int b = static_cast<int>(color.B * 255);

    std::cout << "\033[38;2;" <<
            r << ";" <<
            g << ";" <<
            b << "m";
}

Color Logger::GetVerbosityColor(const LogVerbosity verbosity)
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

std::string_view Logger::GetVerbosityPrefix(const LogVerbosity verbosity)
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
