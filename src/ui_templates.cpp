#include "../include/ui_templates.hpp"
#include <sstream>
#include <iomanip>

// ============================================================================
// MEMBER 4: C++ UI VIEW IMPLEMENTATION
// Module: ui_templates.cpp
// ============================================================================

static std::string f2(double v) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2) << v;
    return ss.str();
}

std::string renderDashboard(
    const TransformerInputs& in, 
    const TransformerOutputs* out, 
    const std::string& curveSVG, 
    const std::string& lossSVG, 
    const std::string& errorMsg
) {
    std::ostringstream h;

    h << R"html(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Transformer Efficiency Analyzer (100% C++)</title>
    <style>
        :root {
            --bg-main: #0b1120;
            --bg-card: #1e293b;
            --bg-card-sub: #0f172a;
            --border: #334155;
            --primary: #38bdf8;
            --primary-hover: #0284c7;
            --accent-green: #10b981;
            --accent-amber: #f59e0b;
            --accent-rose: #f43f5e;
            --text-main: #f8fafc;
            --text-muted: #94a3b8;
        }
        * { box-sizing: border-box; margin: 0; padding: 0; }
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif;
            background-color: var(--bg-main);
            color: var(--text-main);
            line-height: 1.5;
            padding: 24px 16px;
        }
        .container { max-width: 1080px; margin: 0 auto; }
        
        /* Header */
        header { text-align: center; margin-bottom: 30px; }
        .logo-badge {
            display: inline-block;
            background: rgba(56, 189, 248, 0.1);
            color: var(--primary);
            border: 1px solid rgba(56, 189, 248, 0.3);
            font-size: 0.8rem;
            font-weight: 600;
            padding: 4px 14px;
            border-radius: 9999px;
            text-transform: uppercase;
            letter-spacing: 0.05em;
            margin-bottom: 12px;
        }
        h1 { font-size: 2.2rem; font-weight: 800; color: #fff; letter-spacing: -0.02em; }
        .tagline { color: var(--text-muted); font-size: 1rem; margin-top: 6px; }

        /* Error Alert */
        .alert-error {
            background: rgba(244, 63, 94, 0.15);
            border: 1px solid var(--accent-rose);
            color: #fecdd3;
            padding: 14px 18px;
            border-radius: 8px;
            margin-bottom: 24px;
            font-weight: 500;
        }

        /* Cards */
        .card {
            background: var(--bg-card);
            border: 1px solid var(--border);
            border-radius: 14px;
            padding: 24px;
            margin-bottom: 24px;
            box-shadow: 0 10px 25px -5px rgba(0, 0, 0, 0.3);
        }
        .card-header {
            display: flex;
            align-items: center;
            justify-content: space-between;
            margin-bottom: 20px;
            border-bottom: 1px solid var(--border);
            padding-bottom: 12px;
        }
        .card-title { font-size: 1.25rem; font-weight: 700; color: #fff; display: flex; align-items: center; gap: 8px; }

        /* Form Grid */
        .form-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(220px, 1fr));
            gap: 16px;
        }
        .input-group { display: flex; flex-direction: column; gap: 6px; }
        label { font-size: 0.85rem; font-weight: 600; color: var(--text-muted); }
        .unit-wrap { position: relative; }
        input[type="number"] {
            width: 100%;
            background: var(--bg-card-sub);
            border: 1px solid var(--border);
            border-radius: 8px;
            padding: 10px 14px;
            color: #fff;
            font-size: 0.95rem;
            transition: border-color 0.15s ease;
        }
        input[type="number"]:focus {
            outline: none;
            border-color: var(--primary);
            box-shadow: 0 0 0 3px rgba(56, 189, 248, 0.2);
        }
        
        /* Submit Button */
        .btn-submit {
            display: block;
            width: 100%;
            background: linear-gradient(135deg, #0284c7, #38bdf8);
            color: #fff;
            border: none;
            border-radius: 8px;
            padding: 14px 20px;
            font-size: 1rem;
            font-weight: 700;
            cursor: pointer;
            margin-top: 20px;
            transition: all 0.2s ease;
            text-shadow: 0 1px 2px rgba(0,0,0,0.2);
        }
        .btn-submit:hover { opacity: 0.95; transform: translateY(-1px); }

        /* KPI Stat Cards */
        .kpi-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(220px, 1fr));
            gap: 16px;
            margin-bottom: 24px;
        }
        .kpi-card {
            background: var(--bg-card-sub);
            border: 1px solid var(--border);
            border-radius: 12px;
            padding: 18px;
            position: relative;
            overflow: hidden;
        }
        .kpi-card::before {
            content: '';
            position: absolute;
            top: 0; left: 0; right: 0; height: 3px;
        }
        .kpi-primary::before { background: var(--primary); }
        .kpi-green::before { background: var(--accent-green); }
        .kpi-amber::before { background: var(--accent-amber); }
        .kpi-rose::before { background: var(--accent-rose); }
        
        .kpi-label { font-size: 0.8rem; font-weight: 600; text-transform: uppercase; color: var(--text-muted); letter-spacing: 0.05em; }
        .kpi-value { font-size: 2.1rem; font-weight: 800; color: #fff; margin: 4px 0; }
        .kpi-desc { font-size: 0.82rem; color: var(--text-muted); }

        /* Charts Layout */
        .charts-grid {
            display: grid;
            grid-template-columns: 3fr 2fr;
            gap: 20px;
            margin-bottom: 24px;
        }
        @media (max-width: 840px) {
            .charts-grid { grid-template-columns: 1fr; }
        }
        .svg-container { border-radius: 10px; overflow: hidden; border: 1px solid var(--border); }

        /* Calculation Steps */
        .audit-list {
            background: var(--bg-card-sub);
            border: 1px solid var(--border);
            border-radius: 8px;
            padding: 16px 20px;
            list-style-type: none;
        }
        .audit-list li {
            font-family: 'Consolas', 'Courier New', monospace;
            font-size: 0.88rem;
            color: #cbd5e1;
            padding: 8px 0;
            border-bottom: 1px solid rgba(51, 65, 85, 0.5);
        }
        .audit-list li:last-child { border-bottom: none; font-weight: bold; color: var(--primary); }

        /* Team Roles Footer */
        footer {
            margin-top: 40px;
            padding: 24px;
            background: var(--bg-card);
            border: 1px solid var(--border);
            border-radius: 12px;
            text-align: center;
        }
        .team-title { font-size: 0.95rem; font-weight: 700; color: var(--primary); margin-bottom: 12px; text-transform: uppercase; letter-spacing: 0.05em; }
        .team-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
            gap: 12px;
            font-size: 0.85rem;
            color: var(--text-muted);
        }
        .member-badge {
            background: var(--bg-card-sub);
            border: 1px solid var(--border);
            padding: 10px;
            border-radius: 6px;
        }
        .member-name { font-weight: bold; color: #fff; }
        .member-role { font-size: 0.75rem; color: var(--primary); }
    </style>
</head>
<body>
<div class="container">

    <header>
        <span class="logo-badge">Pure C++ Web Architecture</span>
        <h1>Transformer Efficiency Analyzer</h1>
        <p class="tagline">Comprehensive Loss Diagnostics, Peak Loading & Equivalent Circuit Performance</p>
    </header>
)html";

    if (!errorMsg.empty()) {
        h << R"html(<div class="alert-error"><strong>Input Validation Error:</strong> )html" << errorMsg << R"html(</div>)html";
    }

    h << R"html(
    <!-- INPUT FORM CARD -->
    <div class="card">
        <div class="card-header">
            <h2 class="card-title">⚙️ Operating Parameters & Test Data</h2>
            <span style="font-size:0.85rem; color:var(--text-muted);">Standard O.C. & S.C. Specification</span>
        </div>
        <form method="POST" action="/calculate">
            <div class="form-grid">
                <div class="input-group">
                    <label>Rated Capacity (S)</label>
                    <input type="number" step="0.1" name="ratedKVA" value=")html" << in.ratedKVA << R"html(" required>
                    <span style="font-size:0.75rem; color:var(--text-muted);">kVA</span>
                </div>
                <div class="input-group">
                    <label>Primary Voltage (V1)</label>
                    <input type="number" step="1" name="primaryV" value=")html" << in.primaryV << R"html(" required>
                    <span style="font-size:0.75rem; color:var(--text-muted);">Volts (High-Voltage)</span>
                </div>
                <div class="input-group">
                    <label>Secondary Voltage (V2)</label>
                    <input type="number" step="1" name="secondaryV" value=")html" << in.secondaryV << R"html(" required>
                    <span style="font-size:0.75rem; color:var(--text-muted);">Volts (Low-Voltage)</span>
                </div>
                <div class="input-group">
                    <label>Iron / Core Loss (Pi)</label>
                    <input type="number" step="1" name="ironLoss" value=")html" << in.ironLoss << R"html(" required>
                    <span style="font-size:0.75rem; color:var(--text-muted);">Watts (from O.C. Test)</span>
                </div>
                <div class="input-group">
                    <label>Full Load Cu Loss (Pcu)</label>
                    <input type="number" step="1" name="fullLoadCuLoss" value=")html" << in.fullLoadCuLoss << R"html(" required>
                    <span style="font-size:0.75rem; color:var(--text-muted);">Watts (from S.C. Test)</span>
                </div>
                <div class="input-group">
                    <label>Operating Load Fraction (x)</label>
                    <input type="number" step="0.05" min="0.05" max="2.0" name="loadFraction" value=")html" << in.loadFraction << R"html(" required>
                    <span style="font-size:0.75rem; color:var(--text-muted);">0.75 = 75% Load</span>
                </div>
                <div class="input-group">
                    <label>Power Factor (cos &phi;)</label>
                    <input type="number" step="0.01" min="0.1" max="1.0" name="powerFactor" value=")html" << in.powerFactor << R"html(" required>
                    <span style="font-size:0.75rem; color:var(--text-muted);">0.80 Lagging</span>
                </div>
            </div>
            <button type="submit" class="btn-submit">⚡ Compute Performance & Generate Curves</button>
        </form>
    </div>
)html";

    if (out) {
        h << R"html(
    <!-- RESULTS DASHBOARD -->
    <div class="card">
        <div class="card-header">
            <h2 class="card-title">📊 Performance Diagnostics</h2>
            <span style="font-size:0.85rem; color:var(--accent-green); font-weight:600;">Computed Successfully</span>
        </div>

        <div class="kpi-grid">
            <div class="kpi-card kpi-primary">
                <div class="kpi-label">Operating Efficiency</div>
                <div class="kpi-value">)html" << f2(out->efficiency) << R"html(<span style="font-size:1.2rem;">%</span></div>
                <div class="kpi-desc">At )html" << f2(in.loadFraction * 100.0) << R"html(% Load, PF )html" << f2(in.powerFactor) << R"html(</div>
            </div>
            <div class="kpi-card kpi-green">
                <div class="kpi-label">Peak Efficiency (&eta; max)</div>
                <div class="kpi-value">)html" << f2(out->maxEfficiency) << R"html(<span style="font-size:1.2rem;">%</span></div>
                <div class="kpi-desc">When Core Loss = Copper Loss</div>
            </div>
            <div class="kpi-card kpi-amber">
                <div class="kpi-label">Max Efficiency Load</div>
                <div class="kpi-value">)html" << f2(out->maxEffLoadFraction * 100.0) << R"html(<span style="font-size:1.2rem;">%</span></div>
                <div class="kpi-desc">Optimal Loading: )html" << f2(out->maxEffKVA) << R"html( kVA</div>
            </div>
            <div class="kpi-card kpi-rose">
                <div class="kpi-label">Total Dissipated Losses</div>
                <div class="kpi-value">)html" << f2(out->totalLossKW) << R"html(<span style="font-size:1.2rem;">kW</span></div>
                <div class="kpi-desc">Pi: )html" << f2(out->ironLossW) << "W | Pcu: " << f2(out->copperLossW) << R"html(W</div>
            </div>
        </div>

        <!-- CHARTS SECTION -->
        <div class="charts-grid">
            <div class="svg-container">
                )html" << curveSVG << R"html(
            </div>
            <div class="svg-container">
                )html" << lossSVG << R"html(
            </div>
        </div>

        <!-- STEP BY STEP AUDIT TRAIL -->
        <h3 style="margin-bottom:12px; font-size:1.1rem; color:#fff;">📑 Step-by-Step Analytical Working (Viva Reference)</h3>
        <ul class="audit-list">
)html";
        for (const auto& step : out->calculationSteps) {
            h << "            <li>" << step << "</li>\n";
        }
        h << R"html(
        </ul>
    </div>
)html";
    }

    // Team Credits Footer
    h << R"html(
    <footer>
        <div class="team-title">Academic Project Team (C++ Course)</div>
        <div class="team-grid">
            <div class="member-badge">
                <div class="member-name">Member 1</div>
                <div class="member-role">Core Electrical Engine (transformer_core.cpp)</div>
            </div>
            <div class="member-badge">
                <div class="member-name">Member 2</div>
                <div class="member-role">SVG Vector Plotter & Validator (svg_plotter.cpp)</div>
            </div>
            <div class="member-badge">
                <div class="member-name">Member 3</div>
                <div class="member-role">Web Server & API Routing (main.cpp)</div>
            </div>
            <div class="member-badge">
                <div class="member-name">Member 4</div>
                <div class="member-role">UI View & Styling (ui_templates.cpp)</div>
            </div>
        </div>
        <p style="margin-top:14px; font-size:0.75rem; color:#64748b;">
            100% C++ Implementation &bull; Zero JavaScript &bull; Zero External Runtimes
        </p>
    </footer>

</div>
</body>
</html>)html";

    return h.str();
}
