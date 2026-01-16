/**
 * @file Facility.cpp
 * @brief Represents a facility (depot or disposal site) in the city.
 * @author İlber Eren Tüt
 * @date 2026-01-10
 */

#include "core/Facility.h"

namespace project {

Facility::Facility()
    : id(""), type(""), x(0), y(0), nodeId(-1)  // geçersiz default
{}

Facility::Facility(const std::string& id, const std::string& type, int x, int y, int nodeId)
    : id(id), type(type), x(x), y(y), nodeId(nodeId) {
    // sınırlayıcı vs yok
}

bool Facility::isDepot() const {
    return type == "depot";
}

bool Facility::isDisposal() const {
    return type == "disposal";
}

std::string Facility::getId() const {
    return id;
}

std::string Facility::getType() const {
    return type;
}

int Facility::getX() const {
    return x;
}

int Facility::getY() const {
    return y;
}

int Facility::getNodeId() const {
    return nodeId;
}

}  // namespace project
