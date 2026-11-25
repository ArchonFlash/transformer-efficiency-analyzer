#pragma once
#include <string>
#include "transformer_core.hpp"

// ============================================================================
// MEMBER 2: INPUT VALIDATION ENGINE
// Module: validator.hpp
// ============================================================================

inline std::string validateInputs(const TransformerInputs& in) {
    if (in.ratedKVA <= 0.0) {
        return "Rated Capacity (kVA) must be greater than zero.";
    }
    if (in.primaryV <= 0.0 || in.secondaryV <= 0.0) {
        return "Primary and Secondary Voltages must be positive numbers.";
    }
    if (in.ironLoss < 0.0) {
        return "Iron Loss (Pi) cannot be negative.";
    }
    if (in.fullLoadCuLoss <= 0.0) {
        return "Full Load Copper Loss (Pcu) must be greater than zero.";
    }
    if (in.powerFactor <= 0.0 || in.powerFactor > 1.0) {
        return "Power Factor must be between 0.01 and 1.00.";
    }
    if (in.loadFraction <= 0.0 || in.loadFraction > 2.0) {
        return "Load Fraction must be between 0.05 (5%) and 2.00 (200%).";
    }
    return ""; // Empty string indicates valid inputs
}
