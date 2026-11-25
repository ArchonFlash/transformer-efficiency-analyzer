# Transformer Efficiency Analyzer

An interactive engineering dashboard for diagnosing electrical transformer losses, load factor characteristics, and equivalent circuit performance.

## 👥 Team Responsibilities

| Member | Module | Focus Area |
| :--- | :--- | :--- |
| **Member 1 (Electrical Modeling Lead)** | `include/transformer_core.hpp`<br>`src/transformer_core.cpp` | Core electrical equations ($P_i$, $P_{cu}$, $P_{out}$, $P_{in}$, $\eta$), maximum efficiency condition ($x_{max} = \sqrt{P_i / P_{cu}}$), and analytical working steps. |
| **Member 2 (Analytics & Validation Lead)** | `include/svg_plotter.hpp`<br>`src/svg_plotter.cpp`<br>`include/validator.hpp` | Multi-power-factor vector curves, maximum efficiency operating point plotting, and input parameter validation. |
| **Member 3 (Architecture & Server Lead)** | `src/main.cpp` | HTTP application server configuration, request routing (`GET /`, `POST /calculate`), and endpoint orchestration. |
| **Member 4 (Interface & Design Lead)** | `include/ui_templates.hpp`<br>`src/ui_templates.cpp` | Modern responsive engineering dashboard, metric KPI display, and step-by-step analytical reporting. |

## 🚀 How to Run

### Quick Start
Double-click `build.bat` in the project root directory.

### Manual Compilation
From the project folder:
```bash
g++ -std=c++17 -O2 -Iinclude src/main.cpp src/transformer_core.cpp src/svg_plotter.cpp src/ui_templates.cpp -lws2_32 -o transformer_analyzer.exe
./transformer_analyzer.exe
```

Access the dashboard at:
**http://localhost:8080**
