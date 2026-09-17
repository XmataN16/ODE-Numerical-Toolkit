#include "ode_toolkit/core/config.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

namespace ode::core {

SolverConfig load_config_from_json(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open config file: " + filepath);
    }
    
    // Читаем весь файл целиком
    std::string content((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    
    SolverConfig config;
    
    // Извлечение строкового значения
    auto extract_string = [&content](const std::string& key) -> std::string {
        std::string search_key = "\"" + key + "\"";
        size_t key_pos = content.find(search_key);
        if (key_pos == std::string::npos) return "";
        
        size_t colon = content.find(":", key_pos);
        if (colon == std::string::npos) return "";
        
        size_t start = content.find("\"", colon);
        if (start == std::string::npos) return "";
        start++;
        
        size_t end = content.find("\"", start);
        if (end == std::string::npos) return "";
        
        return content.substr(start, end - start);
    };
    
    // Извлечение числового значения
    auto extract_number = [&content](const std::string& key) -> double {
        std::string search_key = "\"" + key + "\"";
        size_t key_pos = content.find(search_key);
        if (key_pos == std::string::npos) return 0.0;
        
        size_t colon = content.find(":", key_pos);
        if (colon == std::string::npos) return 0.0;
        
        size_t start = colon + 1;
        while (start < content.size() && (content[start] == ' ' || content[start] == '\t' || content[start] == '\n' || content[start] == '\r')) {
            start++;
        }
        
        size_t end = start;
        while (end < content.size() && (std::isdigit(content[end]) || content[end] == '.' || content[end] == '-' || content[end] == 'e' || content[end] == 'E' || content[end] == '+')) {
            end++;
        }
        
        if (start == end) return 0.0;
        return std::stod(content.substr(start, end - start));
    };
    
    // Извлечение массива
    auto extract_array = [&content](const std::string& key) -> std::vector<double> {
        std::vector<double> result;
        std::string search_key = "\"" + key + "\"";
        size_t key_pos = content.find(search_key);
        if (key_pos == std::string::npos) return result;
        
        size_t colon = content.find(":", key_pos);
        if (colon == std::string::npos) return result;
        
        size_t start = content.find("[", colon);
        if (start == std::string::npos) return result;
        
        size_t end = content.find("]", start);
        if (end == std::string::npos) return result;
        
        std::string arr_str = content.substr(start + 1, end - start - 1);
        
        std::stringstream ss(arr_str);
        std::string token;
        while (std::getline(ss, token, ',')) {
            size_t first = token.find_first_not_of(" \t\n\r");
            if (first == std::string::npos) continue;
            token = token.substr(first);
            size_t last = token.find_last_not_of(" \t\n\r");
            if (last != std::string::npos) {
                token = token.substr(0, last + 1);
            }
            
            if (!token.empty()) {
                try {
                    result.push_back(std::stod(token));
                } catch (...) {}
            }
        }
        
        return result;
    };
    
    config.method = extract_string("method");
    config.ode_id = extract_string("ode_id");
    config.params = extract_array("params");
    config.y0 = extract_array("y0");
    config.t0 = extract_number("t0");
    config.t_end = extract_number("t_end");
    config.dt = extract_number("dt");
    config.output_file = extract_string("output_file");
    
    // Заменяем \\ на /
    for (auto& ch : config.output_file) {
        if (ch == '\\') ch = '/';
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