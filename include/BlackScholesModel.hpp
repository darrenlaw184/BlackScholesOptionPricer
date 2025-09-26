#pragma once

#include <cmath>
#include <vector>
#include <memory>

/**
 * @file BlackScholesModel.hpp
 * @brief Black-Scholes option pricing model implementation
 * 
 * This header provides a type-safe, modern C++ implementation of the Black-Scholes
 * model for European option pricing with comprehensive parameter validation.
 */

namespace BlackScholes {

/**
 * @brief Strongly typed parameters for Black-Scholes model
 * 
 * This structure ensures type safety and prevents parameter mix-ups
 * by explicitly naming each parameter with validation.
 */
struct OptionParameters {
    double underlying_price;    ///< Current price of the underlying asset (S)
    double strike_price;       ///< Strike price of the option (K)
    double time_to_expiration; ///< Time to expiration in years (T)
    double risk_free_rate;     ///< Risk-free interest rate (r)
    double volatility;         ///< Volatility of the underlying asset (σ)
    
    /**
     * @brief Construct option parameters with validation
     * @param S Current underlying asset price (must be > 0)
     * @param K Strike price (must be > 0)
     * @param T Time to expiration in years (must be > 0)
     * @param r Risk-free rate (can be negative in current markets)
     * @param sigma Volatility (must be > 0)
     * @throws std::invalid_argument if parameters are invalid
     */
    OptionParameters(double S, double K, double T, double r, double sigma);
    
    /**
     * @brief Validate all parameters
     * @return true if all parameters are valid
     */
    [[nodiscard]] bool is_valid() const noexcept;
};

/**
 * @brief Result of Black-Scholes calculation
 * 
 * Contains both call and put prices along with Greeks for comprehensive analysis
 */
struct OptionPrices {
    double call_price;  ///< European call option price
    double put_price;   ///< European put option price
    double delta_call;  ///< Delta for call option
    double delta_put;   ///< Delta for put option
    double gamma;       ///< Gamma (same for call and put)
    double theta_call;  ///< Theta for call option
    double theta_put;   ///< Theta for put option
    double vega;        ///< Vega (same for call and put)
    double rho_call;    ///< Rho for call option
    double rho_put;     ///< Rho for put option
};

/**
 * @brief Black-Scholes option pricing model
 * 
 * Thread-safe implementation of the Black-Scholes model with full Greeks calculation
 */
class Model {
public:
    /**
     * @brief Calculate option prices using Black-Scholes formula
     * @param params Validated option parameters
     * @return Complete option pricing results including Greeks
     * @throws std::invalid_argument if parameters are invalid
     */
    [[nodiscard]] static OptionPrices calculate_prices(const OptionParameters& params);
    
    /**
     * @brief Calculate call option price only
     * @param params Validated option parameters
     * @return Call option price
     */
    [[nodiscard]] static double call_price(const OptionParameters& params);
    
    /**
     * @brief Calculate put option price only
     * @param params Validated option parameters
     * @return Put option price
     */
    [[nodiscard]] static double put_price(const OptionParameters& params);
    
    /**
     * @brief Generate price curve for plotting
     * @param base_params Base parameters (strike, time, rate, volatility)
     * @param price_range Range around current price to calculate
     * @param num_points Number of points to calculate
     * @return Vector of (underlying_price, call_price, put_price) tuples
     */
    [[nodiscard]] static std::vector<std::tuple<double, double, double>> 
    generate_price_curve(const OptionParameters& base_params, 
                        double price_range = 50.0, 
                        int num_points = 100);

private:
    /**
     * @brief Calculate d1 parameter for Black-Scholes formula
     * @param S Underlying price
     * @param K Strike price
     * @param T Time to expiration
     * @param r Risk-free rate
     * @param sigma Volatility
     * @return d1 value
     */
    [[nodiscard]] static double calculate_d1(double S, double K, double T, double r, double sigma) noexcept;
    
    /**
     * @brief Calculate d2 parameter for Black-Scholes formula
     * @param d1 Previously calculated d1 value
     * @param sigma Volatility
     * @param T Time to expiration
     * @return d2 value
     */
    [[nodiscard]] static double calculate_d2(double d1, double sigma, double T) noexcept;
    
    /**
     * @brief Standard normal cumulative distribution function
     * @param x Input value
     * @return N(x) - cumulative probability
     */
    [[nodiscard]] static double normal_cdf(double x) noexcept;
    
    /**
     * @brief Standard normal probability density function
     * @param x Input value
     * @return φ(x) - probability density
     */
    [[nodiscard]] static double normal_pdf(double x) noexcept;
};

} // namespace BlackScholes
