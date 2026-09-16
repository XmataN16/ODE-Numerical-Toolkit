// include/ode_toolkit/solvers/factory.hpp
#pragma once
#include "isolver.hpp"
#include <string>

namespace ode::solvers 
{

    class SolverFactory 
    {
    public:
        // Создает решатель по имени (например, "euler", "runge_kutta_4")
        static SolverPtr create(const std::string& method_name);
    };

}