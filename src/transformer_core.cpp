#include "../include/transformer_core.hpp"
#include <cmath>
#include <sstream>
#include <iomanip>

// ============================================================================
// MEMBER 1: ELECTRICAL PHYSICS IMPLEMENTATION
// Module: transformer_core.cpp
// ============================================================================

static std::string fmt(double val, int precision = 2) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(precision) << val;
    return ss.str();
}

TransformerOutputs calculatePerformance(const TransformerInputs& in) {
    TransformerOutputs out;
    
    // 1. Rated Currents
    out.ratedCurrentPrimary = (in.primaryV > 0) ? (in.ratedKVA * 1000.0) / in.primaryV : 0.0;
    out.ratedCurrentSecondary = (in.secondaryV > 0) ? (in.ratedKVA * 1000.0) / in.secondaryV : 0.0;
    
    // 2. Operating Power and Losses
    out.operatingKVA = in.loadFraction * in.ratedKVA;
    out.outputPowerKW = out.operatingKVA * in.powerFactor;
    out.ironLossW = in.ironLoss;
    out.copperLossW = std::pow(in.loadFraction, 2.0) * in.fullLoadCuLoss;
    out.totalLossKW = (out.ironLossW + out.copperLossW) / 1000.0;
    out.inputPowerKW = out.outputPowerKW + out.totalLossKW;
    
    // 3. Operating Efficiency
    if (out.inputPowerKW > 0.0) {
        out.efficiency = (out.outputPowerKW / out.inputPowerKW) * 100.0;
    } else {
        out.efficiency = 0.0;
    }
    
    // 4. Maximum Efficiency Condition
    // Condition: Constant Loss (Pi) = Variable Loss (x^2 * Pcu,fl)
    // Therefore: x_max = sqrt(Pi / Pcu,fl)
    if (in.fullLoadCuLoss > 0.0 && in.ironLoss >= 0.0) {
        out.maxEffLoadFraction = std::sqrt(in.ironLoss / in.fullLoadCuLoss);
        out.maxEffKVA = out.maxEffLoadFraction * in.ratedKVA;
        
        double maxOutputKW = out.maxEffKVA * in.powerFactor;
        double maxLossKW = (2.0 * in.ironLoss) / 1000.0; // At max eff, Copper loss equals Iron loss
        double maxInputKW = maxOutputKW + maxLossKW;
        
        out.maxEfficiency = (maxInputKW > 0.0) ? (maxOutputKW / maxInputKW) * 100.0 : 0.0;
    } else {
        out.maxEffLoadFraction = 0.0;
        out.maxEffKVA = 0.0;
        out.maxEfficiency = 0.0;
    }
    
    // 5. Build Step-by-Step Viva / Report Audit Trail
    out.calculationSteps.push_back(
        "1. Primary Rated Current: I1 = (S * 1000) / V1 = (" + fmt(in.ratedKVA) + " * 1000) / " + fmt(in.primaryV) + " = " + fmt(out.ratedCurrentPrimary) + " A"
    );
    out.calculationSteps.push_back(
        "2. Secondary Rated Current: I2 = (S * 1000) / V2 = (" + fmt(in.ratedKVA) + " * 1000) / " + fmt(in.secondaryV) + " = " + fmt(out.ratedCurrentSecondary) + " A"
    );
    out.calculationSteps.push_back(
        "3. Operating Output Power: Pout = x * S * cos(phi) = " + fmt(in.loadFraction) + " * " + fmt(in.ratedKVA) + " * " + fmt(in.powerFactor) + " = " + fmt(out.outputPowerKW) + " kW"
    );
    out.calculationSteps.push_back(
        "4. Operating Copper Loss: Pcu(x) = x^2 * Pcu,fl = (" + fmt(in.loadFraction) + ")^2 * " + fmt(in.fullLoadCuLoss) + " = " + fmt(out.copperLossW) + " W"
    );
    out.calculationSteps.push_back(
        "5. Total Losses: Ploss = Pi + Pcu(x) = " + fmt(out.ironLossW) + " W + " + fmt(out.copperLossW) + " W = " + fmt(out.totalLossKW) + " kW"
    );
    out.calculationSteps.push_back(
        "6. Input Power: Pin = Pout + Ploss = " + fmt(out.outputPowerKW) + " kW + " + fmt(out.totalLossKW) + " kW = " + fmt(out.inputPowerKW) + " kW"
    );
    out.calculationSteps.push_back(
        "7. Operating Efficiency: eta = (Pout / Pin) * 100 = (" + fmt(out.outputPowerKW) + " / " + fmt(out.inputPowerKW) + ") * 100 = " + fmt(out.efficiency) + " %"
    );
    out.calculationSteps.push_back(
        "8. Condition for Peak Efficiency: x_max = sqrt(Pi / Pcu,fl) = sqrt(" + fmt(in.ironLoss) + " / " + fmt(in.fullLoadCuLoss) + ") = " + fmt(out.maxEffLoadFraction, 4) + " (" + fmt(out.maxEffLoadFraction * 100.0) + "% Load)"
    );
    out.calculationSteps.push_back(
        "9. Peak Efficiency Value: eta_max = " + fmt(out.maxEfficiency) + " % at " + fmt(out.maxEffKVA) + " kVA"
    );
    
    return out;
}

std::vector<std::pair<double, double>> calculateEfficiencyCurve(const TransformerInputs& in, double pf) {
    std::vector<std::pair<double, double>> points;
    // Compute from 10% load to 140% load in 5% steps
    for (int pct = 10; pct <= 140; pct += 5) {
        double x = pct / 100.0;
        double pOutKW = x * in.ratedKVA * pf;
        double pCuW = std::pow(x, 2.0) * in.fullLoadCuLoss;
        double pLossKW = (in.ironLoss + pCuW) / 1000.0;
        double pInKW = pOutKW + pLossKW;
        double eff = (pInKW > 0) ? (pOutKW / pInKW) * 100.0 : 0.0;
        points.push_back({ static_cast<double>(pct), eff });
    }
    return points;
}
