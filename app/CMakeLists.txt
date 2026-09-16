add_executable(ode_num_app main.cpp)

# Линкуем
target_link_libraries(ode_num_app PRIVATE ode_num_core)