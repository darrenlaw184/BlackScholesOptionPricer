# Changelog

All notable changes to the Black-Scholes Option Pricer project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2025-09-25

### Initial Release

#### Added
- **Core Black-Scholes Implementation**
  - Complete Black-Scholes formula for European call and put options
  - High-precision mathematical calculations using double precision
  - Comprehensive parameter validation with exception safety
  - Put-Call parity validation for result verification

- **Full Greeks Analysis**
  - Delta: Price sensitivity to underlying asset price changes
  - Gamma: Delta sensitivity to underlying asset price changes
  - Theta: Time decay (converted to per-day basis)
  - Vega: Volatility sensitivity (per 1% volatility change)
  - Rho: Interest rate sensitivity (per 1% rate change)

- **Modern C++17 Implementation**
  - Type-safe parameter handling with custom structures
  - RAII memory management with smart pointers
  - Const-correctness and constexpr usage throughout
  - Exception-safe error handling
  - No raw pointers or C-style arrays

- **Interactive GUI Interface**
  - Real-time parameter input with immediate validation
  - Professional dark theme with responsive design
  - Auto-calculation mode with manual override option
  - Comprehensive help tooltips for all parameters
  - Input validation with user-friendly error messages

- **Advanced Visualization**
  - Real-time option price curves vs underlying asset price
  - Separate views for Call, Put, or both option types simultaneously
  - Payoff diagrams showing option values at expiration
  - Customizable price ranges and plot resolution
  - Professional color coding (Green for calls, Red for puts)
  - Current price indicator on all plots

- **Cross-Platform Support**
  - Windows support with Visual Studio and MinGW
  - Linux support with GCC and Clang
  - macOS support with Xcode and Homebrew
  - CMake-based build system for consistent builds

- **Developer Experience**
  - Comprehensive documentation with examples
  - Automated dependency setup scripts (PowerShell and Bash)
  - Convenient build scripts for Windows users
  - Makefile with common development tasks
  - Professional project structure and organization

- **Build System and Dependencies**
  - CMake 3.16+ build configuration
  - Dear ImGui integration for immediate-mode GUI
  - ImPlot integration for real-time plotting capabilities
  - GLFW for cross-platform windowing
  - OpenGL 3.0+ for hardware-accelerated rendering

#### Technical Specifications
- **Programming Language**: C++17 with modern best practices
- **Mathematical Precision**: Double precision floating-point arithmetic
- **GUI Framework**: Dear ImGui with ImPlot for plotting
- **Graphics API**: OpenGL 3.0+ with GLFW
- **Build System**: CMake with cross-platform support
- **Memory Management**: RAII with smart pointers, no memory leaks
- **Error Handling**: Exception-safe with comprehensive validation

#### Features Overview
- **Input Parameters**:
  - Underlying asset price (S) - Current market price
  - Strike price (K) - Option exercise price
  - Time to expiration (T) - Years until expiration
  - Risk-free rate (r) - Annual interest rate (decimal)
  - Volatility (σ) - Annual volatility (decimal)

- **Output Results**:
  - European call option price
  - European put option price
  - Complete Greeks analysis
  - Put-Call parity validation
  - Real-time visualization and plotting

- **Visualization Features**:
  - Dynamic price curves across underlying price ranges
  - Payoff diagrams at expiration
  - Multiple plot types and customization options
  - Professional color schemes and styling

#### Development Notes
- Built with strict type safety and modern C++ guidelines
- Comprehensive error handling and input validation
- Optimized for performance with efficient mathematical computations
- Extensible architecture for future enhancements
- Professional code organization with clear separation of concerns

#### Known Limitations
- Currently supports European options only (American options not implemented)
- Single-asset options only (no multi-asset or exotic options)
- No historical data integration or volatility estimation
- No options strategies or portfolio analysis
- Requires OpenGL 3.0+ compatible hardware

#### Future Considerations
- American option pricing models
- Monte Carlo simulation methods
- Historical volatility calculation
- Options trading strategies analysis
- Data import/export functionality
- Multi-language localization support
