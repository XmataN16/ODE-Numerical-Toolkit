// src/core/solution.cpp
#include "ode_toolkit/core/solution.hpp"
#include <fstream>
#include <stdexcept>
#include <iomanip>

namespace ode::core {

    void Solution::export_to_csv(const std::string& filepath) const {
        std::ofstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filepath);
        }

        // Заголовки: time, y0, y1, ...
        file << "time";
        if (!states.empty()) {
            for (size_t i = 0; i < states[0].size(); ++i) {
                file << ",y" << i;
            }
        }
        file << "\n";

        // Данные
        file << std::fixed << std::setprecision(8);
        for (size_t i = 0; i < times.size(); ++i) {
            file << times[i];
            for (const auto& val : states[i]) {
                file << "," << val;
            }
            file << "\n";
        }
    }

}