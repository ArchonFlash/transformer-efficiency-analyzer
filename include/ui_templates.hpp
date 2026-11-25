#pragma once
#include <string>
#include "transformer_core.hpp"

// ============================================================================
// MEMBER 4: C++ UI VIEW & TEMPLATING ENGINE
// Module: ui_templates.hpp
// ============================================================================

std::string renderDashboard(
    const TransformerInputs& in, 
    const TransformerOutputs* out, 
    const std::string& curveSVG, 
    const std::string& lossSVG, 
    const std::string& errorMsg = ""
);
