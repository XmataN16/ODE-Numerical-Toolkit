#include "ode_toolkit/solvers/factory.hpp"
#include "ode_toolkit/core/config.hpp"
#include <iostream>
#include <cmath>
#include <map>

using namespace ode::core;
using namespace ode::solvers;

// Фабрика предопределённых ОДУ
OdeFunction create_ode(const std::string& ode_id, const std::vector<double>& params) 
{
    std::map<std::string, OdeFunction> ode_registry = 
    {
        // y' = -2y (экспоненциальный спад)
        {"exp_decay", [](Time t, const State& y) -> Derivative 
            {
            return { -2.0 * y[0] };
        }},
        
        // y' = y (экспоненциальный рост)
        {"exp_growth", [](Time t, const State& y) -> Derivative 
            {
            return { y[0] };
        }},
        
        // y' = -k*y (параметрический спад, params[0] = k)
        {"param_decay", [&params](Time t, const State& y) -> Derivative 
            {
            double k = params.empty() ? 1.0 : params[0];
            return { -k * y[0] };
        }},
        
        // Гармонический осциллятор: y'' + y = 0
        // Система: y1' = y2, y2' = -y1
        {"harmonic", [](Time t, const State& y) -> Derivative 
            {
            return { y[1], -y[0] };
        }}
    };

    auto it = ode_registry.find(ode_id);
    if (it == ode_registry.end()) 
    {
        throw std::invalid_argument("Unknown ODE: " + ode_id);
    }
    return it->second;
}

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) 
        {
            std::cerr << "Usage: " << argv[0] << " <config.json>\n";
            return 1;
        }

        std::string config_path = argv[1];
        
        // Загружаем конфигурацию
        SolverConfig config = load_config_from_json(config_path);
        
        std::cout << "Loaded config:\n";
        std::cout << "  Method: " << config.method << "\n";
        std::cout << "  ODE: " << config.ode_id << "\n";
        std::cout << "  t: [" << config.t0 << ", " << config.t_end << "]\n";
        std::cout << "  dt: " << config.dt << "\n";

        // Создаём решатель
        auto solver = SolverFactory::create(config.method);
        
        // Создаём ОДУ
        OdeFunction ode = create_ode(config.ode_id, config.params);

        // Решаем
        Solution sol = solver->solve(ode, config.y0, config.t0, config.t_end, config.dt);

        // Сохраняем результат
        sol.export_to_csv(config.output_file);
        
        std::cout << "Solution saved to: " << config.output_file << "\n";
        std::cout << "Points: " << sol.size() << "\n";

        return 0;
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}