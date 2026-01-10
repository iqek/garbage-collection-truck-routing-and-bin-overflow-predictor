/**
 * @file Route.h
 * @brief Represents a collection route with sequence of bins to visit.
 * @author İlber Eren Tüt
 * @date 2025-01-10
 */

#include "core/Route.h"

namespace project {

Route::Route(){

    binIndices = nullptr;
    length = 0;
    totalDistance = 0;
    needsDisposal = false;
}

Route::Route(int* bins, int count){

    //Binler yoksa checki
    if (bins == nullptr || count <= 0){

        binIndices = nullptr;
        length = 0;
        totalDistance = 0;
        needsDisposal = false;
        return;
    }

    totalDistance = 0;
    needsDisposal = false;

    //Binin dışardakı pointerını bişeler yapmamak için kopyalıcaz içte

    length=count;
    binIndices = new int[length];
    for (int i = 0; i < length;i=i+1){
        binIndices[i] = bins[i];
    }
}

Route::~Route(){

    //memory leak engellemek içn
    delete[] binIndices;
    length = 0;
    totalDistance = 0;
    needsDisposal = false;
}

Route::Route(const Route& other){

    length = other.length;
    totalDistance = other.totalDistance;
    needsDisposal = other.needsDisposal;

    // other boşalınca biz de boş olalım
    if (other.binIndices == nullptr || other.length == 0){
        binIndices = nullptr;
        return;
    }

    binIndices = new int[length];

    for (int i = 0; i < length;i=i+1){
        binIndices[i] = other.binIndices[i];
    }
}




Route& Route::operator=(const Route& other){

    //self assingla alakalı hata
    if (this == &other){
        return *this;
    }

    //Mem leak temizliği
    delete[] binIndices;
    binIndices = nullptr;

    //kopyalama
    length = other.length;
    totalDistance = other.totalDistance;
    needsDisposal = other.needsDisposal;

    //boşalınca bitir
    if (other.binIndices == nullptr || other.length == 0){
        binIndices = nullptr;
        return *this;
    }

    //kopyala
    binIndices = new int[length];
    for (int i = 0; i < length; i++){
        binIndices[i] = other.binIndices[i];
    }



    return *this;
}



void Route::addBin(int binIndex){

    //vektör yok diye arrayden geçircez
    int newLength = length + 1;

    int* newArray = new int[newLength];

    for (int i = 0; i < length; i=i+1){

        newArray[i] = binIndices[i];
    }

    newArray[length] = binIndex;

    delete[] binIndices;

    binIndices = newArray;
    length = newLength;
}

int Route::getBinAt(int position) const{

    //uzaklık ve bulunma kontrol
    if (binIndices == nullptr){
        return -1;
    }

    if (position < 0 || position >= length){
        return -1;
    }

    return binIndices[position];
}

//fonklar
int Route::getLength() const{

    return length;
}

void Route::setTotalDistance(int distance){

    totalDistance = distance;
}

int Route::getTotalDistance() const{

    return totalDistance;
}

void Route::setNeedsDisposal(bool needs){

    needsDisposal = needs;
}

bool Route::requiresDisposal() const{

    return needsDisposal;
}

bool Route::isEmpty() const{

    return (length == 0);
}

void Route::clear(){

    delete[] binIndices;
    binIndices = nullptr;
    length = 0;
    totalDistance = 0;
    needsDisposal = false;
}

} // namespace project

