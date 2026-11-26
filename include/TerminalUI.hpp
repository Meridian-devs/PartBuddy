#pragma once

#include <string>

namespace TUI
{
    // Reset CLI formatting
    constexpr const char* RESET     = "\033[0m";
    constexpr const char* BOLD      = "\033[1m";

    // ANSI escape characters for coloring text in CLI
    constexpr const char* RED       = "\033[31m";
    constexpr const char* GREEN     = "\033[32m";
    constexpr const char* YELLOW    = "\033[33m";
    constexpr const char* BLUE      = "\033[34m";
    constexpr const char* MAGENTA   = "\033[35m";
    constexpr const char* CYAN      = "\033[36m";
    constexpr const char* WHITE     = "\033[37m";

    // ANSI escape characters for coloring text in CLI
    constexpr const char* BOLDRED       = "\033[1;31m";
    constexpr const char* BOLDGREEN     = "\033[1;32m";
    constexpr const char* BOLDYELLOW    = "\033[1;33m";
    constexpr const char* BOLDBLUE      = "\033[1;34m";
    constexpr const char* BOLDMAGENTA   = "\033[1;35m";
    constexpr const char* BOLDCYAN      = "\033[1;36m";
    constexpr const char* BOLDWHITE     = "\033[1;37m";
}
