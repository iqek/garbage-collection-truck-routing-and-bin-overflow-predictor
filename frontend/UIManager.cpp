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
    // Only refresh window panels in NORMAL mode
    if (viewMode == ViewMode::NORMAL) {
        windows.refreshAll();
    }
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
    int boxHeight = 18;  // Increased to fit 7 days of history
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
    
    mvprintw(startY + 7, startX + 2, "FILL HISTORY (Last 7 days)");
    
    // Draw fill history as a list with bars
    const int* history = bin.getFillHistory();
    int capacity = bin.getCapacity();
    int currentDay = simulation.getTime();
    
    // Note: Initial state (Day 0) is now recorded in constructor
    // Day 0 → history[0], Day 1 → history[1], etc. (with circular wraparound after 7 days)
    
    // Calculate which days to show (last 7 or fewer)
    int startDay = (currentDay >= 6) ? currentDay - 6 : 0;
    int endDay = currentDay;
    
    for (int i = 0; i < 7; i++) {
        int row = startY + 8 + i;
        int simDay = startDay + i;
        
        if (simDay <= endDay) {
            // Day N's data is at history index N % 7
            int actualIdx = simDay % 7;
            int fillValue = history[actualIdx];
            
            // Draw day label and value
            if (simDay == currentDay) {
                attron(COLOR_PAIR(colors.WARNING) | A_BOLD);
                mvprintw(row, startX + 2, "D%d*: %3d/%3d ", 
                         simDay, fillValue, capacity);
                attroff(COLOR_PAIR(colors.WARNING) | A_BOLD);
            } else {
                mvprintw(row, startX + 2, "D%-2d: %3d/%3d ", 
                         simDay, fillValue, capacity);
            }
            
            // Draw horizontal bar (15 chars wide)
            int barWidth = 15;
            int barFilled = (capacity > 0) ? (fillValue * barWidth) / capacity : 0;
            if (barFilled > barWidth) barFilled = barWidth;
            
            printw("[");
            for (int j = 0; j < barWidth; j++) {
                if (j < barFilled) {
                    printw("#");
                } else {
                    printw("-");
                }
            }
            printw("]");
        } else {
            // Empty row for days that haven't happened yet
            mvprintw(row, startX + 2, "                                ");
        }
    }
    
    mvprintw(startY + boxHeight - 2, startX + 2, "[Enter] Close  [ESC] Back");
    
    refresh();
}

void UIManager::drawFullMap() {
    werase(stdscr);
    
    // Draw header
    attron(COLOR_PAIR(colors.HEADER) | A_BOLD);
    mvprintw(0, 0, "FULL MAP VIEW - Press [ESC] to return");
    attroff(COLOR_PAIR(colors.HEADER) | A_BOLD);
    
    Facilities& facilities = simulation.getFacilities();
    Truck& truck = facilities.getTruck();
    
    // Draw legend
    int row = 2;
    mvprintw(row++, 2, "Legend:");
    attron(COLOR_PAIR(colors.SUCCESS));
    mvprintw(row++, 4, "[D]   - Depot");
    attroff(COLOR_PAIR(colors.SUCCESS));
    
    attron(COLOR_PAIR(colors.DANGER));
    mvprintw(row++, 4, "[DS]  - Disposal Site");
    attroff(COLOR_PAIR(colors.DANGER));
    
    attron(COLOR_PAIR(colors.INFO));
    mvprintw(row++, 4, "[B#]  - Bin (# = bin number)");
    attroff(COLOR_PAIR(colors.INFO));
    
    attron(COLOR_PAIR(colors.WARNING));
    mvprintw(row++, 4, "[T]   - Truck");
    attroff(COLOR_PAIR(colors.WARNING));
    
    mvprintw(row++, 4, "----  - Road connection");
    
    row += 2;
    
    // Draw city map (same as small map but larger)
    mvprintw(row++, 10, "    D ---- B1");
    mvprintw(row++, 10, "    |      |");
    mvprintw(row++, 10, "    B2 - DS1");
    mvprintw(row++, 10, "    |  /  |");
    mvprintw(row++, 10, "    B3----B4");
    mvprintw(row++, 10, "      \\");
    mvprintw(row++, 10, "        B5");
    
    row += 2;
    
    // Draw facility details
    mvprintw(row++, 2, "Facilities:");
    Facility* facilityArray = facilities.getFacilities();
    for (int i = 0; i < facilities.getFacilityCount(); i++) {
        Facility& facility = facilityArray[i];
        int colorPair = (facility.getType() == "depot") ? colors.SUCCESS : colors.DANGER;
        attron(COLOR_PAIR(colorPair));
        mvprintw(row++, 4, "%s - %s (Node %d)", 
                 facility.getId().c_str(), 
                 facility.getType().c_str(),
                 facility.getNodeId());
        attroff(COLOR_PAIR(colorPair));
    }
    
    row += 1;
    
    // Draw bin details
    mvprintw(row++, 2, "Bins:");
    for (int i = 0; i < facilities.getBinCount(); i++) {
        Bin& bin = facilities.getBin(i);
        int fillPercent = getBinFillPercent(bin);
        int colorPair = getBinColorPair(fillPercent);
        
        attron(COLOR_PAIR(colorPair));
        mvprintw(row++, 4, "%s - %s (Node %d) [%d/%d] %d%%", 
                 bin.getId().c_str(),
                 bin.getLocation().c_str(),
                 bin.getNodeId(),
                 bin.getCurrentFill(),
                 bin.getCapacity(),
                 fillPercent);
        attroff(COLOR_PAIR(colorPair));
    }
    
    row += 1;
    
    // Draw truck details
    mvprintw(row++, 2, "Truck:");
    int loadPercent = (truck.getCurrentLoad() * 100) / truck.getCapacity();
    attron(COLOR_PAIR(colors.WARNING));
    mvprintw(row++, 4, "%s - Node %d [%d/%d] %d%%", 
             truck.getId().c_str(),
             truck.getCurrentNode(),
             truck.getCurrentLoad(),
             truck.getCapacity(),
             loadPercent);
    attroff(COLOR_PAIR(colors.WARNING));
    
    refresh();
}

void UIManager::drawHistory() {
    werase(stdscr);
    
    // Draw header
    attron(COLOR_PAIR(colors.HEADER) | A_BOLD);
    mvprintw(0, 0, "SIMULATION HISTORY - Press [ESC] to return");
    attroff(COLOR_PAIR(colors.HEADER) | A_BOLD);
    
    Facilities& facilities = simulation.getFacilities();
    int row = 2;
    
    // Simulation Progress
    attron(COLOR_PAIR(colors.INFO) | A_BOLD);
    mvprintw(row++, 2, "SIMULATION PROGRESS");
    attroff(COLOR_PAIR(colors.INFO) | A_BOLD);
    row++;
    
    mvprintw(row++, 4, "Current Day: %d / %d", simulation.getTime(), simulation.getMaxTime());
    mvprintw(row++, 4, "Progress: [");
    int progressWidth = 40;
    int filled = (simulation.getTime() * progressWidth) / simulation.getMaxTime();
    for (int i = 0; i < progressWidth; i++) {
        if (i < filled) {
            attron(COLOR_PAIR(colors.SUCCESS));
            addch('=');
            attroff(COLOR_PAIR(colors.SUCCESS));
        } else {
            addch('-');
        }
    }
    printw("] %.1f%%", (simulation.getTime() * 100.0) / simulation.getMaxTime());
    row += 2;
    
    // Performance Statistics
    attron(COLOR_PAIR(colors.INFO) | A_BOLD);
    mvprintw(row++, 2, "PERFORMANCE STATISTICS");
    attroff(COLOR_PAIR(colors.INFO) | A_BOLD);
    row++;
    
    mvprintw(row++, 4, "Total Distance Traveled: %d units", simulation.getTotalDistance());
    mvprintw(row++, 4, "Collections Completed: %d", simulation.getCollectionsCompleted());
    
    int overflows = simulation.getOverflowCount();
    if (overflows > 0) {
        attron(COLOR_PAIR(colors.DANGER));
        mvprintw(row++, 4, "Overflow Events: %d", overflows);
        attroff(COLOR_PAIR(colors.DANGER));
    } else {
        attron(COLOR_PAIR(colors.SUCCESS));
        mvprintw(row++, 4, "Overflow Events: 0 (Perfect!)");
        attroff(COLOR_PAIR(colors.SUCCESS));
    }
    row += 2;
    
    // Current Bin Status
    attron(COLOR_PAIR(colors.INFO) | A_BOLD);
    mvprintw(row++, 2, "CURRENT BIN STATUS");
    attroff(COLOR_PAIR(colors.INFO) | A_BOLD);
    row++;
    
    for (int i = 0; i < facilities.getBinCount(); i++) {
        Bin& bin = facilities.getBin(i);
        int fillPercent = getBinFillPercent(bin);
        int colorPair = getBinColorPair(fillPercent);
        
        attron(COLOR_PAIR(colorPair));
        mvprintw(row++, 4, "%s: [%d/%d] %3d%% ", 
                 bin.getId().c_str(),
                 bin.getCurrentFill(),
                 bin.getCapacity(),
                 fillPercent);
        
        // Draw fill bar
        int barWidth = 30;
        printw("[");
        int barFilled = (fillPercent * barWidth) / 100;
        for (int j = 0; j < barWidth; j++) {
            if (j < barFilled) {
                addch('#');
            } else {
                addch('-');
            }
        }
        printw("]");
        attroff(COLOR_PAIR(colorPair));
    }
    
    refresh();
}

void UIManager::drawConfig() {
    werase(stdscr);
    
    // Draw header
    attron(COLOR_PAIR(colors.HEADER) | A_BOLD);
    mvprintw(0, 0, "CONFIGURATION - Press [ESC] to return");
    attroff(COLOR_PAIR(colors.HEADER) | A_BOLD);
    
    Facilities& facilities = simulation.getFacilities();
    Truck& truck = facilities.getTruck();
    
    int row = 2;
    
    // Simulation Settings
    attron(COLOR_PAIR(colors.INFO) | A_BOLD);
    mvprintw(row++, 2, "SIMULATION SETTINGS");
    attroff(COLOR_PAIR(colors.INFO) | A_BOLD);
    row++;
    
    mvprintw(row++, 4, "Total Duration: %d days", simulation.getMaxTime());
    mvprintw(row++, 4, "Current Day: %d", simulation.getTime());
    mvprintw(row++, 4, "Simulation Speed: %.1fx", speedMultiplier);
    row++;
    
    // Truck Configuration
    attron(COLOR_PAIR(colors.INFO) | A_BOLD);
    mvprintw(row++, 2, "TRUCK CONFIGURATION");
    attroff(COLOR_PAIR(colors.INFO) | A_BOLD);
    row++;
    
    mvprintw(row++, 4, "ID: %s", truck.getId().c_str());
    mvprintw(row++, 4, "Capacity: %d units", truck.getCapacity());
    mvprintw(row++, 4, "Now at Node: %d", truck.getCurrentNode());
    row++;
    
    // Bin Configuration
    attron(COLOR_PAIR(colors.INFO) | A_BOLD);
    mvprintw(row++, 2, "BIN CONFIGURATION (%d bins)", facilities.getBinCount());
    attroff(COLOR_PAIR(colors.INFO) | A_BOLD);
    row++;
    
    mvprintw(row++, 4, "%-4s %-12s %-8s %-8s %-8s %-6s", 
             "ID", "Location", "Capacity", "Initial", "FillRate", "Node");
    mvprintw(row++, 4, "------------------------------------------------------------");
    
    for (int i = 0; i < facilities.getBinCount(); i++) {
        Bin& bin = facilities.getBin(i);
        
        mvprintw(row++, 4, "%-4s %-12s %-8d %-8d %-8d %-6d",
                 bin.getId().c_str(),
                 bin.getLocation().c_str(),
                 bin.getCapacity(),
                 bin.getInitialFill(),
                 bin.getFillRate(),
                 bin.getNodeId());
    }
    
    row += 2;
    
    // Facilities
    attron(COLOR_PAIR(colors.INFO) | A_BOLD);
    mvprintw(row++, 2, "FACILITIES (%d facilities)", facilities.getFacilityCount());
    attroff(COLOR_PAIR(colors.INFO) | A_BOLD);
    row++;
    
    Facility* facilityArray = facilities.getFacilities();
    for (int i = 0; i < facilities.getFacilityCount(); i++) {
        Facility& facility = facilityArray[i];
        int colorPair = (facility.getType() == "depot") ? colors.SUCCESS : colors.DANGER;
        attron(COLOR_PAIR(colorPair));
        mvprintw(row++, 4, "%s (%s) - Node %d", 
                 facility.getId().c_str(),
                 facility.getType().c_str(),
                 facility.getNodeId());
        attroff(COLOR_PAIR(colorPair));
    }
    
    row += 2;
    
    // Controls
    attron(COLOR_PAIR(colors.WARNING));
    mvprintw(row++, 2, "CONTROLS:");
    attroff(COLOR_PAIR(colors.WARNING));
    mvprintw(row++, 4, "[P] Play/Pause  [S] Step  [R] Reset  [+/-] Speed");
    mvprintw(row++, 4, "[V] Full Map  [H] History  [C] Config  [Q] Quit");
    
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
    simulation.reset();
    state = SimulationState::PAUSED;
    // Don't reset selectedBinIndex - keep user on the same bin they were viewing
    binListScrollOffset = 0;
    setStatusMessage("Simulation reset to initial state - Press [P] to start", 120);
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