// src/solvers/factory.cpp
#include "ode_toolkit/solvers/factory.hpp"
#include "ode_toolkit/solvers/euler.hpp"
#include <stdexcept>

namespace ode::solvers {

    SolverPtr SolverFactory::create(const std::string& method_name) 
    {
        if (method_name == "euler") 
        {
            return std::make_unique<EulerSolver>();
        }
        // if (method_name == "rk4") { ... }
        
        throw std::invalid_argument("Unknown solver method: " + method_name);
    }

}