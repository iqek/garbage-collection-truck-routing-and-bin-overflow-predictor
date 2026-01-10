/**
 * @file Simulation.cpp
 * @brief Implementation of Simulation class.
 * @author Kerem Akdeniz
 * @date 2025-12-28
 */

#include "core/Simulation.h"
#include <iostream>
#include <climits>

namespace project {

// Constructor
Simulation::Simulation(Graph& graph, Facilities& facilities, int duration)                             
    : graph(graph), facilities(facilities), planner(graph), currentTime(0), maxTime(duration),         // Simulation.h'ten gelen private ve public değişkenler. Tüm değerleri initalize ettik.
      overflowCount(0), totalDistance(0), collectionsCompleted(0) {     
}

// step(), günlük yapılacak işlemler
void Simulation::step() {
    // 1. Update all bin fill levels
    for (int i = 0; i < facilities.getBinCount(); i++) {
        facilities.getBin(i).updateFill();                                              //fill level'ı güncelle (tüm bin'ler için)
        facilities.getBin(i).recordFillLevel(facilities.getBin(i).getCurrentFill());    // 2. Record fill history for predictions
    }

    // 2.1 Overflow check (başlamadan önce)
    checkOverflows();

    // 3. Plan collection route
    // Save bin states 
    int* savedFills = new int[facilities.getBinCount()];
    for(int i = 0; i < facilities.getBinCount(); i++){
        savedFills[i] = facilities.getBin(i).getCurrentFill();
    }
    int savedTruckLoad = facilities.getTruck().getCurrentLoad();
    int savedTruckNode = facilities.getTruck().getCurrentNode();
    Route plannedroute = planner.planRoute(facilities);     //.planRoute kullanarak "plannedroute" oluşturduk
    // Restore bin states
    for(int i = 0; i < facilities.getBinCount(); i++){
        int currentFill = facilities.getBin(i).getCurrentFill();
        int toRestore = savedFills[i] - currentFill;
        if(toRestore != 0){
            // Manually restore the fill level
            facilities.getBin(i).setCurrentFill(savedFills[i]);
        }
    }
    delete[] savedFills;
    
    // Restore truck state
    facilities.getTruck().setCurrentLoad(savedTruckLoad);
    facilities.getTruck().moveTo(savedTruckNode);

    // 4. Execute truck movements and collections
    Truck& truck = facilities.getTruck();
    int currentLocation = truck.getCurrentNode();           //currentNode demek yerine currentLocation

    for (int i = 0; i < plannedroute.getLength(); i++) {
        int binNum = plannedroute.getBinAt(i);
        Bin& bin = facilities.getBin(binNum);

        // 4.1 Bin'e git
        int binLocation = bin.getNodeId();                  //binNode demek yerine binLocation
        int distance = planner.computeDistance(currentLocation, binLocation);
        if (distance != INT_MAX && distance > 0) {
            totalDistance += distance;
        }
        truck.moveTo(binLocation);
        currentLocation = binLocation;              //binLocation'a geldik

        // 4.2 Collect
        int garbageAmount = bin.getCurrentFill();
        int remainingCapacity = truck.getRemainingCapacity();

        if (garbageAmount > remainingCapacity) {    //limiti aşıyorsa
            garbageAmount = remainingCapacity;     //garbageAmount artık = remainingCapacity ki alınabilecek maksimum değer olsun ve bir sonraki if statement'ta toplanabilsin
        }

        if (garbageAmount > 0 && garbageAmount <= remainingCapacity) { //eğer atık var ve sınırı aşmıyor ise
            truck.collect(garbageAmount);         //topla
            bin.collect(garbageAmount);          //toplanan atık kadar bin'den çıkar
            collectionsCompleted++;             //toplama sayacını arttır
        }

        // 5. Handle disposal trips when truck is full
        if (truck.isFull()) {
            
            int disposalLocation = planner.findNearestDisposal(currentLocation, facilities);
            if (disposalLocation != -1) { //RoutePlanner.h'teki line 70'e referans
                distance = planner.computeDistance(currentLocation, disposalLocation);
                if (distance != INT_MAX && distance > 0) {
                    totalDistance += distance;
                }
                truck.moveTo(disposalLocation);
                truck.unload();
                currentLocation = disposalLocation;
            }
        }
    }

    // Depot'a dön
    int depotLocation = facilities.getDepotNode();
    if (currentLocation != depotLocation) {
        int distance = planner.computeDistance(currentLocation, depotLocation);
        if (distance != INT_MAX && distance > 0) {
            totalDistance += distance;
        }
        truck.moveTo(depotLocation);
    }

    // 6. Track overflow events
    if (planner.hasCriticalBins(facilities)) {
        handleEmergencyReschedule();
    }

    currentTime++;   //günlük mesai bitişi, günü bir arttır
}

// Simülasyonu başlat
void Simulation::run() {
    while (!isFinished()) {
        step();
    }
}

// Bitti mi? Check
bool Simulation::isFinished() const {
    return currentTime >= maxTime;
}

// Current time getter
int Simulation::getTime() const {
    return currentTime;
}

// Max time getter
int Simulation::getMaxTime() const {
    return maxTime;
}

// Facilities getter
Facilities& Simulation::getFacilities() {
    return facilities;
}

// Overflow check
void Simulation::checkOverflows() {
    for (int i = 0; i < facilities.getBinCount(); i++) {
        if (facilities.getBin(i).isOverflowing()) {
            overflowCount++;
        }
    }
}

// Emergency reschedule
    /**
     * (reference to Simulation.h) Handles dynamic rescheduling when critical bins detected.
     * 
     * Called when sensor data indicates unexpected rapid filling.
     * Adjusts current route to prioritize critical bins.
     */

void Simulation::handleEmergencyReschedule() {
    //Plan a new route
    Route emergencyRoute = planner.planRoute(facilities);

    Truck& truck = facilities.getTruck();
    int currentLocation = truck.getCurrentNode();

    for (int i = 0; i < emergencyRoute.getLength(); i++) {
        int binIndex = emergencyRoute.getBinAt(i);
        Bin& bin = facilities.getBin(binIndex);

        //Overflowing'leri bul ve topla
        if (bin.isOverflowing()) {
            int overflowingbinLocation = bin.getNodeId();
            int distance = planner.computeDistance(currentLocation, overflowingbinLocation);
            totalDistance += distance;
            truck.moveTo(overflowingbinLocation);

            int garbageAmount = bin.getCurrentFill();
            int remainingCapacity = truck.getRemainingCapacity();

            if (garbageAmount > remainingCapacity) {
                garbageAmount = remainingCapacity;
            }

            if (garbageAmount > 0) {
                truck.collect(garbageAmount);
                bin.collect(garbageAmount);
                collectionsCompleted++;
            }

            currentLocation = overflowingbinLocation;

            // Truck full olunca boşalt
            if (truck.isFull()) {
                int disposalLocation = planner.findNearestDisposal(currentLocation, facilities);
                if (disposalLocation != -1) {
                    distance = planner.computeDistance(currentLocation, disposalLocation);
                    totalDistance += distance;
                    truck.moveTo(disposalLocation);
                    truck.unload();
                    currentLocation = disposalLocation;
                }
            }
        }
    }
}

// Performance
int Simulation::getOverflowCount() const {
    return overflowCount;
}

int Simulation::getTotalDistance() const {
    return totalDistance;
}

int Simulation::getCollectionsCompleted() const {
    return collectionsCompleted;
}

// Print statistics
void Simulation::printStatistics() const {
    std::cout << "======= Simulation Statistics =======\n";
    std::cout << "Simulation Duration: " << maxTime << " days\n";
    std::cout << "Distance Traveled: " << totalDistance << " units\n";
    std::cout << "Overflow Event(s): " << overflowCount << std::endl;
    std::cout << "Collections Completed: " << collectionsCompleted << std::endl;
    std::cout << "Average Distance per Day: " << (maxTime > 0 ? totalDistance / maxTime : 0) << " units\n";
    std::cout << "Average Collections per Day: " << (maxTime > 0 ? collectionsCompleted / maxTime : 0)  << std::endl;
    std::cout << "=====================================\n";
}

} // namespace project