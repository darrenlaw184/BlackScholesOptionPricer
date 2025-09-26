#include "BlackScholesModel.hpp"
#include <stdexcept>
#include <algorithm>
#include <numbers>

namespace BlackScholes {

// OptionParameters implementation
OptionParameters::OptionParameters(double S, double K, double T, double r, double sigma)
    : underlying_price(S)
    , strike_price(K)
    , time_to_expiration(T)
    , risk_free_rate(r)
    , volatility(sigma)
{
    if (!is_valid()) {
        throw std::invalid_argument("Invalid option parameters provided");
    }
}

bool OptionParameters::is_valid() const noexcept {
    return underlying_price > 0.0 
        && strike_price > 0.0 
        && time_to_expiration > 0.0 
        && volatility > 0.0
        && std::isfinite(risk_free_rate)
        && std::isfinite(underlying_price)
        && std::isfinite(strike_price)
        && std::isfinite(time_to_expiration)
        && std::isfinite(volatility);
}

// Model implementation
OptionPrices Model::calculate_prices(const OptionParameters& params) {
    if (!params.is_valid()) {
        throw std::invalid_argument("Invalid parameters for Black-Scholes calculation");
    }
    
    const double S = params.underlying_price;
    const double K = params.strike_price;
    const double T = params.time_to_expiration;
    const double r = params.risk_free_rate;
    const double sigma = params.volatility;
    
    // Calculate d1 and d2
    const double d1 = calculate_d1(S, K, T, r, sigma);
    const double d2 = calculate_d2(d1, sigma, T);
    
    // Calculate standard normal CDF and PDF values
    const double N_d1 = normal_cdf(d1);
    const double N_d2 = normal_cdf(d2);
    const double N_neg_d1 = normal_cdf(-d1);
    const double N_neg_d2 = normal_cdf(-d2);
    const double phi_d1 = normal_pdf(d1);
    
    // Discount factors
    const double discount_factor = std::exp(-r * T);
    const double sqrt_T = std::sqrt(T);
    
    // Calculate option prices
    const double call = S * N_d1 - K * discount_factor * N_d2;
    const double put = K * discount_factor * N_neg_d2 - S * N_neg_d1;
    
    // Calculate Greeks
    const double delta_call_val = N_d1;
    const double delta_put_val = N_d1 - 1.0;
    const double gamma_val = phi_d1 / (S * sigma * sqrt_T);
    const double theta_call_val = -(S * phi_d1 * sigma) / (2.0 * sqrt_T) 
                                  - r * K * discount_factor * N_d2;
    const double theta_put_val = -(S * phi_d1 * sigma) / (2.0 * sqrt_T) 
                                 + r * K * discount_factor * N_neg_d2;
    const double vega_val = S * phi_d1 * sqrt_T;
    const double rho_call_val = K * T * discount_factor * N_d2;
    const double rho_put_val = -K * T * discount_factor * N_neg_d2;
    
    return OptionPrices{
        .call_price = call,
        .put_price = put,
        .delta_call = delta_call_val,
        .delta_put = delta_put_val,
        .gamma = gamma_val,
        .theta_call = theta_call_val / 365.25, // Convert to per day
        .theta_put = theta_put_val / 365.25,   // Convert to per day
        .vega = vega_val / 100.0,              // Convert to per 1% volatility change
        .rho_call = rho_call_val / 100.0,      // Convert to per 1% rate change
        .rho_put = rho_put_val / 100.0         // Convert to per 1% rate change
    };
}

double Model::call_price(const OptionParameters& params) {
    if (!params.is_valid()) {
        throw std::invalid_argument("Invalid parameters for Black-Scholes calculation");
    }
    
    const double d1 = calculate_d1(params.underlying_price, params.strike_price, 
                                  params.time_to_expiration, params.risk_free_rate, 
                                  params.volatility);
    const double d2 = calculate_d2(d1, params.volatility, params.time_to_expiration);
    
    const double N_d1 = normal_cdf(d1);
    const double N_d2 = normal_cdf(d2);
    const double discount_factor = std::exp(-params.risk_free_rate * params.time_to_expiration);
    
    return params.underlying_price * N_d1 - params.strike_price * discount_factor * N_d2;
}

double Model::put_price(const OptionParameters& params) {
    if (!params.is_valid()) {
        throw std::invalid_argument("Invalid parameters for Black-Scholes calculation");
    }
    
    const double d1 = calculate_d1(params.underlying_price, params.strike_price, 
                                  params.time_to_expiration, params.risk_free_rate, 
                                  params.volatility);
    const double d2 = calculate_d2(d1, params.volatility, params.time_to_expiration);
    
    const double N_neg_d1 = normal_cdf(-d1);
    const double N_neg_d2 = normal_cdf(-d2);
    const double discount_factor = std::exp(-params.risk_free_rate * params.time_to_expiration);
    
    return params.strike_price * discount_factor * N_neg_d2 - params.underlying_price * N_neg_d1;
}

std::vector<std::tuple<double, double, double>> 
Model::generate_price_curve(const OptionParameters& base_params, 
                           double price_range, 
                           int num_points) {
    if (num_points <= 0) {
        throw std::invalid_argument("Number of points must be positive");
    }
    
    std::vector<std::tuple<double, double, double>> curve;
    curve.reserve(static_cast<size_t>(num_points));
    
    const double start_price = std::max(0.01, base_params.underlying_price - price_range);
    const double end_price = base_params.underlying_price + price_range;
    const double step = (end_price - start_price) / static_cast<double>(num_points - 1);
    
    for (int i = 0; i < num_points; ++i) {
        const double current_price = start_price + static_cast<double>(i) * step;
        
        // Create modified parameters with current price
        const OptionParameters current_params(
            current_price,
            base_params.strike_price,
            base_params.time_to_expiration,
            base_params.risk_free_rate,
            base_params.volatility
        );
        
        const double call = call_price(current_params);
        const double put = put_price(current_params);
        
        curve.emplace_back(current_price, call, put);
    }
    
    return curve;
}

// Private helper methods
double Model::calculate_d1(double S, double K, double T, double r, double sigma) noexcept {
    const double numerator = std::log(S / K) + (r + 0.5 * sigma * sigma) * T;
    const double denominator = sigma * std::sqrt(T);
    return numerator / denominator;
}

double Model::calculate_d2(double d1, double sigma, double T) noexcept {
    return d1 - sigma * std::sqrt(T);
}

double Model::normal_cdf(double x) noexcept {
    // Abramowitz and Stegun approximation with high accuracy
    // Maximum error: 7.5 × 10^-8
    constexpr double a1 =  0.254829592;
    constexpr double a2 = -0.284496736;
    constexpr double a3 =  1.421413741;
    constexpr double a4 = -1.453152027;
    constexpr double a5 =  1.061405429;
    constexpr double p  =  0.3275911;
    
    const int sign = (x < 0) ? -1 : 1;
    x = std::abs(x);
    
    // A&S formula 7.1.26
    const double t = 1.0 / (1.0 + p * x);
    const double y = 1.0 - (((((a5 * t + a4) * t) + a3) * t + a2) * t + a1) * t * std::exp(-x * x / 2.0);
    
    return 0.5 * (1.0 + sign * y);
}

double Model::normal_pdf(double x) noexcept {
    constexpr double inv_sqrt_2pi = 0.3989422804014327; // 1/sqrt(2*π)
    return inv_sqrt_2pi * std::exp(-0.5 * x * x);
}

} // namespace BlackScholes
