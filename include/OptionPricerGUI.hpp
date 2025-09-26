#pragma once

#include "BlackScholesModel.hpp"
#include <imgui.h>
#include <implot.h>
#include <memory>
#include <vector>
#include <string>

// Forward declarations
struct GLFWwindow;

/**
 * @file OptionPricerGUI.hpp
 * @brief GUI interface for Black-Scholes option pricer
 * 
 * Provides a modern, responsive GUI using Dear ImGui with real-time plotting
 * capabilities for visualizing option prices across different underlying prices.
 */

namespace GUI {

/**
 * @brief Plot type selection for visualization
 */
enum class PlotType {
    CallPrice,  ///< Show call option price curve
    PutPrice,   ///< Show put option price curve
    Both        ///< Show both call and put price curves
};

/**
 * @brief Main GUI application class
 * 
 * Manages the complete user interface including parameter input,
 * real-time calculation, and dynamic plotting of option prices.
 */
class OptionPricerGUI {
public:
    /**
     * @brief Constructor - initializes GUI with default parameters
     */
    OptionPricerGUI();
    
    /**
     * @brief Destructor
     */
    ~OptionPricerGUI() = default;
    
    // Prevent copying and moving for simplicity
    OptionPricerGUI(const OptionPricerGUI&) = delete;
    OptionPricerGUI& operator=(const OptionPricerGUI&) = delete;
    OptionPricerGUI(OptionPricerGUI&&) = delete;
    OptionPricerGUI& operator=(OptionPricerGUI&&) = delete;
    
    /**
     * @brief Render the complete GUI interface
     * 
     * This method should be called every frame to update the GUI
     * and handle user interactions.
     */
    void render();
    
    /**
     * @brief Check if the application should close
     * @return true if user requested application closure
     */
    [[nodiscard]] bool should_close() const noexcept { return should_close_; }

private:
    // GUI State
    bool should_close_ = false;
    bool show_demo_window_ = false;
    bool auto_calculate_ = true;
    
    // Input parameters with sensible defaults
    float underlying_price_ = 100.0f;      // S
    float strike_price_ = 105.0f;          // K  
    float time_to_expiration_ = 1.0f;      // T (years)
    float risk_free_rate_ = 0.05f;         // r (5%)
    float volatility_ = 0.2f;              // σ (20%)
    
    // Plot configuration
    PlotType current_plot_type_ = PlotType::Both;
    float price_range_ = 50.0f;
    int num_plot_points_ = 200;
    bool show_greeks_ = false;
    
    // Calculation results
    BlackScholes::OptionPrices current_prices_{};
    std::vector<double> plot_x_data_;
    std::vector<double> plot_call_data_;
    std::vector<double> plot_put_data_;
    bool results_valid_ = false;
    std::string error_message_;
    
    /**
     * @brief Render the parameter input panel
     */
    void render_parameter_panel();
    
    /**
     * @brief Render the results display panel
     */
    void render_results_panel();
    
    /**
     * @brief Render the plotting panel with real-time graphs
     */
    void render_plot_panel();
    
    /**
     * @brief Render the Greeks display panel
     */
    void render_greeks_panel();
    
    /**
     * @brief Update calculations based on current parameters
     */
    void update_calculations();
    
    /**
     * @brief Update plot data for visualization
     */
    void update_plot_data();
    
    /**
     * @brief Get current parameters as BlackScholes::OptionParameters
     * @return Option parameters object
     * @throws std::invalid_argument if parameters are invalid
     */
    [[nodiscard]] BlackScholes::OptionParameters get_current_parameters() const;
    
    /**
     * @brief Format currency value for display
     * @param value Currency value to format
     * @return Formatted string
     */
    [[nodiscard]] static std::string format_currency(double value);
    
    /**
     * @brief Format percentage value for display
     * @param value Percentage value (as decimal)
     * @return Formatted string
     */
    [[nodiscard]] static std::string format_percentage(double value);
    
    /**
     * @brief Show help tooltip for a parameter
     * @param help_text Help text to display
     */
    static void show_help_marker(const char* help_text);
    
    /**
     * @brief Apply consistent styling to the GUI
     */
    void setup_style();
};

/**
 * @brief RAII wrapper for ImGui/ImPlot context management
 */
class GuiContext {
public:
    /**
     * @brief Initialize ImGui and ImPlot contexts
     * @param window GLFW window handle
     * @param glsl_version OpenGL Shading Language version
     */
    GuiContext(GLFWwindow* window, const char* glsl_version);
    
    /**
     * @brief Cleanup contexts
     */
    ~GuiContext();
    
    // Non-copyable and non-movable
    GuiContext(const GuiContext&) = delete;
    GuiContext& operator=(const GuiContext&) = delete;
    GuiContext(GuiContext&&) = delete;
    GuiContext& operator=(GuiContext&&) = delete;
    
    /**
     * @brief Begin new frame for rendering
     */
    void new_frame();
    
    /**
     * @brief End frame and render to screen
     */
    void render();

private:
    GLFWwindow* window_;
};

} // namespace GUI
