#pragma once
#include <string>
#include <vector>

// ============================================================================
// MEMBER 1: ELECTRICAL PHYSICS & CALCULATION ENGINE
// Module: transformer_core.hpp
// ============================================================================

struct TransformerInputs {
    double ratedKVA;       // S: Rated apparent power in kVA (e.g. 50 kVA)
    double primaryV;       // V1: Primary voltage in Volts (e.g. 2400 V)
    double secondaryV;     // V2: Secondary voltage in Volts (e.g. 240 V)
    double ironLoss;       // Pi: Core/Iron loss in Watts (from Open Circuit test)
    double fullLoadCuLoss; // Pcu,fl: Full load Copper loss in Watts (from Short Circuit test)
    double powerFactor;    // cos(phi): Power factor (e.g. 0.8)
    double loadFraction;   // x: Operating load fraction (e.g. 0.75 for 75% load)
};

struct TransformerOutputs {
    // Current ratings
    double ratedCurrentPrimary;   // I1_fl (A)
    double ratedCurrentSecondary; // I2_fl (A)
    
    // Operating points
    double operatingKVA;          // x * S
    double outputPowerKW;         // Pout = x * S * cos(phi)
    double copperLossW;           // Pcu(x) = x^2 * Pcu_fl
    double ironLossW;             // Pi
    double totalLossKW;           // (Pi + Pcu(x)) / 1000
    double inputPowerKW;          // Pin = Pout + Ploss
    double efficiency;            // eta = (Pout / Pin) * 100 %
    
    // Maximum Efficiency condition (Pi = x^2 * Pcu => x_max = sqrt(Pi / Pcu))
    double maxEffLoadFraction;    // x_max
    double maxEffKVA;             // x_max * S
    double maxEfficiency;         // eta_max %
    
    // Step-by-step audit trail for presentation / viva
    std::vector<std::string> calculationSteps;
};

// Computes all electrical metrics and step-by-step working
TransformerOutputs calculatePerformance(const TransformerInputs& in);

// Generates data points (load percentage vs efficiency) for a given power factor
std::vector<std::pair<double, double>> calculateEfficiencyCurve(const TransformerInputs& in, double pf);
