/**
 * @file Bin.cpp
 * @brief Represents a garbage bin in the city.
 * @author Miray Duygulu
 * @date 2025-12-23
 */

#include "core/Bin.h"

namespace project {

// Default constructor
Bin::Bin()
    : id(""),
      location(""),
      capacity(0),
      currentFill(0),
      fillRate(0),
      nodeId(-1),
      historyIndex(0)
{
    for (int i = 0; i < 7; ++i) {
        fillHistory[i] = 0;
    }
}

// Constructor
Bin::Bin(const std::string& id,
         const std::string& location,
         int capacity,
         int currentFill,
         int fillRate,
         int nodeId)
    : id(id),
      location(location),
      capacity(capacity),
      currentFill(currentFill),
      fillRate(fillRate),
      nodeId(nodeId),
      historyIndex(0)
{
    
    for (int i = 0; i < 7; ++i) {     // Initialize fill history to 0
        fillHistory[i] = 0;
    }
}


void Bin::updateFill() {              // Updates the fill level for one day
    currentFill += fillRate;
                
    if (currentFill > capacity) {   
        currentFill = capacity;       //currentFill capacity ye eşitlenir çünkü
    }                                 //ne kadar taştığıyla ilgilenmiyoruz
                                      //eşikte olup olamadığına bakılıyor

   
    recordFillLevel(currentFill);      // Record today's fill level
}


void Bin::collect(int amount) {       // Empties the bin by a specified amount
    if (amount < 0) 
    return; 

    currentFill -= amount;

    if (currentFill < 0) {          //we convert negative numbers to zero
        currentFill = 0;
    }
}


void Bin::recordFillLevel(int fillLevel) {   // Records fill level into circular history buffer
    fillHistory[historyIndex] = fillLevel;
    historyIndex = (historyIndex + 1) % 7;   //for cycle
}

 
double Bin::getAverageFillRate() const {     // we calculate average fill rate
    int sum = 0;                            

    for (int i = 0; i < 7; ++i) {
        sum += fillHistory[i];
    }

    return sum / 7.0;
}


bool Bin::isOverflowing() const {           // we check if bin is overflowing
    return currentFill >= capacity;
}

// Getter methods

std::string Bin::getId() const 
{
    return id;
}

std::string Bin::getLocation() const 
{
    return location;
}

int Bin::getCurrentFill() const 
{
    return currentFill;
}

int Bin::getCapacity() const 
{
    return capacity;
}

int Bin::getFillRate() const 
{
    return fillRate;
}

int Bin::getNodeId() const 
{
    return nodeId;
}
// Set
void Bin::setCurrentFill(int fill) 
{
    currentFill = fill;
}

} // namespace project

