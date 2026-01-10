/**
 * @file ColorScheme.h
 * @brief Color pair definitions for the TUI
 * @date 2025-12-30
 */

#pragma once

#include <ncurses.h>

namespace project {

class ColorScheme {
public:
    // Color pair indices
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