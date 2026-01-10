/**
 * @file OverflowPredictor.h
 * @brief Estimates garbage bin overflow times and risks using historical data.
 * @author Miray Duygulu, Kerem Akdeniz, İlber Eren Tüt, İrem Irmak Ünlüer, İpek Çelik
 * @date 2025-12-23
 */

#pragma once

#include "core/Bin.h"

namespace project {

/**
 * @brief Class for estimating the overflow risk of garbage bins.
 *
 * Uses historical fill rate data and current state to predict when bins
 * will overflow. Provides critical metrics for routing priority decisions.
 */
class OverflowPredictor {
private:
    int criticalThreshold;  // Days - if predicted overflow < this, bin is critical

public:
    /**
     * @brief Constructs an overflow predictor.
     * @param threshold Number of days to consider critical (default 2).
     */
    explicit OverflowPredictor(int threshold = 2);

    /**
     * @brief Predicts the number of days remaining until the bin overflows.
     *
     * Uses the bin's average historical fill rate if available, otherwise
     * falls back to the current fill rate.
     * Formula: (Capacity - CurrentFill) / EffectiveFillRate
     * @param bin The bin object to evaluate.
     * @pre The bin's fill rate must be greater than zero.
     * @return The estimated number of days until the bin reaches full capacity.
     *         Returns -1 if already overflowing.
     */
    int predictDaysToOverflow(const Bin& bin) const;

    /**
     * @brief Checks if a bin is under critical overflow risk.
     * @param bin The bin object to evaluate.
     * @return `true` if predicted overflow is within critical threshold days.
     */
    bool isCritical(const Bin& bin) const;

    /**
     * @brief Calculates overflow risk score for routing decisions.
     * 
     * Lower score = higher priority (more urgent).
     * @param bin The bin to evaluate.
     * @return Risk score (days to overflow, lower is more urgent).
     */
    double getOverflowRisk(const Bin& bin) const;

    /**
     * @brief Updates the critical threshold.
     * @param threshold New threshold in days.
     */
    void setCriticalThreshold(int threshold);
};

} // namespace project