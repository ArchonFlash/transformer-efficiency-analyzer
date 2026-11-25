#include <iostream>
#include <string>
#include "../include/httplib.h"
#include "../include/transformer_core.hpp"
#include "../include/validator.hpp"
#include "../include/svg_plotter.hpp"
#include "../include/ui_templates.hpp"

// ============================================================================
// MEMBER 3: HTTP SERVER & CONTROLLER ROUTING
// Module: main.cpp
// ============================================================================

int main() {
    httplib::Server svr;

    // Standard industrial benchmark defaults: 50 kVA, 2400/240 V transformer
    TransformerInputs defaultInputs = {
        50.0,   // 50 kVA
        2400.0, // 2400 V Primary
        240.0,  // 240 V Secondary
        400.0,  // 400 W Iron loss (O.C. test)
        800.0,  // 800 W Full-load copper loss (S.C. test)
        0.80,   // 0.80 Power factor
        0.75    // 75% Operating load
    };

    // Route: GET / (Initial Dashboard with default benchmark data)
    svr.Get("/", [&](const httplib::Request&, httplib::Response& res) {
        TransformerOutputs initialOut = calculatePerformance(defaultInputs);
        std::string curveSVG = generateEfficiencyCurveSVG(defaultInputs, initialOut);
        std::string lossSVG = generateLossesBreakdownSVG(initialOut);
        
        std::string page = renderDashboard(defaultInputs, &initialOut, curveSVG, lossSVG);
        res.set_content(page, "text/html");
    });

    // Route: POST /calculate (Process user input form)
    svr.Post("/calculate", [&](const httplib::Request& req, httplib::Response& res) {
        TransformerInputs userIn = defaultInputs;

        try {
            if (req.has_param("ratedKVA")) userIn.ratedKVA = std::stod(req.get_param_value("ratedKVA"));
            if (req.has_param("primaryV")) userIn.primaryV = std::stod(req.get_param_value("primaryV"));
            if (req.has_param("secondaryV")) userIn.secondaryV = std::stod(req.get_param_value("secondaryV"));
            if (req.has_param("ironLoss")) userIn.ironLoss = std::stod(req.get_param_value("ironLoss"));
            if (req.has_param("fullLoadCuLoss")) userIn.fullLoadCuLoss = std::stod(req.get_param_value("fullLoadCuLoss"));
            if (req.has_param("loadFraction")) userIn.loadFraction = std::stod(req.get_param_value("loadFraction"));
            if (req.has_param("powerFactor")) userIn.powerFactor = std::stod(req.get_param_value("powerFactor"));
        } catch (...) {
            std::string errPage = renderDashboard(userIn, nullptr, "", "", "Invalid numeric inputs provided.");
            res.set_content(errPage, "text/html");
            return;
        }

        // Validate bounds
        std::string validationError = validateInputs(userIn);
        if (!validationError.empty()) {
            std::string errPage = renderDashboard(userIn, nullptr, "", "", validationError);
            res.set_content(errPage, "text/html");
            return;
        }

        // Compute performance & vector graphics
        TransformerOutputs out = calculatePerformance(userIn);
        std::string curveSVG = generateEfficiencyCurveSVG(userIn, out);
        std::string lossSVG = generateLossesBreakdownSVG(out);

        std::string page = renderDashboard(userIn, &out, curveSVG, lossSVG);
        res.set_content(page, "text/html");
    });

    std::cout << "============================================================\n";
    std::cout << "  TRANSFORMER EFFICIENCY ANALYZER\n";
    std::cout << "============================================================\n";
    std::cout << "  [OK] Server running successfully!\n";
    std::cout << "  [URL] Open in your browser: http://localhost:8080\n";
    std::cout << "  [EXIT] Press Ctrl+C in this terminal window to stop.\n";
    std::cout << "============================================================\n";

    svr.listen("0.0.0.0", 8080);
    return 0;
}
