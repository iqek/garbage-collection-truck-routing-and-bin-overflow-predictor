/**
 * @file ColorScheme.h
 * @brief Color pair definitions for the TUI
 * @author Kerem Akdeniz
 * @date 2026-01-10
 */

#pragma once
#include <ncurses.h>
namespace project {

class ColorScheme {
public:
    int HEADER;
    int SUCCESS;
    int WARNING;
    int DANGER;
    int CRITICAL;
    int INFO;
    int NORMAL;
    int HIGHLIGHT;
    ColorScheme();
    void initialize();
};

} // namespace project
