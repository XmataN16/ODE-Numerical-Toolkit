#include "ode_toolkit/core/config.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

namespace ode::core {

    // Простой парсер JSON
    SolverConfig load_config_from_json(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open config file: " + filepath);
        }

        SolverConfig config;
        std::string line;
        
        while (std::getline(file, line)) {
            // Простой парсинг ключ-значение
            if (line.find("\"method\"") != std::string::npos) {
                size_t start = line.find(":") + 1;
                size_t end = line.find("\"", start + 1);
                config.method = line.substr(start + 2, end - start - 2);
            }
            else if (line.find("\"ode_id\"") != std::string::npos) {
                size_t start = line.find(":") + 1;
                size_t end = line.find("\"", start + 1);
                config.ode_id = line.substr(start + 2, end - start - 2);
            }
            else if (line.find("\"y0\"") != std::string::npos) {
                // Парсим массив [1.0, 2.0]
                size_t start = line.find("[") + 1;
                size_t end = line.find("]");
                std::string arr_str = line.substr(start, end - start);
                
                std::stringstream ss(arr_str);
                std::string token;
                while (std::getline(ss, token, ',')) {
                    config.y0.push_back(std::stod(token));
                }
            }
            else if (line.find("\"t0\"") != std::string::npos) {
                size_t start = line.find(":") + 1;
                config.t0 = std::stod(line.substr(start));
            }
            else if (line.find("\"t_end\"") != std::string::npos) {
                size_t start = line.find(":") + 1;
                config.t_end = std::stod(line.substr(start));
            }
            else if (line.find("\"dt\"") != std::string::npos) {
                size_t start = line.find(":") + 1;
                config.dt = std::stod(line.substr(start));
            }
            else if (line.find("\"output_file\"") != std::string::npos) {
                size_t start = line.find(":") + 1;
                size_t end = line.find("\"", start + 1);
                config.output_file = line.substr(start + 2, end - start - 2);
            }
        }

        return config;
    }

    void save_config_to_json(const SolverConfig& config, const std::string& filepath) {
        std::ofstream file(filepath);
        file << "{\n";
        file << "  \"method\": \"" << config.method << "\",\n";
        file << "  \"ode_id\": \"" << config.ode_id << "\",\n";
        file << "  \"y0\": [";
        for (size_t i = 0; i < config.y0.size(); ++i) {
            file << config.y0[i];
            if (i < config.y0.size() - 1) file << ", ";
        }
        file << "],\n";
        file << "  \"t0\": " << config.t0 << ",\n";
        file << "  \"t_end\": " << config.t_end << ",\n";
        file << "  \"dt\": " << config.dt << ",\n";
        file << "  \"output_file\": \"" << config.output_file << "\"\n";
        file << "}\n";
    }

}