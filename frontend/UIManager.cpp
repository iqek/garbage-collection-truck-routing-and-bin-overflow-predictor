/**
 * @file UIManager.cpp
 * @brief Implementation of UIManager
 * @date 2025-12-30
 */

#include "UIManager.h"
#include <unistd.h>
#include <sstream>
#include <iomanip>

namespace project {

UIManager::UIManager(Simulation& sim) 
    : simulation(sim), 
      state(SimulationState::PAUSED),
      viewMode(ViewMode::NORMAL),
      selectedBinIndex(0),
      speedMultiplier(1.0),
      frameDelay(100000),
      autoScrollEnabled(true),
      binListScrollOffset(0),
      messageTimer(0) {
}

UIManager::~UIManager() {
    cleanup();
}

void UIManager::initialize() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
    
    if (has_colors()) {
        start_color();
        colors.initialize();
    }
    
    windows.createWindows();
    setStatusMessage("Garbage Collection Simulator - Press [P] to start", 120);
}

void UIManager::run() {
    while (viewMode != ViewMode::NORMAL || state != SimulationState::FINISHED) {
        handleInput();
        
        if (state == SimulationState::RUNNING && !simulation.isFinished()) {
            simulation.step();
        }
        
        if (simulation.isFinished() && state == SimulationState::RUNNING) {
            state = SimulationState::FINISHED;
            setStatusMessage("Simulation Complete! Press [R] to restart or [Q] to quit", 300);
        }
        
        updateDisplay();
        
        if (messageTimer > 0) messageTimer--;
        
        usleep(frameDelay);
    }
}

void UIManager::cleanup() {
    windows.destroyWindows();
    endwin();
}

void UIManager::updateDisplay() {
    erase();
    
    switch (viewMode) {
        case ViewMode::NORMAL:
            drawHeader();
            drawBinList();
            drawTruckStatus();
            drawMap();
            drawStats();
            drawStatusBar();
            break;
        case ViewMode::BIN_DETAIL:
            drawBinDetail();
            break;
        case ViewMode::FULL_MAP:
            drawFullMap();
            break;
        case ViewMode::HISTORY:
            drawHistory();
            break;
        case ViewMode::CONFIG:
            drawConfig();
            break;
    }
    
    refresh();
    windows.refreshAll();
}

void UIManager::handleInput() {
    int ch = getch();
    if (ch == ERR) return;
    
    switch (ch) {
        case 'q':
        case 'Q':
            if (viewMode == ViewMode::NORMAL) {
                state = SimulationState::FINISHED;
            } else {
                viewMode = ViewMode::NORMAL;
            }
            break;
            
        case 'p':
        case 'P':
            togglePause();
            break;
            
        case 's':
        case 'S':
            if (state == SimulationState::PAUSED) {
                stepSimulation();
            }
            break;
            
        case 'r':
        case 'R':
            resetSimulation();
            break;
            
        case '+':
        case '=':
            updateSpeed(true);
            break;
            
        case '-':
        case '_':
            updateSpeed(false);
            break;
            
        case KEY_UP:
            if (viewMode == ViewMode::NORMAL && selectedBinIndex > 0) {
                selectedBinIndex--;
                if (selectedBinIndex < binListScrollOffset) {
                    binListScrollOffset = selectedBinIndex;
                }
            }
            break;
            
        case KEY_DOWN:
            if (viewMode == ViewMode::NORMAL) {
                Facilities& facilities = simulation.getFacilities();
                if (selectedBinIndex < facilities.getBinCount() - 1) {
                    selectedBinIndex++;
                }
            }
            break;
            
        case 10: // Enter
        case KEY_ENTER:
            if (viewMode == ViewMode::NORMAL) {
                viewMode = ViewMode::BIN_DETAIL;
            } else {
                viewMode = ViewMode::NORMAL;
            }
            break;
            
        case 'v':
        case 'V':
            viewMode = ViewMode::FULL_MAP;
            break;
            
        case 'h':
        case 'H':
            viewMode = ViewMode::HISTORY;
            break;
            
        case 'c':
        case 'C':
            viewMode = ViewMode::CONFIG;
            break;
            
        case 27: // ESC
            viewMode = ViewMode::NORMAL;
            break;
    }
}

void UIManager::drawHeader() {
    WINDOW* win = windows.getWindow("header");
    werase(win);
    box(win, 0, 0);
    
    wattron(win, COLOR_PAIR(colors.HEADER) | A_BOLD);
    mvwprintw(win, 0, 2, " GARBAGE COLLECTION SIMULATOR ");
    wattroff(win, COLOR_PAIR(colors.HEADER) | A_BOLD);
    
    std::string dayInfo = "Day: " + std::to_string(simulation.getTime()) + "/" + 
                          std::to_string(simulation.getMaxTime());
    mvwprintw(win, 0, 35, "%s", dayInfo.c_str());
    
    std::string controls = "[P]ause [S]tep [R]eset [Q]uit";
    mvwprintw(win, 0, 55, "%s", controls.c_str());
    
    if (state == SimulationState::RUNNING) {
        wattron(win, COLOR_PAIR(colors.SUCCESS));
        mvwprintw(win, 1, 2, "> RUNNING");
        wattroff(win, COLOR_PAIR(colors.SUCCESS));
    } else if (state == SimulationState::PAUSED) {
        wattron(win, COLOR_PAIR(colors.WARNING));
        mvwprintw(win, 1, 2, "|| PAUSED");
        wattroff(win, COLOR_PAIR(colors.WARNING));
    } else {
        wattron(win, COLOR_PAIR(colors.INFO));
        mvwprintw(win, 1, 2, "[] FINISHED");
        wattroff(win, COLOR_PAIR(colors.INFO));
    }
    
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << speedMultiplier << "x";
    mvwprintw(win, 1, 15, "Speed: %s", ss.str().c_str());
}

void UIManager::drawBinList() {
    WINDOW* win = windows.getWindow("binlist");
    werase(win);
    box(win, 0, 0);
    
    wattron(win, COLOR_PAIR(colors.HEADER) | A_BOLD);
    mvwprintw(win, 0, 2, " BIN STATUS ");
    wattroff(win, COLOR_PAIR(colors.HEADER) | A_BOLD);
    
    Facilities& facilities = simulation.getFacilities();
    int binCount = facilities.getBinCount();
    
    mvwprintw(win, 0, 16, "(%d)", binCount);
    
    int maxDisplay = 15;
    int row = 2;
    
    for (int i = binListScrollOffset; i < binCount && row < maxDisplay + 2; i++) {
        Bin& bin = facilities.getBin(i);
        int fillPercent = getBinFillPercent(bin);
        int colorPair = getBinColorPair(fillPercent);
        
        if (i == selectedBinIndex) {
            wattron(win, A_REVERSE);
        }
        
        mvwprintw(win, row, 2, "[%s]", bin.getId().c_str());
        
        std::string bar = getProgressBar(fillPercent, 10);
        wattron(win, COLOR_PAIR(colorPair));
        mvwprintw(win, row, 8, "%s", bar.c_str());
        wattroff(win, COLOR_PAIR(colorPair));
        
        mvwprintw(win, row, 19, "%3d%%", fillPercent);
        
        if (bin.isOverflowing()) {
            wattron(win, COLOR_PAIR(colors.CRITICAL) | A_BLINK);
            mvwprintw(win, row, 23, "!");
            wattroff(win, COLOR_PAIR(colors.CRITICAL) | A_BLINK);
        }
        
        if (i == selectedBinIndex) {
            wattroff(win, A_REVERSE);
        }
        
        row++;
    }
    
    mvwprintw(win, maxDisplay + 3, 2, "[Up/Dn] Navigate");
    mvwprintw(win, maxDisplay + 4, 2, "[Enter] Details");
}

void UIManager::drawTruckStatus() {
    WINDOW* win = windows.getWindow("truck");
    werase(win);
    box(win, 0, 0);
    
    wattron(win, COLOR_PAIR(colors.HEADER) | A_BOLD);
    mvwprintw(win, 0, 2, " TRUCK STATUS ");
    wattroff(win, COLOR_PAIR(colors.HEADER) | A_BOLD);
    
    Truck& truck = simulation.getFacilities().getTruck();
    
    mvwprintw(win, 2, 2, "ID: %s", truck.getId().c_str());
    mvwprintw(win, 3, 2, "Location: Node %d", truck.getCurrentNode());
    
    int loadPercent = (truck.getCapacity() > 0) ? 
                      (truck.getLoad() * 100 / truck.getCapacity()) : 0;
    
    mvwprintw(win, 4, 2, "Load: %d/%d ", truck.getLoad(), truck.getCapacity());
    
    std::string bar = getProgressBar(loadPercent, 10);
    int colorPair = getBinColorPair(loadPercent);
    wattron(win, COLOR_PAIR(colorPair));
    mvwprintw(win, 4, 20, "%s", bar.c_str());
    wattroff(win, COLOR_PAIR(colorPair));
    mvwprintw(win, 4, 31, "%3d%%", loadPercent);
    
    if (truck.isFull()) {
        wattron(win, COLOR_PAIR(colors.WARNING));
        mvwprintw(win, 5, 2, "Status: FULL - Heading to disposal");
        wattroff(win, COLOR_PAIR(colors.WARNING));
    } else {
        wattron(win, COLOR_PAIR(colors.SUCCESS));
        mvwprintw(win, 5, 2, "Status: Collecting");
        wattroff(win, COLOR_PAIR(colors.SUCCESS));
    }
}

void UIManager::drawMap() {
    WINDOW* win = windows.getWindow("map");
    werase(win);
    box(win, 0, 0);
    
    wattron(win, COLOR_PAIR(colors.HEADER) | A_BOLD);
    mvwprintw(win, 0, 2, " CITY MAP ");
    wattroff(win, COLOR_PAIR(colors.HEADER) | A_BOLD);
    
    mvwprintw(win, 2, 2, "   D ---- B1");
    mvwprintw(win, 3, 2, "   |      |");
    mvwprintw(win, 4, 2, "   B2 - DS1");
    mvwprintw(win, 5, 2, "   |  /  |");
    mvwprintw(win, 6, 2, "   B3----B4");
    mvwprintw(win, 7, 2, "     \\");
    mvwprintw(win, 8, 2, "       B5");
    
    mvwprintw(win, 10, 2, "[V] Full Map View");
}

void UIManager::drawStats() {
    WINDOW* win = windows.getWindow("stats");
    werase(win);
    box(win, 0, 0);
    
    wattron(win, COLOR_PAIR(colors.HEADER) | A_BOLD);
    mvwprintw(win, 0, 2, " STATISTICS & ALERTS ");
    wattroff(win, COLOR_PAIR(colors.HEADER) | A_BOLD);
    
    mvwprintw(win, 2, 2, "Distance: %s", 
              formatDistance(simulation.getTotalDistance()).c_str());
    mvwprintw(win, 3, 2, "Collections: %d", simulation.getCollectionsCompleted());
    
    int overflows = simulation.getOverflowCount();
    if (overflows > 0) {
        wattron(win, COLOR_PAIR(colors.CRITICAL));
        mvwprintw(win, 4, 2, "Overflows: %d", overflows);
        wattroff(win, COLOR_PAIR(colors.CRITICAL));
    } else {
        wattron(win, COLOR_PAIR(colors.SUCCESS));
        mvwprintw(win, 4, 2, "Overflows: 0");
        wattroff(win, COLOR_PAIR(colors.SUCCESS));
    }
    
    int alertRow = 6;
    Facilities& facilities = simulation.getFacilities();
    OverflowPredictor predictor(2);
    
    for (int i = 0; i < facilities.getBinCount() && alertRow < 14; i++) {
        Bin& bin = facilities.getBin(i);
        
        if (bin.isOverflowing()) {
            wattron(win, COLOR_PAIR(colors.CRITICAL) | A_BOLD);
            mvwprintw(win, alertRow++, 2, "! CRITICAL: %s overflowing NOW!", 
                     bin.getId().c_str());
            wattroff(win, COLOR_PAIR(colors.CRITICAL) | A_BOLD);
        } else if (predictor.isCritical(bin)) {
            int days = predictor.predictDaysToOverflow(bin);
            wattron(win, COLOR_PAIR(colors.WARNING));
            mvwprintw(win, alertRow++, 2, "! WARNING: %s critical (%.1f days)", 
                     bin.getId().c_str(), days / 1.0);
            wattroff(win, COLOR_PAIR(colors.WARNING));
        }
    }
    
    mvwprintw(win, 16, 2, "[H]istory [C]onfig");
}

void UIManager::drawStatusBar() {
    if (messageTimer > 0 && !statusMessage.empty()) {
        attron(COLOR_PAIR(colors.INFO));
        mvprintw(LINES - 1, 0, "%s", statusMessage.c_str());
        attroff(COLOR_PAIR(colors.INFO));
    }
}

void UIManager::drawBinDetail() {
    WINDOW* win = stdscr;
    werase(win);
    
    Facilities& facilities = simulation.getFacilities();
    if (selectedBinIndex >= facilities.getBinCount()) {
        viewMode = ViewMode::NORMAL;
        return;
    }
    
    Bin& bin = facilities.getBin(selectedBinIndex);
    
    int centerX = COLS / 2;
    int centerY = LINES / 2;
    int boxWidth = 40;
    int boxHeight = 16;
    int startX = centerX - boxWidth / 2;
    int startY = centerY - boxHeight / 2;
    
    attron(COLOR_PAIR(colors.HEADER) | A_BOLD);
    mvprintw(startY, startX, "+");
    for (int i = 1; i < boxWidth - 1; i++) printw("-");
    printw("+");
    attroff(COLOR_PAIR(colors.HEADER) | A_BOLD);
    
    for (int i = 1; i < boxHeight - 1; i++) {
        mvprintw(startY + i, startX, "|");
        mvprintw(startY + i, startX + boxWidth - 1, "|");
    }
    
    attron(COLOR_PAIR(colors.HEADER) | A_BOLD);
    mvprintw(startY + boxHeight - 1, startX, "+");
    for (int i = 1; i < boxWidth - 1; i++) printw("-");
    printw("+");
    attroff(COLOR_PAIR(colors.HEADER) | A_BOLD);
    
    attron(COLOR_PAIR(colors.HEADER) | A_BOLD);
    mvprintw(startY, startX + 2, " BIN DETAILS: %s ", bin.getId().c_str());
    attroff(COLOR_PAIR(colors.HEADER) | A_BOLD);
    
    mvprintw(startY + 2, startX + 2, "Location: %s", bin.getLocation().c_str());
    mvprintw(startY + 3, startX + 2, "Current Fill: %d/%d units", 
             bin.getCurrentFill(), bin.getCapacity());
    mvprintw(startY + 4, startX + 2, "Fill Rate: %d units/day", bin.getFillRate());
    
    OverflowPredictor predictor(2);
    int days = predictor.predictDaysToOverflow(bin);
    if (days >= 0) {
        mvprintw(startY + 5, startX + 2, "Days to Overflow: %.1f days", days / 1.0);
    } else {
        attron(COLOR_PAIR(colors.CRITICAL));
        mvprintw(startY + 5, startX + 2, "Status: OVERFLOWING");
        attroff(COLOR_PAIR(colors.CRITICAL));
    }
    
    mvprintw(startY + 7, startX + 2, "FILL HISTORY (7 days)");
    
    mvprintw(startY + boxHeight - 2, startX + 2, "[Enter] Close  [ESC] Back");
    
    refresh();
}

void UIManager::drawFullMap() {
    // Placeholder for full screen map
    werase(stdscr);
    attron(COLOR_PAIR(colors.HEADER) | A_BOLD);
    mvprintw(0, 0, "FULL MAP VIEW - Press [ESC] to return");
    attroff(COLOR_PAIR(colors.HEADER) | A_BOLD);
    refresh();
}

void UIManager::drawHistory() {
    // Placeholder for history view
    werase(stdscr);
    attron(COLOR_PAIR(colors.HEADER) | A_BOLD);
    mvprintw(0, 0, "SIMULATION HISTORY - Press [ESC] to return");
    attroff(COLOR_PAIR(colors.HEADER) | A_BOLD);
    refresh();
}

void UIManager::drawConfig() {
    // Placeholder for config view
    werase(stdscr);
    attron(COLOR_PAIR(colors.HEADER) | A_BOLD);
    mvprintw(0, 0, "CONFIGURATION - Press [ESC] to return");
    attroff(COLOR_PAIR(colors.HEADER) | A_BOLD);
    refresh();
}

void UIManager::setStatusMessage(const std::string& msg, int duration) {
    statusMessage = msg;
    messageTimer = duration;
}

void UIManager::updateSpeed(bool increase) {
    if (increase) {
        if (speedMultiplier < 10.0) {
            speedMultiplier *= 2.0;
            frameDelay /= 2;
        }
    } else {
        if (speedMultiplier > 0.25) {
            speedMultiplier /= 2.0;
            frameDelay *= 2;
        }
    }
    
    std::stringstream ss;
    ss << "Speed: " << std::fixed << std::setprecision(1) << speedMultiplier << "x";
    setStatusMessage(ss.str(), 60);
}

void UIManager::togglePause() {
    if (state == SimulationState::RUNNING) {
        state = SimulationState::PAUSED;
        setStatusMessage("Simulation paused - Press [P] to resume or [S] to step", 120);
    } else if (state == SimulationState::PAUSED) {
        state = SimulationState::RUNNING;
        setStatusMessage("Simulation running", 60);
    }
}

void UIManager::stepSimulation() {
    if (!simulation.isFinished()) {
        simulation.step();
        setStatusMessage("Advanced 1 day", 30);
    }
}

void UIManager::resetSimulation() {
    // Note: This would require simulation reset functionality
    setStatusMessage("Reset not yet implemented", 60);
}

int UIManager::getBinFillPercent(const Bin& bin) const {
    if (bin.getCapacity() == 0) return 0;
    return (bin.getCurrentFill() * 100) / bin.getCapacity();
}

int UIManager::getBinColorPair(int fillPercent) const {
    if (fillPercent >= 91) return colors.CRITICAL;
    if (fillPercent >= 76) return colors.DANGER;
    if (fillPercent >= 51) return colors.WARNING;
    return colors.SUCCESS;
}

std::string UIManager::formatDistance(int distance) const {
    return std::to_string(distance) + " units";
}

std::string UIManager::getProgressBar(int percent, int width) const {
    int filled = (percent * width) / 100;
    std::string bar;
    for (int i = 0; i < filled; i++) bar += "#";
    for (int i = filled; i < width; i++) bar += "-";
    return bar;
}

} // namespace project