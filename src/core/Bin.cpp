/**
 * @file Bin.cpp
 * @brief Represents a garbage bin in the city.
 * @author İlber Eren TÜT
 * @date 2026-01-10
 */

#include "Bin.h"

namespace project {

Bin::Bin()
    : id(""),
      location(""),
      capacity(0),
      currentFill(0),
      fillRate(0),
      nodeId(-1),
      historyIndex(0) {
    for (int i = 0; i < 7; i=i+1){
        fillHistory[i] = 0;
    }
}    

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
    
    for (int i = 0; i < 7; i=i+1){
        fillHistory[i] = 0;
    }
}

//currentfil checki
void Bin::updateFill(){   

    currentFill = fillRate + currentFill;
                
    if (currentFill > capacity){   
        currentFill = capacity;       
    }                                 
                                      
    recordFillLevel(currentFill); 
}

//collect
void Bin::collect(int amount){   

    if (amount < 0) 
    return; 

    currentFill = currentFill - amount;

    if (currentFill < 0){          
        currentFill = 0;
    }
}


void Bin::recordFillLevel(int fillLevel){

    fillHistory[historyIndex] = fillLevel;
    historyIndex = (historyIndex + 1) % 7;   
}

 //aver fill ratei
double Bin::getAverageFillRate() const{
    int sum = 0;                            

    for (int i = 0; i < 7; i=i+1){
        sum = sum + fillHistory[i];
    }

    return sum / 7.0;
}

//fonklar
bool Bin::isOverflowing() const{
    return currentFill >= capacity;
}


//getterlar
std::string Bin::getId() const{
    return id;
}

std::string Bin::getLocation() const{
    return location;
}

int Bin::getCurrentFill() const{
    return currentFill;
}

int Bin::getCapacity() const{
    return capacity;
}

int Bin::getFillRate() const{
    return fillRate;
}

int Bin::getNodeId() const{
    return nodeId;
}

} // namespace project

