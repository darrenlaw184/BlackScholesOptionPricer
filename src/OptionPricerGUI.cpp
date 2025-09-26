#include "OptionPricerGUI.hpp"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <iomanip>

namespace GUI {

// OptionPricerGUI implementation
OptionPricerGUI::OptionPricerGUI() {
    setup_style();
    update_calculations();
}

void OptionPricerGUI::render() {
    // Main menu bar
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) {
                should_close_ = true;
            }
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Show Greeks", nullptr, &show_greeks_);
            ImGui::MenuItem("Show Demo", nullptr, &show_demo_window_);
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("About")) {
                ImGui::OpenPopup("About Black-Scholes Pricer");
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    
    // About popup
    if (ImGui::BeginPopupModal("About Black-Scholes Pricer", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Black-Scholes Option Pricer v1.0");
        ImGui::Text("A modern C++ implementation of the Black-Scholes model");
        ImGui::Text("for European option pricing with real-time visualization.");
        ImGui::Separator();
        ImGui::Text("Features:");
        ImGui::BulletText("Type-safe parameter handling");
        ImGui::BulletText("Real-time price calculation and plotting");
        ImGui::BulletText("Complete Greeks analysis");
        ImGui::BulletText("Interactive visualization");
        
        if (ImGui::Button("Close")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    
    // Demo window
    if (show_demo_window_) {
        ImGui::ShowDemoWindow(&show_demo_window_);
        // ImPlot::ShowDemoWindow(&show_demo_window_); // Not available in this ImPlot version
    }
    
    // Main application layout
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    // ImGui::SetNextWindowViewport(viewport->ID); // Not available in this ImGui version
    
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | 
                                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
    
    if (ImGui::Begin("Black-Scholes Option Pricer", nullptr, window_flags)) {
        // Left panel - Parameters and Results
        if (ImGui::BeginChild("LeftPanel", ImVec2(350, 0), true)) {
            render_parameter_panel();
            ImGui::Separator();
            render_results_panel();
            
            if (show_greeks_) {
                ImGui::Separator();
                render_greeks_panel();
            }
        }
        ImGui::EndChild();
        
        ImGui::SameLine();
        
        // Right panel - Plot
        if (ImGui::BeginChild("RightPanel", ImVec2(0, 0), true)) {
            render_plot_panel();
        }
        ImGui::EndChild();
    }
    ImGui::End();
}

void OptionPricerGUI::render_parameter_panel() {
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Option Parameters");
    ImGui::Separator();
    
    bool params_changed = false;
    
    // Underlying Price (S)
    ImGui::Text("Underlying Price (S)");
    ImGui::SameLine();
    show_help_marker("Current price of the underlying asset");
    if (ImGui::InputFloat("##underlying", &underlying_price_, 0.1f, 1.0f, "%.2f")) {
        underlying_price_ = std::max(0.01f, underlying_price_);
        params_changed = true;
    }
    
    // Strike Price (K)
    ImGui::Text("Strike Price (K)");
    ImGui::SameLine();
    show_help_marker("Exercise price of the option");
    if (ImGui::InputFloat("##strike", &strike_price_, 0.1f, 1.0f, "%.2f")) {
        strike_price_ = std::max(0.01f, strike_price_);
        params_changed = true;
    }
    
    // Time to Expiration (T)
    ImGui::Text("Time to Expiration (T)");
    ImGui::SameLine();
    show_help_marker("Time until expiration in years (e.g., 0.25 for 3 months)");
    if (ImGui::InputFloat("##time", &time_to_expiration_, 0.01f, 0.1f, "%.3f")) {
        time_to_expiration_ = std::max(0.001f, time_to_expiration_);
        params_changed = true;
    }
    
    // Risk-free Rate (r)
    ImGui::Text("Risk-free Rate (r)");
    ImGui::SameLine();
    show_help_marker("Risk-free interest rate as decimal (e.g., 0.05 for 5%)");
    if (ImGui::InputFloat("##rate", &risk_free_rate_, 0.001f, 0.01f, "%.4f")) {
        params_changed = true;
    }
    
    // Volatility (σ)
    ImGui::Text("Volatility (σ)");
    ImGui::SameLine();
    show_help_marker("Annual volatility as decimal (e.g., 0.2 for 20%)");
    if (ImGui::InputFloat("##volatility", &volatility_, 0.01f, 0.1f, "%.3f")) {
        volatility_ = std::max(0.001f, volatility_);
        params_changed = true;
    }
    
    ImGui::Spacing();
    
    // Auto-calculation toggle
    if (ImGui::Checkbox("Auto Calculate", &auto_calculate_)) {
        if (auto_calculate_) {
            params_changed = true;
        }
    }
    
    ImGui::SameLine();
    
    // Manual calculation button
    if (ImGui::Button("Calculate") || (auto_calculate_ && params_changed)) {
        update_calculations();
    }
    
    // Calculation settings
    ImGui::Spacing();
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Plot Settings");
    ImGui::Separator();
    
    if (ImGui::InputFloat("Price Range", &price_range_, 1.0f, 10.0f, "±%.0f")) {
        price_range_ = std::max(1.0f, price_range_);
        if (auto_calculate_) {
            update_plot_data();
        }
    }
    
    if (ImGui::InputInt("Plot Points", &num_plot_points_)) {
        num_plot_points_ = std::clamp(num_plot_points_, 50, 1000);
        if (auto_calculate_) {
            update_plot_data();
        }
    }
}

void OptionPricerGUI::render_results_panel() {
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Option Prices");
    ImGui::Separator();
    
    if (!results_valid_) {
        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Error:");
        ImGui::TextWrapped("%s", error_message_.c_str());
        return;
    }
    
    // Call Price
    ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), "Call Price:");
    ImGui::SameLine();
    ImGui::Text("%s", format_currency(current_prices_.call_price).c_str());
    
    // Put Price  
    ImGui::TextColored(ImVec4(0.8f, 0.2f, 0.2f, 1.0f), "Put Price:");
    ImGui::SameLine();
    ImGui::Text("%s", format_currency(current_prices_.put_price).c_str());
    
    // Put-Call Parity Check
    const double parity_lhs = current_prices_.call_price - current_prices_.put_price;
    const double parity_rhs = underlying_price_ - strike_price_ * std::exp(-risk_free_rate_ * time_to_expiration_);
    const double parity_diff = std::abs(parity_lhs - parity_rhs);
    
    ImGui::Spacing();
    ImGui::Text("Put-Call Parity Check:");
    if (parity_diff < 0.01) {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), "✓ Valid");
    } else {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "⚠ Difference: %.4f", parity_diff);
    }
}

void OptionPricerGUI::render_plot_panel() {
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Option Price Visualization");
    ImGui::Separator();
    
    // Plot type selection
    ImGui::Text("Display:");
    ImGui::SameLine();
    if (ImGui::RadioButton("Call", current_plot_type_ == PlotType::CallPrice)) {
        current_plot_type_ = PlotType::CallPrice;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Put", current_plot_type_ == PlotType::PutPrice)) {
        current_plot_type_ = PlotType::PutPrice;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Both", current_plot_type_ == PlotType::Both)) {
        current_plot_type_ = PlotType::Both;
    }
    
    if (!results_valid_ || plot_x_data_.empty()) {
        ImGui::Text("No data to display. Please check parameters and calculate.");
        return;
    }
    
    // Main price plot
    if (ImPlot::BeginPlot("Option Prices vs Underlying Price", ImVec2(-1, -150))) {
        ImPlot::SetupAxes("Underlying Price ($)", "Option Price ($)");
        ImPlot::SetupAxisLimits(ImAxis_X1, plot_x_data_.front(), plot_x_data_.back());
        
        // Current price indicator would go here (commented for compatibility)
        // const double current_price = static_cast<double>(underlying_price_);
        
        // Plot option curves
        if (current_plot_type_ == PlotType::CallPrice || current_plot_type_ == PlotType::Both) {
            ImPlot::SetNextLineStyle(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), 2.0f);
            ImPlot::PlotLine("Call Price", plot_x_data_.data(), plot_call_data_.data(), 
                           static_cast<int>(plot_x_data_.size()));
        }
        
        if (current_plot_type_ == PlotType::PutPrice || current_plot_type_ == PlotType::Both) {
            ImPlot::SetNextLineStyle(ImVec4(0.8f, 0.2f, 0.2f, 1.0f), 2.0f);
            ImPlot::PlotLine("Put Price", plot_x_data_.data(), plot_put_data_.data(), 
                           static_cast<int>(plot_x_data_.size()));
        }
        
        ImPlot::EndPlot();
    }
    
    // Payoff diagram
    if (ImPlot::BeginPlot("Payoff at Expiration", ImVec2(-1, -1))) {
        ImPlot::SetupAxes("Underlying Price ($)", "Payoff ($)");
        ImPlot::SetupAxisLimits(ImAxis_X1, plot_x_data_.front(), plot_x_data_.back());
        
        // Calculate payoffs
        std::vector<double> call_payoff, put_payoff;
        call_payoff.reserve(plot_x_data_.size());
        put_payoff.reserve(plot_x_data_.size());
        
        for (const double price : plot_x_data_) {
            call_payoff.push_back(std::max(0.0, price - static_cast<double>(strike_price_)));
            put_payoff.push_back(std::max(0.0, static_cast<double>(strike_price_) - price));
        }
        
        if (current_plot_type_ == PlotType::CallPrice || current_plot_type_ == PlotType::Both) {
            ImPlot::SetNextLineStyle(ImVec4(0.2f, 0.8f, 0.2f, 0.7f), 1.5f);
            ImPlot::PlotLine("Call Payoff", plot_x_data_.data(), call_payoff.data(), 
                           static_cast<int>(plot_x_data_.size()));
        }
        
        if (current_plot_type_ == PlotType::PutPrice || current_plot_type_ == PlotType::Both) {
            ImPlot::SetNextLineStyle(ImVec4(0.8f, 0.2f, 0.2f, 0.7f), 1.5f);
            ImPlot::PlotLine("Put Payoff", plot_x_data_.data(), put_payoff.data(), 
                           static_cast<int>(plot_x_data_.size()));
        }
        
        ImPlot::EndPlot();
    }
}

void OptionPricerGUI::render_greeks_panel() {
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Greeks Analysis");
    ImGui::Separator();
    
    if (!results_valid_) {
        ImGui::Text("No valid results to display Greeks.");
        return;
    }
    
    // Create table for Greeks
    if (ImGui::BeginTable("Greeks", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Greek", ImGuiTableColumnFlags_WidthFixed, 80.0f);
        ImGui::TableSetupColumn("Call", ImGuiTableColumnFlags_WidthFixed, 80.0f);
        ImGui::TableSetupColumn("Put", ImGuiTableColumnFlags_WidthFixed, 80.0f);
        ImGui::TableHeadersRow();
        
        // Delta
        ImGui::TableNextRow();
        ImGui::TableNextColumn(); ImGui::Text("Delta");
        ImGui::TableNextColumn(); ImGui::Text("%.4f", current_prices_.delta_call);
        ImGui::TableNextColumn(); ImGui::Text("%.4f", current_prices_.delta_put);
        
        // Gamma
        ImGui::TableNextRow();
        ImGui::TableNextColumn(); ImGui::Text("Gamma");
        ImGui::TableNextColumn(); ImGui::Text("%.6f", current_prices_.gamma);
        ImGui::TableNextColumn(); ImGui::Text("%.6f", current_prices_.gamma);
        
        // Theta
        ImGui::TableNextRow();
        ImGui::TableNextColumn(); ImGui::Text("Theta");
        ImGui::TableNextColumn(); ImGui::Text("%.4f", current_prices_.theta_call);
        ImGui::TableNextColumn(); ImGui::Text("%.4f", current_prices_.theta_put);
        
        // Vega
        ImGui::TableNextRow();
        ImGui::TableNextColumn(); ImGui::Text("Vega");
        ImGui::TableNextColumn(); ImGui::Text("%.4f", current_prices_.vega);
        ImGui::TableNextColumn(); ImGui::Text("%.4f", current_prices_.vega);
        
        // Rho
        ImGui::TableNextRow();
        ImGui::TableNextColumn(); ImGui::Text("Rho");
        ImGui::TableNextColumn(); ImGui::Text("%.4f", current_prices_.rho_call);
        ImGui::TableNextColumn(); ImGui::Text("%.4f", current_prices_.rho_put);
        
        ImGui::EndTable();
    }
}

void OptionPricerGUI::update_calculations() {
    try {
        const auto params = get_current_parameters();
        current_prices_ = BlackScholes::Model::calculate_prices(params);
        results_valid_ = true;
        error_message_.clear();
        update_plot_data();
    } catch (const std::exception& e) {
        results_valid_ = false;
        error_message_ = e.what();
    }
}

void OptionPricerGUI::update_plot_data() {
    if (!results_valid_) {
        return;
    }
    
    try {
        const auto params = get_current_parameters();
        const auto curve_data = BlackScholes::Model::generate_price_curve(
            params, static_cast<double>(price_range_), num_plot_points_);
        
        plot_x_data_.clear();
        plot_call_data_.clear();
        plot_put_data_.clear();
        
        plot_x_data_.reserve(curve_data.size());
        plot_call_data_.reserve(curve_data.size());
        plot_put_data_.reserve(curve_data.size());
        
        for (const auto& [price, call, put] : curve_data) {
            plot_x_data_.push_back(price);
            plot_call_data_.push_back(call);
            plot_put_data_.push_back(put);
        }
    } catch (const std::exception& e) {
        // Handle plot data generation errors gracefully
        plot_x_data_.clear();
        plot_call_data_.clear();
        plot_put_data_.clear();
    }
}

BlackScholes::OptionParameters OptionPricerGUI::get_current_parameters() const {
    return BlackScholes::OptionParameters(
        static_cast<double>(underlying_price_),
        static_cast<double>(strike_price_),
        static_cast<double>(time_to_expiration_),
        static_cast<double>(risk_free_rate_),
        static_cast<double>(volatility_)
    );
}

std::string OptionPricerGUI::format_currency(double value) {
    std::ostringstream oss;
    oss << "$" << std::fixed << std::setprecision(2) << value;
    return oss.str();
}

std::string OptionPricerGUI::format_percentage(double value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << (value * 100.0) << "%";
    return oss.str();
}

void OptionPricerGUI::show_help_marker(const char* help_text) {
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(help_text);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void OptionPricerGUI::setup_style() {
    ImGuiStyle& style = ImGui::GetStyle();
    
    // Colors
    style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.54f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.18f, 0.18f, 0.18f, 0.67f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    
    // Styling
    style.WindowRounding = 5.0f;
    style.FrameRounding = 3.0f;
    style.GrabRounding = 3.0f;
    style.ScrollbarRounding = 5.0f;
    style.WindowPadding = ImVec2(8, 8);
    style.FramePadding = ImVec2(4, 3);
    style.ItemSpacing = ImVec2(8, 4);
    style.ItemInnerSpacing = ImVec2(4, 4);
}

// GuiContext implementation
GuiContext::GuiContext(GLFWwindow* window, const char* glsl_version) 
    : window_(window) {
    
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    // Not available in this ImGui version
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Not available in this ImGui version
    
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

GuiContext::~GuiContext() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
}

void GuiContext::new_frame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GuiContext::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    // Update and Render additional Platform Windows - disabled for compatibility
    // ImGuiIO& io = ImGui::GetIO();
    // if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    //     GLFWwindow* backup_current_context = glfwGetCurrentContext();
    //     ImGui::UpdatePlatformWindows();
    //     ImGui::RenderPlatformWindowsDefault();
    //     glfwMakeContextCurrent(backup_current_context);
    // }
}

} // namespace GUI
