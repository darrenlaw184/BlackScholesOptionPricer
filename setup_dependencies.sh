#!/bin/bash
# Black-Scholes Option Pricer - Dependency Setup Script (Linux/macOS)
# This script automatically downloads and sets up the required external libraries

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
WHITE='\033[1;37m'
NC='\033[0m' # No Color

# Configuration
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
EXTERNAL_DIR="${PROJECT_ROOT}/external"
IMGUI_URL="https://github.com/ocornut/imgui.git"
IMPLOT_URL="https://github.com/epezent/implot.git"

FORCE=false
VERBOSE=false

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -f|--force)
            FORCE=true
            shift
            ;;
        -v|--verbose)
            VERBOSE=true
            shift
            ;;
        -h|--help)
            cat << EOF
Black-Scholes Option Pricer - Dependency Setup Script

USAGE:
    ./setup_dependencies.sh [OPTIONS]

OPTIONS:
    -f, --force     Force re-download of dependencies even if they exist
    -v, --verbose   Enable verbose output
    -h, --help      Show this help message

EXAMPLES:
    ./setup_dependencies.sh
    ./setup_dependencies.sh --force
    ./setup_dependencies.sh --verbose

This script will download and set up Dear ImGui and ImPlot libraries
required to build the Black-Scholes Option Pricer.
EOF
            exit 0
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            exit 1
            ;;
    esac
done

function print_status() {
    echo -e "${GREEN}✓ $1${NC}"
}

function print_warning() {
    echo -e "${YELLOW}⚠ $1${NC}"
}

function print_error() {
    echo -e "${RED}✗ $1${NC}"
}

function print_info() {
    echo -e "${WHITE}$1${NC}"
}

function check_command() {
    if ! command -v "$1" &> /dev/null; then
        return 1
    fi
    return 0
}

function clone_repository() {
    local url=$1
    local target_path=$2
    local name=$3
    
    if [[ -d "$target_path" ]]; then
        if [[ "$FORCE" == true ]]; then
            print_warning "$name already exists at $target_path. Removing due to --force flag..."
            rm -rf "$target_path"
        else
            print_status "$name already exists at $target_path. Skipping..."
            return
        fi
    fi
    
    print_info "Cloning $name..."
    
    if [[ "$VERBOSE" == true ]]; then
        git clone --depth 1 "$url" "$target_path"
    else
        git clone --depth 1 "$url" "$target_path" &> /dev/null
    fi
    
    if [[ $? -eq 0 ]]; then
        print_status "$name cloned successfully"
    else
        print_error "Failed to clone $name"
        exit 1
    fi
}

function detect_os() {
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        echo "linux"
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        echo "macos"
    elif [[ "$OSTYPE" == "msys" ]]; then
        echo "windows"
    else
        echo "unknown"
    fi
}

function show_install_instructions() {
    local os=$(detect_os)
    
    echo -e "\n${CYAN}System Dependencies Installation:${NC}"
    
    case $os in
        "linux")
            echo -e "${WHITE}Ubuntu/Debian:${NC}"
            echo -e "${CYAN}sudo apt update${NC}"
            echo -e "${CYAN}sudo apt install build-essential cmake libglfw3-dev libgl1-mesa-dev${NC}"
            echo ""
            echo -e "${WHITE}CentOS/RHEL/Fedora:${NC}"
            echo -e "${CYAN}sudo dnf install gcc-c++ cmake glfw-devel mesa-libGL-devel${NC}"
            ;;
        "macos")
            echo -e "${WHITE}Using Homebrew:${NC}"
            echo -e "${CYAN}brew install cmake glfw${NC}"
            echo ""
            echo -e "${WHITE}Using MacPorts:${NC}"
            echo -e "${CYAN}sudo port install cmake glfw${NC}"
            ;;
        "windows")
            echo -e "${WHITE}Using vcpkg:${NC}"
            echo -e "${CYAN}.\\vcpkg install glfw3:x64-windows opengl:x64-windows${NC}"
            ;;
        *)
            echo -e "${YELLOW}Please install CMake, GLFW, and OpenGL development libraries for your system.${NC}"
            ;;
    esac
}

function main() {
    echo -e "${CYAN}Black-Scholes Option Pricer - Dependency Setup${NC}"
    echo -e "${CYAN}=============================================${NC}"
    
    # Check prerequisites
    echo -e "\n${WHITE}Checking prerequisites...${NC}"
    
    if ! check_command git; then
        print_error "Git is not available. Please install Git first."
        echo -e "${YELLOW}Installation instructions:${NC}"
        case $(detect_os) in
            "linux")
                echo -e "${CYAN}sudo apt install git${NC}  # Ubuntu/Debian"
                echo -e "${CYAN}sudo dnf install git${NC}  # CentOS/RHEL/Fedora"
                ;;
            "macos")
                echo -e "${CYAN}brew install git${NC}      # Homebrew"
                echo -e "${CYAN}xcode-select --install${NC} # Xcode Command Line Tools"
                ;;
            *)
                echo -e "${CYAN}Visit: https://git-scm.com/downloads${NC}"
                ;;
        esac
        exit 1
    fi
    print_status "Git is available"
    
    # Create external directory
    if [[ ! -d "$EXTERNAL_DIR" ]]; then
        print_info "Creating external directory..."
        mkdir -p "$EXTERNAL_DIR"
        print_status "External directory created"
    else
        print_status "External directory exists"
    fi
    
    # Clone Dear ImGui
    echo -e "\n${WHITE}Setting up Dear ImGui...${NC}"
    local imgui_path="${EXTERNAL_DIR}/imgui"
    clone_repository "$IMGUI_URL" "$imgui_path" "Dear ImGui"
    
    # Clone ImPlot
    echo -e "\n${WHITE}Setting up ImPlot...${NC}"
    local implot_path="${EXTERNAL_DIR}/implot"
    clone_repository "$IMPLOT_URL" "$implot_path" "ImPlot"
    
    # Verify setup
    echo -e "\n${WHITE}Verifying setup...${NC}"
    
    if [[ -f "${imgui_path}/imgui.h" ]]; then
        print_status "Dear ImGui setup verified"
    else
        print_error "Dear ImGui setup failed - imgui.h not found"
        exit 1
    fi
    
    if [[ -f "${implot_path}/implot.h" ]]; then
        print_status "ImPlot setup verified"
    else
        print_error "ImPlot setup failed - implot.h not found"
        exit 1
    fi
    
    # Success message
    echo ""
    echo -e "${GREEN}🎉 Dependencies setup completed successfully!${NC}"
    
    echo -e "\n${CYAN}Next steps:${NC}"
    echo -e "${WHITE}1. Install system dependencies:${NC}"
    show_install_instructions
    
    echo -e "\n${WHITE}2. Build the project:${NC}"
    echo -e "${CYAN}mkdir -p build && cd build${NC}"
    echo -e "${CYAN}cmake .. -DCMAKE_BUILD_TYPE=Release${NC}"
    echo -e "${CYAN}make -j\$(nproc)${NC}"
    
    echo -e "\n${WHITE}3. Run the application:${NC}"
    echo -e "${CYAN}./BlackScholesOptionPricer${NC}"
    
    echo ""
}

# Make sure script is executable
if [[ ! -x "${BASH_SOURCE[0]}" ]]; then
    print_warning "Making script executable..."
    chmod +x "${BASH_SOURCE[0]}"
fi

# Run main function
main "$@"
