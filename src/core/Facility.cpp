/**
 * @file Bin.cpp
 * @brief Represents a garbage bin in the city.
 * @author İlber Eren TÜT
 * @date 2025.12.30
 */

#include "Facility.h"

namespace project {

Facility::Facility()
    : id(""),
      type(""),
      x(0),
      y(0),
      nodeId(-1) //geçersiz default
{

}



Facility::Facility(const std::string& id,
                   const std::string& type,
                   int x,
                   int y,
                   int nodeId)
    : id(id),
      type(type),
      x(x),
      y(y),
      nodeId(nodeId)
{
    //sınırlayıcı vs yok
}

bool Facility::isDepot() const{
    return type == "depot";
}

bool Facility::isDisposal() const{
    return type == "disposal";
}

std::string Facility::getId() const{
    return id;
}

std::string Facility::getType() const{
    return type;
}

int Facility::getX() const{
    return x;
}

int Facility::getY() const{
    return y;
}

int Facility::getNodeId() const{
    return nodeId;
}

} // namespace project
