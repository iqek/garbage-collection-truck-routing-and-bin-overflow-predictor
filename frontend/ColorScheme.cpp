/**
 * @file ColorScheme.cpp
 * @brief Implementation of ColorScheme
 * @date 2025-12-30
 */

#include "ColorScheme.h"

namespace project {

ColorScheme::ColorScheme() 
    : HEADER(1), SUCCESS(2), WARNING(3), DANGER(4), 
      CRITICAL(5), INFO(6), NORMAL(7), HIGHLIGHT(8) {
}

void ColorScheme::initialize() {
    // Header - Cyan on Black
    init_pair(HEADER, COLOR_CYAN, COLOR_BLACK);
    
    // Success - Green on Black (0-50% fill)
    init_pair(SUCCESS, COLOR_GREEN, COLOR_BLACK);
    
    // Warning - Yellow on Black (51-75% fill)
    init_pair(WARNING, COLOR_YELLOW, COLOR_BLACK);
    
    // Danger - Magenta on Black (76-90% fill)
    init_pair(DANGER, COLOR_MAGENTA, COLOR_BLACK);
    
    // Critical - Red on Black (91-100% fill, overflowing)
    init_pair(CRITICAL, COLOR_RED, COLOR_BLACK);
    
    // Info - Blue on Black
    init_pair(INFO, COLOR_BLUE, COLOR_BLACK);
    
    // Normal - White on Black
    init_pair(NORMAL, COLOR_WHITE, COLOR_BLACK);
    
    // Highlight - Black on White (for selection)
    init_pair(HIGHLIGHT, COLOR_BLACK, COLOR_WHITE);
}

} // namespace project