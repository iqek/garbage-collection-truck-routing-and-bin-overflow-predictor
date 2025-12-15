/**
 * @file OverflowPredictor.h
 * @brief Estimates garbage bin overflow times and risks.
 * @author Miray Duygulu, Kerem Akdeniz, İlber Eren Tüt, İrem Irmak Ünlüer, İpek Çelik
 * @date 2025-12-15
 */

#pragma once

#include "Bin.h"

namespace project {

/**
 * @brief Class for estimating the overflow risk of garbage bins.
 *
 * Uses a simple mathematical model (remaining capacity / fill rate) to
 * project when a bin will overflow. This prediction provides a critical
 * metric for routing priority decisions.
 */
class OverflowPredictor {
public:
    /**
     * @brief Predicts the number of days remaining until the bin overflows.
     *
     * The prediction is typically based on the formula:
     * `(Capacity - CurrentFill) / FillRate`.
     * @param bin The bin object to evaluate.
     * @pre The bin's daily fill rate must be greater than zero.
     * @return The estimated number of days until the bin reaches full capacity.
     */
    int predictDaysToOverflow(const Bin& bin) const;

    /**
     * @brief Checks if a bin is under critical overflow risk.
     * @param bin The bin object to evaluate.
     * @return `true` if the predicted time to overflow is below a defined critical threshold.
     */
    bool isCritical(const Bin& bin) const;
};

} // namespace project