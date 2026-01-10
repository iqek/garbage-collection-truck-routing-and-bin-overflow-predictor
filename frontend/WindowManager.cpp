/**
 * @file WindowManager.cpp
 * @brief Implementation of WindowManager
 * @date 2025-12-30
 */

#include "WindowManager.h"

namespace project {

WindowManager::WindowManager() {
}

WindowManager::~WindowManager() {
    destroyWindows();
}

void WindowManager::createWindows() {
    // Header window (top, full width)
    windows["header"] = newwin(3, COLS, 0, 0);
    
    // Bin list window (left side, below header)
    windows["binlist"] = newwin(18, 26, 3, 0);
    
    // Truck status window (right side, upper)
    windows["truck"] = newwin(8, COLS - 26, 3, 26);
    
    // Map window (left side, bottom)
    windows["map"] = newwin(LINES - 21, 26, 21, 0);
    
    // Stats window (right side, lower)
    windows["stats"] = newwin(LINES - 11, COLS - 26, 11, 26);
}

void WindowManager::destroyWindows() {
    for (auto& pair : windows) {
        if (pair.second != nullptr) {
            delwin(pair.second);
            pair.second = nullptr;
        }
    }
    windows.clear();
}

void WindowManager::refreshAll() {
    for (auto& pair : windows) {
        if (pair.second != nullptr) {
            wrefresh(pair.second);
        }
    }
}

void WindowManager::resizeWindows() {
    destroyWindows();
    createWindows();
}

WINDOW* WindowManager::getWindow(const std::string& name) {
    auto it = windows.find(name);
    if (it != windows.end()) {
        return it->second;
    }
    return nullptr;
}

} // namespace project