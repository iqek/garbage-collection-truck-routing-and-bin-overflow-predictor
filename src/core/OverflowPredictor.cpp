/**
 * @file OwerflowPredictor.cpp
 * @brief Implementation of OwerflowPredictor class.
 * @author Miray Duygulu
 * @date 2025-12-27
 */
#include "core/OverflowPredictor.h"
#include <climits>


namespace project {


// Constructor
OverflowPredictor::OverflowPredictor(int threshold)
    : criticalThreshold(threshold) {
    // Threshold represents the number of days under which
    // a bin is considered critical.
}

// Predict days until overflow
int OverflowPredictor::predictDaysToOverflow(const Bin& bin) const {
    int capacity = bin.getCapacity();           //Bin’in maksimum kapasitesi
    int currentFill = bin.getCurrentFill();     //Şu anki doluluk miktarı

    
    if (bin.isOverflowing()) {  // If bin is already full 
        return -1;
    }

    // Bin’in geçmiş verilerden hesaplanan ortalama dolma hızı
    double fillRate = bin.getAverageFillRate();

    if (fillRate <= 0) {
        // Fallback: use current fill as a rough estimation
        // This avoids division by zero and missing data
        fillRate = bin.getFillRate();
    }

    // Precondition safety check
    if (fillRate <= 0) {
        // Cannot predict overflow without a valid fill rate
        return INT_MAX; //infinity value
    }

    // Formula from header documentation:
    // (capacity - currentFill) / effectiveFillRate
    double remaining = capacity - currentFill;
    int days = static_cast<int>(remaining / fillRate);
    if (days < 1) days = 1;
    
    return days;
}

// Check if bin is critical
bool OverflowPredictor::isCritical(const Bin& bin) const {
    int days = predictDaysToOverflow(bin);

    // If already overflowing, treat as critical
    if (days == -1) {
        return true;
    }

    if (days <= criticalThreshold) {
    // Overflow is expected within the critical threshold
        return true;
    }
    return false;
    }


// Get overflow risk score
double OverflowPredictor::getOverflowRisk(const Bin& bin) const {
    int days = predictDaysToOverflow(bin);

    // Overflowing bins get highest priority (lowest score)
    if (days == -1) {
        return 0.0;
    }

    // Lower days => higher priority
    return static_cast<double>(days);
}

/*
 * Alternative:
 * Risk score could be inverted or weighted:
 * risk = 1.0 / (days + 1)
 * This would exaggerate urgency differences for small values.
 */

// Update critical threshold
void OverflowPredictor::setCriticalThreshold(int threshold) {
    criticalThreshold = threshold;
}

} // namespace project
