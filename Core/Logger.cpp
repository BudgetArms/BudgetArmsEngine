#include "Logger.hpp"

#include <array>
#include <print>
#include <regex>

#include "Core/HelperFunctions.hpp"


using namespace bae;


void Logger::Initialize(const LogVerbosity verbosity)
{
    if(m_bIsInitialized)
    {
        LogWarning("Logger is already initialized");
        return;
    }

    m_bIsInitialized = true;
    m_LogVerbosity   = verbosity;
}

void Logger::LogCustom(const std::string& message, const std::string& prefix, const Color& color,
                       const bool bPrintToFile,
                       const std::source_location& location)
{
    if(!m_bIsInitialized)
    {
        return;
    }

    PrintLog(message, prefix, color, location);

    // TODO: Add logic
    if(bPrintToFile)
        return;
}

void Logger::LogFatal(const std::string& message, const std::source_location& location)
{
    if(!m_bIsInitialized)
    {
        return;
    }

    PrintLog(message, LogVerbosity::Fatal, location);
    // TODO: Add log file logic
}


void Logger::LogError(const std::string& message, const std::source_location& location)
{
    if(!m_bIsInitialized)
    {
        return;
    }

    PrintLog(message, LogVerbosity::Error, location);
    // TODO: Add log file logic
}

void Logger::LogWarning(const std::string& message, const std::source_location& location)
{
    if(!m_bIsInitialized)
    {
        return;
    }

    PrintLog(message, LogVerbosity::Warning, location);
    // TODO: Add log file logic
}

void Logger::LogSuccess(const std::string& message, const std::source_location& location)
{
    if(!m_bIsInitialized)
    {
        return;
    }

    PrintLog(message, LogVerbosity::Success, location);
    // TODO: Add log file logic
}

void Logger::LogNormal(const std::string& message, const std::source_location& location)
{
    if(!m_bIsInitialized)
    {
        return;
    }

    PrintLog(message, LogVerbosity::Normal, location);
    // TODO: Add log file logic
}

void Logger::LogVerbose(const std::string& message, const std::source_location& location)
{
    if(!m_bIsInitialized)
    {
        return;
    }

    PrintLog(message, LogVerbosity::Verbose, location);
    // TODO: Add log file logic
}

void Logger::LogVeryVerbose(const std::string& message, const std::source_location& location)
{
    if(!m_bIsInitialized)
    {
        return;
    }

    PrintLog(message, LogVerbosity::VeryVerbose, location);
    // TODO: Add log file logic
}

Color Logger::GetVerbosityColor(const LogVerbosity verbosity)
{
    if(!m_bIsInitialized)
    {
        return m_LoggerErrorColor;
    }

    if(!m_VerbosityData.contains(verbosity))
    {
        PrintLoggerError("VerbosityData doesn't contain the specified verbosity");
        return m_LoggerErrorColor;
    }

    const auto verbosityDataIt = m_VerbosityData.find(verbosity);
    if(verbosityDataIt == m_VerbosityData.end())
    {
        PrintLoggerError("Failed to find verbosity color in verbosityData Element");
        return m_LoggerErrorColor;
    }

    auto& [color, _] = verbosityDataIt->second;
    return color;
}

std::string_view Logger::GetVerbosityPrefix(const LogVerbosity verbosity)
{
    if(!m_bIsInitialized)
    {
        return "LOGGER_NOT_INITIALIZED";
    }

    if(!m_VerbosityData.contains(verbosity))
    {
        PrintLoggerError("VerbosityData doesn't contain the specified verbosity");
        return m_LoggerErrorPrefix;
    }

    const auto verbosityDataIt = m_VerbosityData.find(verbosity);
    if(verbosityDataIt == m_VerbosityData.end())
    {
        PrintLoggerError("Failed to find verbosity color in verbosityData Element");
        return m_LoggerErrorPrefix;
    }

    auto& [_, prefix] = verbosityDataIt->second;
    return prefix;
}

void Logger::SetConsoleColor(const Color& color)
{
    if(!m_bIsInitialized)
    {
        return;
    }

    ForceSetConsoleColor(color);
}

Color Logger::GetLogColor()
{
    return m_LogColor;
}

void Logger::SetLogColor(const Color& color)
{
    m_LogColor = color;
}

LogVerbosity Logger::GetLogVerbosity()
{
    return m_LogVerbosity;
}

void Logger::DebugPrintAllColors()
{
    std::println();
    LogCustom("Testing Colors", "CUSTOM", Color::White, false);
    LogCustom("Testing Colors", "CUSTOM", Color::Black, false);
    LogCustom("Testing Colors", "CUSTOM", Color::DarkRed, false);
    LogCustom("Testing Colors", "CUSTOM", Color::Red, false);
    LogCustom("Testing Colors", "CUSTOM", Color::Green, false);
    LogCustom("Testing Colors", "CUSTOM", Color::Lime, false);
    LogCustom("Testing Colors", "CUSTOM", Color::LightGreen, false);
    LogCustom("Testing Colors", "CUSTOM", Color::DarkGreen, false);
    LogCustom("Testing Colors", "CUSTOM", Color::Blue, false);
    LogCustom("Testing Colors", "CUSTOM", Color::Yellow, false);
    LogCustom("Testing Colors", "CUSTOM", Color::Orange, false);
    LogCustom("Testing Colors", "CUSTOM", Color::Magenta, false);
    LogCustom("Testing Colors", "CUSTOM", Color::Cyan, false);
    LogCustom("Testing Colors", "CUSTOM", Color::Purple, false);
    LogCustom("Testing Colors", "CUSTOM", Color::Gray, false);
    LogCustom("Testing Colors", "CUSTOM", Color::DarkGray, false);
    LogCustom("Testing Colors", "CUSTOM", Color::Transparent, false);
    std::println();
}


void Logger::DebugPrintAllVerbosity()
{
    std::println();
    LogFatal("Testing Fatal");
    LogError("Testing Error");
    LogWarning("Testing Warning");
    LogSuccess("Testing Success");
    LogNormal("Testing Normal");
    LogVerbose("Testing Verbose");
    LogVeryVerbose("Testing VeryVerbose");
    std::println();
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

void Logger::ForceSetConsoleColor([[maybe_unused]] const Color& color)
{
    #ifndef __EMSCRIPTEN__
    const int r = static_cast<int>(color.R * 255);
    const int g = static_cast<int>(color.G * 255);
    const int b = static_cast<int>(color.B * 255);

    std::print("\033[38;2;{};{};{}m", r, g, b);
    #endif
}

void Logger::PrintLog(const std::string& message, const LogVerbosity verbosity, const std::source_location& location)
{
    const std::string_view prefix = GetVerbosityPrefix(verbosity);
    const Color color             = GetVerbosityColor(verbosity);

    PrintLog(message, prefix, color, location);
}

void Logger::PrintLog(const std::string& message, const std::string_view& prefix, const Color& color,
                      const std::source_location& location)
{
    ForceSetConsoleColor(color);
    std::println("{:<20}{:<50} {:<10} {}",
                 std::format("[{}]", prefix),
                 SanitizeFunctionNameSignature(location.function_name()),
                 std::format("Line {}:", location.line()),
                 message);
    ForceSetConsoleColor(m_LogColor);
}

void Logger::PrintLoggerError(const std::string& message, const std::source_location& location)
{
    PrintLog(message, m_LoggerErrorPrefix, m_LoggerErrorColor, location);
}
