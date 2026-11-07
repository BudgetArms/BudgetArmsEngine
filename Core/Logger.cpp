#include "Logger.hpp"

#include <array>
#include <print>
#include <regex>

#include "Core/HelperFunctions.hpp"


using namespace bae;


Color Logger::m_LogColor         = Color::White;
LogVerbosity Logger::m_Verbosity = LogVerbosity::Normal;
bool Logger::m_bIsInitialized    = false;


LogVerbosity Logger::GetVerbosity()
{
    return m_Verbosity;
}

void Logger::LogCustom(const std::string& message, const std::string& prefix, const Color& color,
                       const bool bPrintToFile,
                       const std::source_location& location)
{
    if(!m_bIsInitialized)
    {
        return;
    }

    SetConsoleColor(color);

    std::println("{:<20}{:<50} {:<10} {}",
                 std::format("[{}]", prefix),
                 SanitizeFunctionNameSignature(location.function_name()),
                 std::format("Line {}:", location.line()),
                 message);

    SetConsoleColor(m_LogColor);

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
    if(!m_bIsInitialized)
    {
        return;
    }

    const std::string_view prefix = GetVerbosityPrefix(verbosity);
    const Color color             = GetVerbosityColor(verbosity);

    PrintLog(message, prefix, color, location);
}

void Logger::PrintLog(const std::string& message, const std::string_view& prefix, const Color& color,
                      const std::source_location& location)
{
    if(!m_bIsInitialized)
    {
        return;
    }

    SetConsoleColor(color);
    std::println("{:<20}{:<50} {:<10} {}",
                 std::format("[{}]", prefix),
                 SanitizeFunctionNameSignature(location.function_name()),
                 std::format("Line {}:", location.line()),
                 message);

    SetConsoleColor(m_LogColor);
}

void Logger::ForcePrintLoggerError(const std::string& message, const std::source_location& location)
{
    ForceSetConsoleColor(m_LoggerErrorColor);

    std::println("{:<20}{:<50} {:<10} {}",
                 std::format("[{}]", m_LoggerErrorPrefix),
                 SanitizeFunctionNameSignature(location.function_name()),
                 std::format("Line {}:", location.line()),
                 message);

    ForceSetConsoleColor(m_LogColor);
}


void bae::DebugPrintAllColors()
{
    std::println();
    Logger::LogCustom("Testing Colors", "CUSTOM", Color::White, false);
    Logger::LogCustom("Testing Colors", "CUSTOM", Color::Black, false);
    Logger::LogCustom("Testing Colors", "CUSTOM", Color::DarkRed, false);
    Logger::LogCustom("Testing Colors", "CUSTOM", Color::Red, false);
    Logger::LogCustom("Testing Colors", "CUSTOM", Color::Green, false);
    Logger::LogCustom("Testing Colors", "CUSTOM", Color::Lime, false);
    Logger::LogCustom("Testing Colors", "CUSTOM", Color::LightGreen, false);
    Logger::LogCustom("Testing Colors", "CUSTOM", Color::DarkGreen, false);
    Logger::LogCustom("Testing Colors", "CUSTOM", Color::Blue, false);
    Logger::LogCustom("Testing Colors", "CUSTOM", Color::Yellow, false);
    Logger::LogCustom("Testing Colors", "CUSTOM", Color::Orange, false);
    Logger::LogCustom("Testing Colors", "CUSTOM", Color::Magenta, false);
    Logger::LogCustom("Testing Colors", "CUSTOM", Color::Cyan, false);
    Logger::LogCustom("Testing Colors", "CUSTOM", Color::Purple, false);
    Logger::LogCustom("Testing Colors", "CUSTOM", Color::Gray, false);
    Logger::LogCustom("Testing Colors", "CUSTOM", Color::DarkGray, false);
    Logger::LogCustom("Testing Colors", "CUSTOM", Color::Transparent, false);
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
    if(!m_bIsInitialized)
    {
        return;
    }

    ForceSetConsoleColor(color);
}

Color Logger::GetVerbosityColor(const LogVerbosity verbosity)
{
    if(!m_bIsInitialized)
    {
        return m_LoggerErrorColor;
    }

    if(!m_VerbosityData.contains(verbosity))
    {
        ForcePrintLoggerError("VerbosityData doesn't contain the specified verbosity");
        return m_LoggerErrorColor;
    }

    const auto verbosityDataIt = m_VerbosityData.find(verbosity);
    if(verbosityDataIt == m_VerbosityData.end())
    {
        ForcePrintLoggerError("Failed to find verbosity color in verbosityData Element");
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
        ForcePrintLoggerError("VerbosityData doesn't contain the specified verbosity");
        return m_LoggerErrorPrefix;
    }

    const auto verbosityDataIt = m_VerbosityData.find(verbosity);
    if(verbosityDataIt == m_VerbosityData.end())
    {
        ForcePrintLoggerError("Failed to find verbosity color in verbosityData Element");
        return m_LoggerErrorPrefix;
    }

    auto& [_, prefix] = verbosityDataIt->second;
    return prefix;
}
