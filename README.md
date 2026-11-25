# Transformer Efficiency Analyzer (100% C++)

An academic engineering web dashboard for diagnosing transformer losses, equivalent circuit performance, and efficiency curves, built entirely in C++ with zero external runtime dependencies.

## 👥 Team Roles (4 Members)

| Member | File Assigned | Responsibilities |
| :--- | :--- | :--- |
| **Member 1 (Math & Physics Lead)** | `include/transformer_core.hpp`<br>`src/transformer_core.cpp` | Core electrical equations ($P_i$, $P_{cu}$, $P_{out}$, $P_{in}$, $\eta$), maximum efficiency condition ($x_{max} = \sqrt{P_i / P_{cu}}$), rated current ratings, step-by-step audit trail. |
| **Member 2 (Graphics & Validation Lead)** | `include/svg_plotter.hpp`<br>`src/svg_plotter.cpp`<br>`include/validator.hpp` | Pure C++ SVG vector curve plotter (multi-power factor curves, peak efficiency target pin, gridlines) and input range validation. |
| **Member 3 (Systems & Server Lead)** | `src/main.cpp` | Embedded HTTP web server using `httplib.h`, endpoint routing (`GET /`, `POST /calculate`), socket orchestration. |
| **Member 4 (Frontend UI Lead)** | `include/ui_templates.hpp`<br>`src/ui_templates.cpp` | Modern dark-mode responsive dashboard design, metric KPI stat cards, and step-by-step calculation report generator embedded in C++ raw string literals. |

## 🚀 How to Build and Run

### Option 1: Double Click `build.bat`
Simply double click `build.bat` in the project root folder.

### Option 2: Terminal / PowerShell Command
From the `TransformerAnalyzer` directory:
```bash
g++ -std=c++17 -O2 -Iinclude src/main.cpp src/transformer_core.cpp src/svg_plotter.cpp src/ui_templates.cpp -lws2_32 -o transformer_analyzer.exe
./transformer_analyzer.exe
```

Open your browser to:
**http://localhost:8080**
