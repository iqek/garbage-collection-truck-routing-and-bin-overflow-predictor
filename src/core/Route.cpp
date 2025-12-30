/**
 * @file Route.cpp
 * @brief Skeleton implementation for Route class (logic left to student).
 */

#include "Route.h"

namespace project {


Route::Route()
{
    
    binIndices = nullptr;
    length = 0;
    totalDistance = 0;
    needsDisposal = false;
}

// ------------------------------------------------------------
// Parametrized Constructor
// ------------------------------------------------------------
// Amaç: dışarıdan verilen bins dizisini "deep copy" ile içeri almak.
// Neden deep copy?
// - bins pointer'ı dışarıya ait olabilir (stack, başka class).
// - Route kendi hafızasını yönetmeli.
// Yapılacaklar:
// - count <= 0 veya bins == nullptr ise boş route gibi davran.
// - değilse new int[count] açıp elemanları tek tek kopyala.
// - metadata (totalDistance, needsDisposal) başlangıç değerlerini ayarla.
// ------------------------------------------------------------

Route::Route(int* bins, int count)
{
    
    if(count <=0 || bins == nullptr){

        binIndices = nullptr;
        length = 0;
        totalDistance = 0;
        needsDisposal = false;
        return;
    }

    else{
        binIndices = new int[count]

        for()

    }
}

// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------
// Amaç: new[] ile açılan binIndices dizisini serbest bırakmak.
// - delete[] binIndices
// - (isteğe bağlı) pointer'ı nullptr yap
// - (isteğe bağlı) length/metadata sıfırla
// Not: nullptr üzerinde delete[] güvenlidir.
// ------------------------------------------------------------
Route::~Route()
{
    // TODO:
    // delete[] binIndices;
    // binIndices = nullptr;
    // length = 0; totalDistance = 0; needsDisposal = false;
}

// ------------------------------------------------------------
// Copy Constructor  (Rule of 3)
// ------------------------------------------------------------
// Amaç: Route a = b; gibi kopyalamalarda "deep copy" yapmak.
// Eğer shallow copy yaparsan:
// - iki Route aynı binIndices'i tutar
// - destructor iki kere delete[] eder -> crash (double delete)
// Yapılacaklar:
// - length, totalDistance, needsDisposal kopyalanır
// - other boşsa binIndices = nullptr
// - değilse new int[length] açıp elemanları kopyala
// ------------------------------------------------------------
Route::Route(const Route& other)
{
    // TODO:
    // length = other.length;
    // totalDistance = other.totalDistance;
    // needsDisposal = other.needsDisposal;
    //
    // if (other.binIndices == nullptr || other.length == 0) { binIndices = nullptr; return; }
    // binIndices = new int[length];
    // for (...) binIndices[i] = other.binIndices[i];
}

// ------------------------------------------------------------
// Assignment Operator  (Rule of 3)
// ------------------------------------------------------------
// Amaç: a = b; atamasında deep copy.
// Yapılacaklar sırasıyla:
// 1) self-assignment kontrolü: if (this == &other) return *this;
// 2) mevcut binIndices'i delete[] et (yoksa leak olur)
// 3) other'ın basit alanlarını kopyala
// 4) other boşsa binIndices = nullptr ve return *this
// 5) değilse new int[length] açıp elemanları kopyala
// ------------------------------------------------------------
Route& Route::operator=(const Route& other)
{
    // TODO:
    // if (this == &other) return *this;
    // delete[] binIndices; binIndices = nullptr;
    // length = other.length; totalDistance = other.totalDistance; needsDisposal = other.needsDisposal;
    // if (other.binIndices == nullptr || other.length == 0) { binIndices = nullptr; return *this; }
    // binIndices = new int[length];
    // for (...) binIndices[i] = other.binIndices[i];
    // return *this;

    return *this; // placeholder
}

// ------------------------------------------------------------
// addBin
// ------------------------------------------------------------
// Amaç: Route'un sonuna 1 bin index eklemek.
// Çünkü int* kullandığımız için "büyütme" manuel yapılır:
// - newLength = length + 1
// - new int[newLength] aç
// - eskileri kopyala
// - son elemana binIndex koy
// - delete[] eski dizi
// - binIndices = yeni dizi, length = newLength
// ------------------------------------------------------------
void Route::addBin(int binIndex)
{
    // TODO:
    // int newLength = length + 1;
    // int* newArray = new int[newLength];
    // for (...) newArray[i] = binIndices[i];
    // newArray[newLength - 1] = binIndex;
    // delete[] binIndices;
    // binIndices = newArray;
    // length = newLength;
}

// ------------------------------------------------------------
// getBinAt
// ------------------------------------------------------------
// Amaç: position'daki bin index'i döndürmek.
// Güvenlik önerisi:
// - binIndices nullptr ise veya position range dışıysa sentinel döndür (ör: -1)
// Alternatif: assert / exception (projeye göre).
// ------------------------------------------------------------
int Route::getBinAt(int position) const
{
    // TODO:
    // if (binIndices == nullptr) return -1;
    // if (position < 0 || position >= length) return -1;
    // return binIndices[position];

    return -1; // placeholder
}

// ------------------------------------------------------------
// getLength
// ------------------------------------------------------------
int Route::getLength() const
{
    // TODO: return length;
    return 0; // placeholder
}

// ------------------------------------------------------------
// setTotalDistance
// ------------------------------------------------------------
// totalDistance genelde RoutePlanner tarafından hesaplanıp set edilir.
// İstersen negatif distance gelirse 0'a çekebilirsin.
// ------------------------------------------------------------
void Route::setTotalDistance(int distance)
{
    // TODO:
    // if (distance < 0) distance = 0;
    // totalDistance = distance;
}

// ------------------------------------------------------------
// getTotalDistance
// ------------------------------------------------------------
int Route::getTotalDistance() const
{
    // TODO: return totalDistance;
    return 0; // placeholder
}

// ------------------------------------------------------------
// setNeedsDisposal
// ------------------------------------------------------------
// Route sırasında disposal (dump) uğraması gerekip gerekmediği.
// ------------------------------------------------------------
void Route::setNeedsDisposal(bool needs)
{
    // TODO: needsDisposal = needs;
}

// ------------------------------------------------------------
// requiresDisposal
// ------------------------------------------------------------
bool Route::requiresDisposal() const
{
    // TODO: return needsDisposal;
    return false; // placeholder
}

// ------------------------------------------------------------
// isEmpty
// ------------------------------------------------------------
bool Route::isEmpty() const
{
    // TODO: return (length == 0);
    return true; // placeholder
}

// ------------------------------------------------------------
// clear
// ------------------------------------------------------------
// Route'u sıfırlar:
// - delete[] binIndices
// - binIndices=nullptr
// - length=0
// - totalDistance=0
// - needsDisposal=false
// ------------------------------------------------------------
void Route::clear()
{
    // TODO:
    // delete[] binIndices;
    // binIndices = nullptr;
    // length = 0;
    // totalDistance = 0;
    // needsDisposal = false;
}

} // namespace project
