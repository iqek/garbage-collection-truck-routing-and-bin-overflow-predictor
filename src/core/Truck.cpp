/**
 * @file Truck.h
 * @brief Garbage collection truck and its basic operations.
 * @author İlber Eren Tüt
 * @date 2025-01-10
 */

#include "core/Truck.h"

namespace project {

Truck::Truck(const std::string& id, int capacity, int initialLoad, int startNode)
    : id(id), capacity(capacity), load(initialLoad), currentNode(startNode)
{
    
    //Load sınırlayıcısı

    if(load < 0){
        load=0;
    }

    if(load > capacity){
        load=capacity;
    }
}

Truck::Truck() : id(""), capacity(0), load(0), currentNode(0) {}

void Truck::moveTo(int node) {

    //Truckın hareket fonk.

    currentNode = node;
}

void Truck::collect(int amount) {
    
    //Burda kısmi mi eklicez yoksa hepsini ekleyip mi devam edicez ona sonra karar verip dönücem
    //Collectleme fonk.

    if (amount < 0) return;

    if (load + amount > capacity) return;

    load = load + amount;
}

void Truck::unload() {
    load = 0;
}

int Truck::getRemainingCapacity() const {
    return capacity - load;
}

bool Truck::isFull() const {
    return load >= capacity;
}

//Returnler
std::string Truck::getId() const { return id; }
int Truck::getLoad() const { return load; }
int Truck::getCurrentLoad() const { return load; }
int Truck::getCapacity() const { return capacity; }
int Truck::getCurrentNode() const { return currentNode; }

// Setter methods
void Truck::setCurrentLoad(int newLoad) { 
    load = newLoad; 
}

}


