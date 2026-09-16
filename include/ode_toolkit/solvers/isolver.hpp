#pragma once
#include "ode_toolkit/core/types.hpp"
#include "ode_toolkit/core/solution.hpp"
#include <memory>

namespace ode::solvers 
{

    using namespace ode::core;

    class ISolver 
    {
    public:
        virtual ~ISolver() = default;

        // Явная форма (для простых методов)
        virtual Solution solve(
            const OdeFunction& f, 
            const State& y0, 
            Time t0, 
            Time t_end, 
            Time dt
        ) = 0;

        // Неявная форма (для продвинутых методов)
        // По умолчанию бросает исключение, если метод не поддерживает
        virtual Solution solve_implicit(
            const ImplicitOdeFunction& F,
            const JacobianFunction& J,  // Может быть nullptr для численного якобиана
            const State& y0,
            Time t0,
            Time t_end,
            Time dt
        ) {
            throw std::logic_error("This solver does not support implicit ODEs");
        }

        virtual std::string name() const = 0;
    };

    using SolverPtr = std::unique_ptr<ISolver>;

}