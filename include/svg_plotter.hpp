#pragma once
#include <string>
#include "transformer_core.hpp"

// ============================================================================
// MEMBER 2: PURE C++ SVG GRAPH PLOTTER
// Module: svg_plotter.hpp
// ============================================================================

// Generates an interactive SVG efficiency vs load curve
std::string generateEfficiencyCurveSVG(const TransformerInputs& in, const TransformerOutputs& out);

// Generates an SVG losses comparison bar chart
std::string generateLossesBreakdownSVG(const TransformerOutputs& out);
