// src/solvers/euler.cpp
#include "ode_toolkit/solvers/euler.hpp"
#include <cmath>
#include <stdexcept>

namespace ode::solvers 
{

    Solution EulerSolver::solve(
        const OdeFunction& f,
        const State& y0,
        Time t0,
        Time t_end,
        Time dt
    ) {
        if (dt <= 0) {
            throw std::invalid_argument("Step size (dt) must be positive");
        }

        if (y0.empty()) {
            throw std::invalid_argument("Initial state (y0) cannot be empty");
        }

        Solution solution;
        
        // Начальная точка
        Time t = t0;
        State y = y0;
        
        solution.times.push_back(t);
        solution.states.push_back(y);

        // Основной цикл метода Эйлера
        // y_{n+1} = y_n + dt * f(t_n, y_n)
        while (t < t_end) {
            // Вычисляем производную в текущей точке
            Derivative dydt = f(t, y);

            // Обновляем состояние
            State y_next(y.size());
            for (size_t i = 0; i < y.size(); ++i) {
                y_next[i] = y[i] + dt * dydt[i];
            }

            // Обновляем время
            t += dt;
            
            // Защита от выхода за пределы из-за ошибок округления
            if (t > t_end) {
                t = t_end;
            }

            y = y_next;

            // Сохраняем результат
            solution.times.push_back(t);
            solution.states.push_back(y);
        }

        return solution;
    }

}