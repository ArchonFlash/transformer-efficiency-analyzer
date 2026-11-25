#include "../include/svg_plotter.hpp"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>

// ============================================================================
// MEMBER 2: PURE C++ SVG GENERATOR IMPLEMENTATION
// Module: svg_plotter.cpp
// ============================================================================

static std::string toFmt(double val, int prec = 1) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(prec) << val;
    return ss.str();
}

std::string generateEfficiencyCurveSVG(const TransformerInputs& in, const TransformerOutputs& out) {
    std::ostringstream svg;
    
    // SVG Dimensions
    const int W = 620;
    const int H = 280;
    const int padL = 60;
    const int padR = 30;
    const int padT = 30;
    const int padB = 50;
    
    const double plotW = W - padL - padR;
    const double plotH = H - padT - padB;
    
    // X scale: 0% to 140% Load
    const double minX = 0.0;
    const double maxX = 140.0;
    
    // Y scale: Dynamic range (typically 80% to 100%)
    double minY = 85.0;
    double maxY = 100.0;
    if (out.efficiency < minY && out.efficiency > 0) minY = std::floor(out.efficiency / 5.0) * 5.0;
    if (minY < 50.0) minY = 50.0;
    
    auto mapX = [&](double pct) -> double {
        return padL + ((pct - minX) / (maxX - minX)) * plotW;
    };
    auto mapY = [&](double eff) -> double {
        double clamped = std::max(minY, std::min(maxY, eff));
        return padT + (1.0 - ((clamped - minY) / (maxY - minY))) * plotH;
    };
    
    svg << "<svg viewBox=\"0 0 " << W << " " << H << "\" class=\"svg-chart\" xmlns=\"http://www.w3.org/2000/svg\">\n";
    
    // Background and frame
    svg << "  <rect width=\"100%\" height=\"100%\" fill=\"#1e293b\" rx=\"10\"/>\n";
    
    // Horizontal Gridlines & Y-axis labels
    for (double eff = minY; eff <= maxY; eff += 5.0) {
        double yPos = mapY(eff);
        svg << "  <line x1=\"" << padL << "\" y1=\"" << yPos << "\" x2=\"" << (W - padR) 
            << "\" y2=\"" << yPos << "\" stroke=\"#334155\" stroke-dasharray=\"3,3\" stroke-width=\"1\"/>\n";
        svg << "  <text x=\"" << (padL - 10) << "\" y=\"" << (yPos + 4) 
            << "\" fill=\"#94a3b8\" font-size=\"11\" text-anchor=\"end\">" << toFmt(eff, 0) << "%</text>\n";
    }
    
    // Vertical Gridlines & X-axis labels (Load %)
    for (double pct = 0.0; pct <= maxX; pct += 20.0) {
        double xPos = mapX(pct);
        svg << "  <line x1=\"" << xPos << "\" y1=\"" << padT << "\" x2=\"" << xPos 
            << "\" y2=\"" << (H - padB) << "\" stroke=\"#334155\" stroke-dasharray=\"3,3\" stroke-width=\"1\"/>\n";
        svg << "  <text x=\"" << xPos << "\" y=\"" << (H - padB + 18) 
            << "\" fill=\"#94a3b8\" font-size=\"11\" text-anchor=\"middle\">" << toFmt(pct, 0) << "%</text>\n";
    }
    
    // Axis Titles
    double midY = padT + plotH / 2.0;
    svg << "  <text x=\"" << (padL + plotW / 2.0) << "\" y=\"" << (H - 12) 
        << "\" fill=\"#cbd5e1\" font-size=\"12\" font-weight=\"bold\" text-anchor=\"middle\">Transformer Load Factor (% of Full Load)</text>\n";
    svg << "  <text x=\"18\" y=\"" << midY 
        << "\" fill=\"#cbd5e1\" font-size=\"12\" font-weight=\"bold\" transform=\"rotate(-90 18 " << midY << ")\" text-anchor=\"middle\">Efficiency (%)</text>\n";

    // Lambda to plot a path
    auto drawCurve = [&](double pf, const std::string& strokeColor, const std::string& strokeDash = "") {
        auto pts = calculateEfficiencyCurve(in, pf);
        svg << "  <path d=\"M ";
        bool first = true;
        for (const auto& pt : pts) {
            double sx = mapX(pt.first);
            double sy = mapY(pt.second);
            if (first) {
                svg << sx << " " << sy;
                first = false;
            } else {
                svg << " L " << sx << " " << sy;
            }
        }
        svg << "\" fill=\"none\" stroke=\"" << strokeColor << "\" stroke-width=\"2.5\" ";
        if (!strokeDash.empty()) svg << "stroke-dasharray=\"" << strokeDash << "\" ";
        svg << "/>\n";
    };

    // Plot: Unity PF (1.0)
    drawCurve(1.0, "#10b981");
    
    // Plot: Operating PF (User Input)
    drawCurve(in.powerFactor, "#38bdf8");
    
    // Plot: Lower PF (0.6)
    drawCurve(0.6, "#f59e0b", "4,4");

    // Highlight Maximum Efficiency Point
    double peakPct = out.maxEffLoadFraction * 100.0;
    if (peakPct >= minX && peakPct <= maxX) {
        double px = mapX(peakPct);
        double py = mapY(out.maxEfficiency);
        svg << "  <circle cx=\"" << px << "\" cy=\"" << py << "\" r=\"6\" fill=\"#e11d48\" stroke=\"#ffffff\" stroke-width=\"2\"/>\n";
        svg << "  <text x=\"" << px << "\" y=\"" << (py - 12) << "\" fill=\"#f43f5e\" font-size=\"11\" font-weight=\"bold\" text-anchor=\"middle\">&eta; Max (" 
            << toFmt(out.maxEfficiency) << "%)</text>\n";
    }

    // Legend
    svg << "  <g transform=\"translate(" << (W - 240) << ", 10)\">\n";
    svg << "    <line x1=\"0\" y1=\"10\" x2=\"20\" y2=\"10\" stroke=\"#10b981\" stroke-width=\"2.5\"/>\n";
    svg << "    <text x=\"25\" y=\"14\" fill=\"#94a3b8\" font-size=\"10\">Unity PF (1.0)</text>\n";
    svg << "    <line x1=\"90\" y1=\"10\" x2=\"110\" y2=\"10\" stroke=\"#38bdf8\" stroke-width=\"2.5\"/>\n";
    svg << "    <text x=\"115\" y=\"14\" fill=\"#38bdf8\" font-size=\"10\" font-weight=\"bold\">Op. PF (" << toFmt(in.powerFactor, 2) << ")</text>\n";
    svg << "    <line x1=\"175\" y1=\"10\" x2=\"195\" y2=\"10\" stroke=\"#f59e0b\" stroke-width=\"2.5\" stroke-dasharray=\"4,4\"/>\n";
    svg << "    <text x=\"200\" y=\"14\" fill=\"#94a3b8\" font-size=\"10\">PF 0.6</text>\n";
    svg << "  </g>\n";

    svg << "</svg>";
    return svg.str();
}

std::string generateLossesBreakdownSVG(const TransformerOutputs& out) {
    std::ostringstream svg;
    const int W = 400;
    const int H = 160;
    
    double totalW = out.ironLossW + out.copperLossW;
    double ironPct = (totalW > 0) ? (out.ironLossW / totalW) * 100.0 : 50.0;
    double cuPct = (totalW > 0) ? (out.copperLossW / totalW) * 100.0 : 50.0;
    
    svg << "<svg viewBox=\"0 0 " << W << " " << H << "\" class=\"svg-chart\" xmlns=\"http://www.w3.org/2000/svg\">\n";
    svg << "  <rect width=\"100%\" height=\"100%\" fill=\"#1e293b\" rx=\"10\"/>\n";
    svg << "  <text x=\"20\" y=\"28\" fill=\"#cbd5e1\" font-size=\"13\" font-weight=\"bold\">Losses Distribution at Operating Load</text>\n";
    
    // Stacked Bar background
    double barX = 20;
    double barY = 50;
    double barW = 360;
    double barH = 34;
    
    double ironBarW = (ironPct / 100.0) * barW;
    double cuBarW = (cuPct / 100.0) * barW;
    
    // Iron Loss segment (Purple)
    svg << "  <rect x=\"" << barX << "\" y=\"" << barY << "\" width=\"" << ironBarW << "\" height=\"" << barH 
        << "\" fill=\"#8b5cf6\" rx=\"4\"/>\n";
    // Copper Loss segment (Amber)
    svg << "  <rect x=\"" << (barX + ironBarW) << "\" y=\"" << barY << "\" width=\"" << cuBarW << "\" height=\"" << barH 
        << "\" fill=\"#f59e0b\" rx=\"4\"/>\n";
        
    // Legend and metrics
    svg << "  <circle cx=\"25\" cy=\"115\" r=\"5\" fill=\"#8b5cf6\"/>\n";
    svg << "  <text x=\"36\" y=\"119\" fill=\"#94a3b8\" font-size=\"11\">Core Loss (Pi): <tspan fill=\"#f1f5f9\" font-weight=\"bold\">" 
        << toFmt(out.ironLossW) << " W (" << toFmt(ironPct) << "%)</tspan></text>\n";
        
    svg << "  <circle cx=\"215\" cy=\"115\" r=\"5\" fill=\"#f59e0b\"/>\n";
    svg << "  <text x=\"226\" y=\"119\" fill=\"#94a3b8\" font-size=\"11\">Copper Loss (Pcu): <tspan fill=\"#f1f5f9\" font-weight=\"bold\">" 
        << toFmt(out.copperLossW) << " W (" << toFmt(cuPct) << "%)</tspan></text>\n";

    svg << "  <text x=\"20\" y=\"145\" fill=\"#64748b\" font-size=\"10\">Total Dissipation: " 
        << toFmt(out.totalLossKW, 3) << " kW</text>\n";

    svg << "</svg>";
    return svg.str();
}
