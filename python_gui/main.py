import tkinter as tk
from tkinter import ttk, messagebox
import json
import subprocess
import os
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import pandas as pd


class ODESolverGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("ODE Numerical Toolkit")
        self.root.geometry("900x700")
        
        # Абсолютные пути
        self.script_dir = os.path.dirname(os.path.abspath(__file__))
        self.project_dir = os.path.dirname(self.script_dir)
        
        # Путь к C++ исполняемому файлу
        self.cpp_executable = os.path.join(
            self.project_dir, 
            "build", 
            "mingw-debug", 
            "app", 
            "ode_num_app.exe"
        )
        
        print(f"Project dir: {self.project_dir}")
        print(f"CPP executable: {self.cpp_executable}")
        print(f"Executable exists: {os.path.exists(self.cpp_executable)}")
        
        self.create_widgets()

    def create_widgets(self):
        # Панель управления
        control_frame = ttk.Frame(self.root, padding="10")
        control_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))

        # Метод
        ttk.Label(control_frame, text="Method:").grid(row=0, column=0, sticky=tk.W, pady=5)
        self.method_var = tk.StringVar(value="euler")
        method_combo = ttk.Combobox(control_frame, textvariable=self.method_var, width=20)
        method_combo['values'] = ('euler', 'rk4', 'implicit_euler')
        method_combo.grid(row=0, column=1, sticky=tk.W, pady=5)

        # ОДУ
        ttk.Label(control_frame, text="ODE:").grid(row=1, column=0, sticky=tk.W, pady=5)
        self.ode_var = tk.StringVar(value="exp_decay")
        ode_combo = ttk.Combobox(control_frame, textvariable=self.ode_var, width=20)
        ode_combo['values'] = ('exp_decay', 'exp_growth', 'param_decay', 'harmonic')
        ode_combo.grid(row=1, column=1, sticky=tk.W, pady=5)

        # Параметры ОДУ
        ttk.Label(control_frame, text="ODE params:").grid(row=2, column=0, sticky=tk.W, pady=5)
        self.params_var = tk.StringVar(value="")
        ttk.Entry(control_frame, textvariable=self.params_var, width=20).grid(row=2, column=1, sticky=tk.W, pady=5)

        # Начальные условия
        ttk.Label(control_frame, text="y0:").grid(row=3, column=0, sticky=tk.W, pady=5)
        self.y0_var = tk.StringVar(value="1.0")
        ttk.Entry(control_frame, textvariable=self.y0_var, width=20).grid(row=3, column=1, sticky=tk.W, pady=5)

        # Временной интервал
        ttk.Label(control_frame, text="t0:").grid(row=4, column=0, sticky=tk.W, pady=5)
        self.t0_var = tk.StringVar(value="0.0")
        ttk.Entry(control_frame, textvariable=self.t0_var, width=10).grid(row=4, column=1, sticky=tk.W, pady=5)

        ttk.Label(control_frame, text="t_end:").grid(row=5, column=0, sticky=tk.W, pady=5)
        self.t_end_var = tk.StringVar(value="2.0")
        ttk.Entry(control_frame, textvariable=self.t_end_var, width=10).grid(row=5, column=1, sticky=tk.W, pady=5)

        # Шаг
        ttk.Label(control_frame, text="dt:").grid(row=6, column=0, sticky=tk.W, pady=5)
        self.dt_var = tk.StringVar(value="0.01")
        ttk.Entry(control_frame, textvariable=self.dt_var, width=10).grid(row=6, column=1, sticky=tk.W, pady=5)

        # Кнопка запуска
        self.run_button = ttk.Button(control_frame, text="Run Solver", command=self.run_solver)
        self.run_button.grid(row=7, column=0, columnspan=2, pady=20)

        # Область для графика
        self.fig, self.ax = plt.subplots(figsize=(8, 6))
        self.canvas = FigureCanvasTkAgg(self.fig, master=self.root)
        self.canvas.get_tk_widget().grid(row=0, column=1, rowspan=8, sticky=(tk.W, tk.E, tk.N, tk.S))

    def run_solver(self):
        try:
            # Парсим параметры
            params_str = self.params_var.get().strip()
            params = [float(x) for x in params_str.split(',') if x.strip()] if params_str else []

            # Абсолютный путь к выходному файлу
            output_file = os.path.join(self.script_dir, "temp_result.csv")
            config_file = os.path.join(self.script_dir, "temp_config.json")

            # Собираем конфигурацию
            config = {
                "method": self.method_var.get(),
                "ode_id": self.ode_var.get(),
                "params": params,
                "y0": [float(x) for x in self.y0_var.get().split(',')],
                "t0": float(self.t0_var.get()),
                "t_end": float(self.t_end_var.get()),
                "dt": float(self.dt_var.get()),
                "output_file": output_file
            }

            # Сохраняем JSON
            with open(config_file, 'w') as f:
                json.dump(config, f, separators=(',', ':'))

            print(f"\n=== Запуск решателя ===")
            print(f"Config file: {config_file}")
            print(f"Output file: {output_file}")
            print(f"CPP executable: {self.cpp_executable}")
            print(f"Config: {config}")

            # Проверяем существование exe
            if not os.path.exists(self.cpp_executable):
                messagebox.showerror("Error", f"C++ executable not found:\n{self.cpp_executable}")
                return

            # Запускаем C++ программу
            result = subprocess.run(
                [self.cpp_executable, config_file],
                capture_output=True,
                text=True,
                cwd=self.script_dir
            )

            print(f"\nReturn code: {result.returncode}")
            print(f"STDOUT:\n{result.stdout}")
            if result.stderr:
                print(f"STDERR:\n{result.stderr}")

            if result.returncode != 0:
                messagebox.showerror("C++ Error", f"Solver failed with code {result.returncode}:\n{result.stderr}")
                return

            # Проверяем, создан ли CSV
            if not os.path.exists(output_file):
                messagebox.showerror("Error", f"Output file not created:\n{output_file}\n\nCheck console for details")
                return

            # Строим график
            self.plot_result(output_file)
            messagebox.showinfo("Success", f"Solution computed successfully!\nPoints: {len(pd.read_csv(output_file))}")

        except ValueError as e:
            messagebox.showerror("Input Error", f"Неверный формат числа:\n{e}")
        except Exception as e:
            messagebox.showerror("Error", f"Unexpected error:\n{e}")
            import traceback
            traceback.print_exc()

    def plot_result(self, csv_file):
        df = pd.read_csv(csv_file)

        self.ax.clear()
        self.ax.plot(df['time'], df['y0'], label='y(t)', linewidth=2)

        if 'y1' in df.columns:
            self.ax.plot(df['time'], df['y1'], label="y'(t)", linewidth=2, linestyle='--')

        self.ax.set_xlabel('Time')
        self.ax.set_ylabel('Solution')
        self.ax.set_title('ODE Solution')
        self.ax.legend()
        self.ax.grid(True)

        self.canvas.draw()


if __name__ == "__main__":
    root = tk.Tk()
    app = ODESolverGUI(root)
    root.mainloop()