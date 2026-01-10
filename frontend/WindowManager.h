/**
 * @file WindowManager.h
 * @brief Manages ncurses windows and layout
 * @author Kerem Akdeniz
 * @date 2026-01-10
 */

#pragma once

#include <ncurses.h>
#include <string>
#include <map>

namespace project {

class WindowManager {
private:
    std::map<std::string, WINDOW*> windows;

public:
    WindowManager();
    ~WindowManager();
    
    void createWindows();
    void destroyWindows();
    void refreshAll();
    void resizeWindows();
    WINDOW* getWindow(const std::string& name);
};

} // namespace project
