/**
 * @file main.cpp
 * @brief Black-Scholes Option Pricer - Main Application Entry Point
 * 
 * A modern C++ implementation of the Black-Scholes option pricing model
 * with real-time GUI and interactive visualization capabilities.
 * 
 * Features:
 * - Type-safe Black-Scholes calculation with full Greeks
 * - Real-time interactive GUI using Dear ImGui
 * - Dynamic plotting with ImPlot
 * - Put-Call parity validation
 * - Comprehensive error handling
 * 
 * @author Black-Scholes Option Pricer Team
 * @version 1.0.0
 * @date 2025
 */

#include "OptionPricerGUI.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <stdexcept>

// OpenGL debug callback (simplified for compatibility)
#ifdef _DEBUG
void APIENTRY gl_debug_output(GLenum source, GLenum type, unsigned int id, GLenum severity, 
                             GLsizei length, const char* message, const void* userParam) {
    std::cout << "OpenGL Debug (" << id << "): " << message << std::endl;
}
#endif

/**
 * @brief GLFW error callback
 */
void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

/**
 * @brief Initialize GLFW and create window
 * @param width Window width
 * @param height Window height
 * @param title Window title
 * @return GLFW window pointer or nullptr on failure
 */
std::unique_ptr<GLFWwindow, void(*)(GLFWwindow*)> create_window(int width, int height, const char* title) {
    // Setup GLFW
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    
    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    // Window hints for better quality
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x MSAA
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    
#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

    // Create window
    GLFWwindow* raw_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!raw_window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    // Custom deleter for GLFW window
    auto window_deleter = [](GLFWwindow* w) {
        if (w) {
            glfwDestroyWindow(w);
        }
        glfwTerminate();
    };

    std::unique_ptr<GLFWwindow, void(*)(GLFWwindow*)> window(raw_window, window_deleter);
    
    glfwMakeContextCurrent(window.get());
    glfwSwapInterval(1); // Enable vsync
    
    // OpenGL debug setup disabled for compatibility
    // #ifdef _DEBUG
    // Enable OpenGL debug output if available
    // #endif

    return window;
}

/**
 * @brief Main application entry point
 */
int main() {
    try {
        std::cout << "Starting Black-Scholes Option Pricer v1.0.0" << std::endl;
        std::cout << "Built with modern C++20 and Dear ImGui" << std::endl;
        std::cout << "==========================================" << std::endl;

        // Create window
        constexpr int initial_width = 1400;
        constexpr int initial_height = 900;
        auto window = create_window(initial_width, initial_height, "Black-Scholes Option Pricer v1.0.0");
        
        // Set window icon (optional - would need icon data)
        // glfwSetWindowIcon(window.get(), 1, &icon);
        
        // Initialize GUI context
        constexpr const char* glsl_version = 
#if defined(__APPLE__)
            "#version 150";
#else
            "#version 130";
#endif
        
        GUI::GuiContext gui_context(window.get(), glsl_version);
        
        // Create main application
        GUI::OptionPricerGUI app;
        
        std::cout << "Application initialized successfully!" << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "- Adjust parameters in the left panel" << std::endl;
        std::cout << "- View real-time plots in the right panel" << std::endl;
        std::cout << "- Enable Greeks analysis from the View menu" << std::endl;
        std::cout << "- Toggle between Call/Put/Both views for plotting" << std::endl;
        
        // Main application loop
        while (!glfwWindowShouldClose(window.get()) && !app.should_close()) {
            // Poll events
            glfwPollEvents();
            
            // Start new frame
            gui_context.new_frame();
            
            // Render application
            app.render();
            
            // Render to screen
            int display_w, display_h;
            glfwGetFramebufferSize(window.get(), &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            gui_context.render();
            
            glfwSwapBuffers(window.get());
        }
        
        std::cout << "Application shutting down gracefully..." << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        
        // Show error dialog if possible
        if (glfwInit()) {
            // Could show a simple error dialog here
        }
        
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown fatal error occurred!" << std::endl;
        return EXIT_FAILURE;
    }
    
    std::cout << "Thank you for using Black-Scholes Option Pricer!" << std::endl;
    return EXIT_SUCCESS;
}
