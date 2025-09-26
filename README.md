# Black-Scholes Option Pricer

A modern, type-safe C++17 implementation of the Black-Scholes option pricing model with real-time GUI visualization and comprehensive Greeks analysis.

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)

## Features

### 🧮 **Mathematical Accuracy**
- Complete Black-Scholes formula implementation for European options
- Full Greeks calculation (Delta, Gamma, Theta, Vega, Rho)
- Put-Call parity validation
- High-precision mathematical computations using double precision

### 🛡️ **Type Safety & Modern C++**
- Built with C++17 standards and modern best practices
- Comprehensive parameter validation with exception safety
- RAII memory management with smart pointers
- Const-correctness and constexpr usage throughout
- No raw pointers or C-style arrays

### 🎨 **Interactive GUI**
- Real-time parameter adjustment with immediate recalculation
- Professional dark theme with responsive design
- Input validation with helpful tooltips
- Auto-calculation mode with manual override

### 📊 **Advanced Visualization**
- Dynamic option price curves vs underlying asset price
- Separate views for Call, Put, or both option types
- Payoff diagrams at expiration
- Customizable price ranges and plot resolution
- Professional color-coded plotting (Green for calls, Red for puts)

### 📈 **Greeks Analysis**
- Complete Greeks table with real-time updates
- Detailed sensitivity analysis
- Risk management insights

## Example Scenario

With parameters `S=100`, `K=105`, `T=1.0`, `r=0.05`, `σ=0.2`:
- The application calculates precise call and put prices
- Green curve shows call option prices across underlying price range [50, 150]
- Red curve shows corresponding put option prices
- Greeks provide sensitivity analysis for risk management

## Prerequisites

### Required Dependencies
- **CMake** 3.16 or higher
- **C++17 compatible compiler** (GCC 8+, Clang 8+, MSVC 2019+)
- **OpenGL** 3.0 or higher
- **GLFW** 3.3 or higher

### External Libraries (Included)
- **Dear ImGui** - Immediate mode GUI framework
- **ImPlot** - Plotting extension for ImGui

## Installation

### 1. Clone and Setup
```bash
git clone <repository-url>
cd BlackScholesOptionPricer
mkdir external build
```

### 2. Install Dependencies

#### Windows (vcpkg recommended)
```powershell
# Install vcpkg if not already installed
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install

# Install dependencies
.\vcpkg install glfw3:x64-windows opengl:x64-windows
```

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential cmake libglfw3-dev libgl1-mesa-dev
```

#### macOS
```bash
# Using Homebrew
brew install cmake glfw
```

### 3. Setup External Libraries

Download and extract the following into the `external/` directory:

#### Dear ImGui
```bash
cd external
git clone https://github.com/ocornut/imgui.git
```

#### ImPlot
```bash
cd external  
git clone https://github.com/epezent/implot.git
```

Your directory structure should look like:
```
BlackScholesOptionPricer/
├── external/
│   ├── imgui/
│   └── implot/
├── include/
├── src/
├── CMakeLists.txt
└── README.md
```

### 4. Build

#### Windows (Visual Studio)
```powershell
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

#### Linux/macOS
```bash
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

### 5. Run
```bash
# From build directory
./BlackScholesOptionPricer
```

## Usage Guide

### Parameter Input
1. **Underlying Price (S)**: Current stock/asset price (e.g., 100.00)
2. **Strike Price (K)**: Option exercise price (e.g., 105.00)  
3. **Time to Expiration (T)**: Years until expiration (e.g., 1.0 for one year, 0.25 for 3 months)
4. **Risk-free Rate (r)**: Annual risk-free interest rate as decimal (e.g., 0.05 for 5%)
5. **Volatility (σ)**: Annual volatility as decimal (e.g., 0.2 for 20%)

### GUI Controls
- **Auto Calculate**: Automatically recalculates when parameters change
- **Manual Calculate**: Click "Calculate" button to update results
- **Plot Types**: Toggle between Call, Put, or Both price curves
- **Greeks**: Enable from View menu for detailed sensitivity analysis

### Visualization
- **Main Plot**: Shows option prices across a range of underlying prices
- **Payoff Diagram**: Displays option payoffs at expiration
- **Vertical Line**: Indicates current underlying price
- **Color Coding**: Green for calls, Red for puts

## Project Structure

```
BlackScholesOptionPricer/
├── CMakeLists.txt           # Build configuration
├── README.md               # This file
├── include/
│   ├── BlackScholesModel.hpp    # Mathematical model interface
│   └── OptionPricerGUI.hpp      # GUI interface
├── src/
│   ├── main.cpp                 # Application entry point
│   ├── BlackScholesModel.cpp    # Black-Scholes implementation
│   └── OptionPricerGUI.cpp      # GUI implementation
└── external/
    ├── imgui/              # Dear ImGui library
    └── implot/             # ImPlot library
```

## Technical Implementation

### Black-Scholes Formula
The implementation uses the standard Black-Scholes formula:

**Call Price**: `C = S₀N(d₁) - Ke^(-rT)N(d₂)`

**Put Price**: `P = Ke^(-rT)N(-d₂) - S₀N(-d₁)`

Where:
- `d₁ = [ln(S₀/K) + (r + σ²/2)T] / (σ√T)`
- `d₂ = d₁ - σ√T`
- `N(x)` is the cumulative standard normal distribution

### Greeks Calculations
- **Delta**: ∂V/∂S (price sensitivity to underlying price)
- **Gamma**: ∂²V/∂S² (delta sensitivity to underlying price)  
- **Theta**: ∂V/∂T (time decay)
- **Vega**: ∂V/∂σ (volatility sensitivity)
- **Rho**: ∂V/∂r (interest rate sensitivity)

### Type Safety Features
- Strong typing with custom parameter structures
- Comprehensive input validation
- Exception-safe error handling
- RAII resource management
- Modern C++ best practices throughout

## Troubleshooting

### Build Issues
1. **CMake cannot find GLFW**: Ensure GLFW is properly installed and in your system PATH
2. **ImGui/ImPlot not found**: Verify the external libraries are correctly placed in `external/`
3. **OpenGL errors**: Update graphics drivers and ensure OpenGL 3.0+ support

### Runtime Issues
1. **Window doesn't appear**: Check graphics drivers and OpenGL support
2. **Calculation errors**: Verify input parameters are positive (except risk-free rate)
3. **Plot not updating**: Ensure "Auto Calculate" is enabled or click "Calculate" manually

### Performance
- Reduce "Plot Points" if rendering is slow
- Disable MSAA in `main.cpp` for better performance on older hardware

## Contributing

Contributions are welcome! Please ensure:
1. Code follows modern C++17 practices
2. Maintains type safety and const-correctness  
3. Includes appropriate error handling
4. Updates documentation for new features

## License

This project is licensed under the MIT License. See the LICENSE file for details.

## Acknowledgments

- **Fischer Black**, **Myron Scholes**, and **Robert Merton** for the Black-Scholes model
- **Omar Cornut** for the excellent Dear ImGui library
- **Evan Pezent** for the ImPlot plotting library
- The C++ standards committee for modern C++ features

---

**Black-Scholes Option Pricer v1.0.0** - A professional tool for options pricing and analysis.
