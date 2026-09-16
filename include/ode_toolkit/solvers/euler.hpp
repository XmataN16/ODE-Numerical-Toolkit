#pragma once
#include "isolver.hpp"

namespace ode::solvers {

    using namespace ode::core;

    class EulerSolver : public ISolver
    {
        public:
        EulerSolver() = default;
        ~EulerSolver() override = default;
        
        Solution solve(
            const OdeFunction& f,
            const State& y0,
            Time t0,
            Time t_end,
            Time dt
        ) override;

        std::string name() const override {
            return "Euler Method";
        }
    };
}