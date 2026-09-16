#include "ode_toolkit/solvers/factory.hpp"
#include <iostream>
#include <cmath>

using namespace ode::core;
using namespace ode::solvers;

int main() {
    // Создаем решатель через фабрику
    auto solver = SolverFactory::create("euler");
    
    std::cout << "Using solver: " << solver->name() << "\n";

    // ОДУ: y' = -2y (решение: y = e^{-2t})
    OdeFunction my_ode = [](Time t, const State& y) -> Derivative {
        return { -2.0 * y[0] };
    };

    State y0 = {1.0};
    Solution sol = solver->solve(my_ode, y0, 0.0, 2.0, 0.1);

    std::cout << "Solution points: " << sol.size() << "\n";
    sol.export_to_csv("euler_test.csv");

    return 0;
}