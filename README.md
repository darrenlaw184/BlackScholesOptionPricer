# Black-Scholes Option Pricer

A professional C++20 implementation of the Black-Scholes option pricing model with real-time GUI visualization and comprehensive Greeks analysis.

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![C++](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)

## Overview

This application demonstrates advanced C++ programming techniques applied to quantitative finance. It provides accurate Black-Scholes option pricing with an intuitive graphical interface for parameter adjustment and real-time visualization of option price curves.

### Key Features

**Mathematical Implementation**
- Complete Black-Scholes formula for European call and put options
- Full Greeks calculation (Delta, Gamma, Theta, Vega, Rho)
- Put-Call parity validation for accuracy verification
- High-precision calculations using double-precision arithmetic

**Software Engineering**
- Modern C++20 with strict type safety and RAII principles
- Exception-safe parameter validation and error handling
- Smart pointer usage throughout (no raw pointers)
- Comprehensive const-correctness and move semantics

**User Interface & Visualization**
- Professional GUI built with Dear ImGui
- Real-time plotting with ImPlot for dynamic price curves
- Interactive parameter adjustment with immediate recalculation
- Customizable visualizations with professional styling

**Cross-Platform Architecture**
- CMake-based build system for consistent compilation
- Support for Windows, Linux, and macOS
- Automated dependency management scripts
- Comprehensive documentation and setup instructions

## Technical Specifications

**Core Technologies:**
- **Language:** C++20
- **GUI Framework:** Dear ImGui with ImPlot
- **Graphics:** OpenGL with GLFW
- **Build System:** CMake 3.16+
- **Mathematical Libraries:** Standard library with custom implementations

**Architecture Highlights:**
- Separation of concerns with distinct model, view, and controller components
- Template-based design for type safety and performance
- Modern C++ features including designated initializers and structured bindings
- Professional error handling with exception safety guarantees

## Installation and Setup

### Prerequisites
- CMake 3.16 or higher
- C++20 compatible compiler (GCC 10+, Clang 10+, MSVC 2019+)
- OpenGL 3.0+ support
- GLFW 3.3+

### Quick Start

**Automated Setup (Recommended):**
```bash
# Setup dependencies
./setup_dependencies.sh        # Linux/macOS
./setup_dependencies.ps1       # Windows

# Build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)                # Linux/macOS
cmake --build . --config Release  # Windows

# Run
./BlackScholesOptionPricer
```

**Manual Setup:**
```bash
# Clone external dependencies
git clone --depth 1 https://github.com/ocornut/imgui.git external/imgui
git clone --depth 1 https://github.com/epezent/implot.git external/implot

# Install system dependencies
sudo apt install libglfw3-dev libgl1-mesa-dev  # Ubuntu
brew install glfw                               # macOS
```

### Project Structure
```
BlackScholesOptionPricer/
├── CMakeLists.txt              # Build configuration
├── README.md                   # Project documentation
├── include/
│   ├── BlackScholesModel.hpp  # Mathematical model interface
│   └── OptionPricerGUI.hpp    # GUI components
├── src/
│   ├── main.cpp               # Application entry point
│   ├── BlackScholesModel.cpp # Core pricing algorithms
│   └── OptionPricerGUI.cpp   # User interface implementation
└── external/                  # Third-party dependencies
```

## Usage

The application provides an intuitive interface for option pricing analysis:

**Parameter Input:**
- Underlying asset price (S)
- Strike price (K) 
- Time to expiration in years (T)
- Risk-free interest rate (r)
- Volatility (σ)

**Output Analysis:**
- European call and put option prices
- Complete Greeks analysis
- Real-time price curve visualization
- Put-Call parity validation

**Visualization Features:**
- Dynamic plotting of option prices across underlying price ranges
- Payoff diagrams at expiration
- Professional color-coded charts with customizable parameters

## Mathematical Foundation

The implementation uses the standard Black-Scholes formulas:

**Call Price:** C = S₀N(d₁) - Ke^(-rT)N(d₂)

**Put Price:** P = Ke^(-rT)N(-d₂) - S₀N(-d₁)

Where:
- d₁ = [ln(S₀/K) + (r + σ²/2)T] / (σ√T)
- d₂ = d₁ - σ√T
- N(x) is the cumulative standard normal distribution

Greeks are calculated using standard derivatives of the Black-Scholes formula with appropriate scaling for practical use.

## Development Highlights

**Code Quality:**
- Comprehensive error handling and input validation
- Modern C++ best practices with const-correctness
- Professional documentation and inline comments
- Clean architecture with separated concerns

**Performance:**
- Efficient mathematical computations with vectorized operations
- Real-time calculations with minimal latency
- Optimized memory management using RAII principles

**Maintainability:**
- Modular design with clear interfaces
- Extensive unit testing potential
- Professional build system with cross-platform support

## License

MIT License - suitable for commercial and academic use.

## Technical Contact

This project demonstrates proficiency in:
- Advanced C++ programming and modern language features
- Mathematical finance and quantitative analysis
- GUI development and real-time visualization
- Cross-platform software development
- Professional software engineering practices