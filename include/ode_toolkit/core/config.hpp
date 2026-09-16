#pragma once
#include "types.hpp"
#include <string>
#include <vector>

namespace ode::core 
{

    struct SolverConfig 
    {
        std::string method;           // "euler", "rk4", etc.
        std::string ode_id;           // ID предопределённого ОДУ
        std::vector<double> params;   // Параметры ОДУ (если нужны)
        State y0;                     // Начальные условия
        Time t0;                      // Начало интервала
        Time t_end;                   // Конец интервала
        Time dt;                      // Шаг
        std::string output_file;      // Путь к выходному CSV
    };

    // Загрузка конфигурации из JSON
    SolverConfig load_config_from_json(const std::string& filepath);
    
    // Сохранение конфигурации в JSON (для отладки)
    void save_config_to_json(const SolverConfig& config, const std::string& filepath);

}