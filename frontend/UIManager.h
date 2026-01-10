/**
 * @file UIManager.h
 * @brief Main controller for the interactive TUI
 * @date 2025-12-30
 */

#pragma once

#include <ncurses.h>
#include <string>
#include "core/Simulation.h"
#include "WindowManager.h"
#include "ColorScheme.h"

namespace project {

enum class SimulationState {
    RUNNING,
    PAUSED,
    FINISHED
};

enum class ViewMode {
    NORMAL,
    BIN_DETAIL,
    FULL_MAP,
    HISTORY,
    CONFIG
};

class UIManager {
private:
    Simulation& simulation;
    WindowManager windows;
    ColorScheme colors;
    
    SimulationState state;
    ViewMode viewMode;
    
    int selectedBinIndex;
    double speedMultiplier;
    int frameDelay;
    
    bool autoScrollEnabled;
    int binListScrollOffset;
    
    std::string statusMessage;
    int messageTimer;

public:
    UIManager(Simulation& sim);
    ~UIManager();
    
    void initialize();
    void run();
    void cleanup();
    
private:
    void updateDisplay();
    void handleInput();
    
    void drawHeader();
    void drawBinList();
    void drawTruckStatus();
    void drawMap();
    void drawStats();
    void drawStatusBar();
    
    void drawBinDetail();
    void drawFullMap();
    void drawHistory();
    void drawConfig();
    
    void setStatusMessage(const std::string& msg, int duration = 60);
    void updateSpeed(bool increase);
    void togglePause();
    void stepSimulation();
    void resetSimulation();
    
    int getBinFillPercent(const Bin& bin) const;
    int getBinColorPair(int fillPercent) const;
    std::string formatDistance(int distance) const;
    std::string getProgressBar(int percent, int width) const;
};

} // namespace project