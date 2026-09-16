#pragma once
#include <vector>
#include <functional>
#include <string>

namespace ode::core {

    using Time = double;
    using State = std::vector<double>;
    using Derivative = State;

    // Явная форма: y' = f(t, y)
    using OdeFunction = std::function<Derivative(Time t, const State& y)>;

    // Неявная форма: F(t, y, y') = 0
    // Возвращает вектор невязок (residual)
    using ImplicitOdeFunction = std::function<Derivative(Time t, const State& y, const Derivative& y_prime)>;

    // Для метода Ньютона нужен якобиан ∂F/∂y'
    // Возвращает матрицу (вектор векторов) размера n x n
    using JacobianFunction = std::function<std::vector<std::vector<double>>>(
        Time t, const State& y, const Derivative& y_prime
    );

} 