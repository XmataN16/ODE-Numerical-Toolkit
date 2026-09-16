// include/ode_toolkit/core/solution.hpp
#pragma once
#include "types.hpp"
#include <string>
#include <vector>

namespace ode::core {

    class Solution {
    public:
        std::vector<Time> times;
        std::vector<State> states;

        // Метод для экспорта в CSV
        void export_to_csv(const std::string& filepath) const;
        
        // Геттер для удобства
        size_t size() const { return times.size(); }
        bool empty() const { return times.empty(); }
    };

}