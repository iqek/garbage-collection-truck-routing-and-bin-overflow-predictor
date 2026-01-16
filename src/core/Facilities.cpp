
/**
 * @file Facilites.cpp
 * @brief Manages all physical facilities (Bins, Truck, and Facilities) in the system.
 * @author İlber Eren TÜT
 * @date 2026.01.10
 */

#include "core/Facilities.h"

namespace project {

Facilities::Facilities() {
    bins = nullptr;
    binCount = 0;
    facilities = nullptr;
    facilityCount = 0;
}

Facilities::~Facilities() {
    delete[] bins;
    delete[] facilities;
}

void Facilities::addBin(const Bin& bin) {
    // Binler 1 büyütülecek
    Bin* bigger = new Bin[binCount + 1];

    for (int i = 0; i < binCount; i = i + 1) {
        bigger[i] = bins[i];
    }

    bigger[binCount] = bin;
    delete[] bins;
    bins = bigger;
    binCount = binCount + 1;
}

void Facilities::addFacility(const Facility& facility) {
    // Facility sayısını arttırır
    Facility* bigger = new Facility[facilityCount + 1];

    for (int i = 0; i < facilityCount; i = i + 1) {
        bigger[i] = facilities[i];
    }

    bigger[facilityCount] = facility;
    delete[] facilities;
    facilities = bigger;
    facilityCount = facilityCount + 1;
}

Bin* Facilities::getBins() {
    return bins;
}

Bin& Facilities::getBin(int index) const {
    return bins[index];
}

int Facilities::getBinCount() const {
    return binCount;
}

Facility* Facilities::getFacilities() {
    return facilities;
}

int Facilities::getFacilityCount() const {
    return facilityCount;
}

// depo idyi çevir yoksa error
int Facilities::getDepotNode() const {
    for (int i = 0; i < facilityCount; i = i + 1) {
        if (facilities[i].isDepot()) {
            return facilities[i].getNodeId();
        }
    }
    return -1;
}

// disposal id toplama
// plannerda delete sonra
int* Facilities::getDisposalNodes(int& count) const {
    count = 0;

    for (int i = 0; i < facilityCount; i = i + 1) {
        if (facilities[i].isDisposal()) {
            count = count + 1;
        }
    }

    if (count == 0) {
        return nullptr;
    }

    int* result = new int[count];
    int k = 0;

    for (int i = 0; i < facilityCount; i++) {
        if (facilities[i].isDisposal()) {
            result[k] = facilities[i].getNodeId();
            k = k + 1;
        }
    }

    return result;
}

void Facilities::setTruck(const Truck& truck) {
    this->truck = truck;
}

Truck& Facilities::getTruck() {
    return truck;
}

}  // namespace project
