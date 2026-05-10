import tkinter as tk
from tkinter import messagebox
import subprocess
import os
import random
import time

class SopaTiyuy:
    def __init__(self, root):
        self.root = root
        self.root.title("Tiyuy Sopa de Letras - Motor C++")
        self.root.geometry("600x900")
        self.root.configure(bg="#1a1a2e")
        
        self.semilla = random.randint(1, 9999)
        self.jugando = True
        self.encontradas = 0
        self.datos_palabras = {}
        self.celdas = {}
        self.seleccion_actual = []
        self.t_inicio = time.time()
        
        self.setup_ui()
        self.conectar_cpp()
        self.run_reloj()

    def setup_ui(self):
        # Título principal
        tk.Label(self.root, text="SOPA DE LETRAS", font=("Arial", 26, "bold"), 
                bg="#1a1a2e", fg="#e94560").pack(pady=15)
        
        # Panel de estadísticas
        self.lbl_stats = tk.Label(self.root, text="Tiempo: 0s | Halladas: 0/4", 
                                font=("Arial", 12), bg="#16213e", fg="white", padx=10, pady=5)
        self.lbl_stats.pack(fill="x", padx=50)

        # Tablero de juego
        self.frame_tablero = tk.Frame(self.root, bg="#0f3460", padx=10, pady=10)
        self.frame_tablero.pack(pady=20)

        # Monitor de selección actual
        self.lbl_sel = tk.Label(self.root, text="Selección: ", font=("Arial", 16, "bold"), 
                                bg="#1a1a2e", fg="#f1c40f")
        self.lbl_sel.pack(pady=10)

        # Lista de palabras a buscar
        self.lbl_lista = tk.Label(self.root, text="", font=("Courier New", 11), 
                                bg="#1a1a2e", fg="#00d2ff")
        self.lbl_lista.pack()

        # Botón para limpiar clics fallidos
        tk.Button(self.root, text="LIMPIAR SELECCIÓN", command=self.reset_seleccion, 
                bg="#e94560", fg="white", font=("Arial", 10, "bold"), width=20).pack(pady=15)

    def conectar_cpp(self):
        # Llamamos al C++ para obtener la matriz y las coordenadas de las palabras
        ruta_exe = os.path.join(os.path.dirname(__file__), "sopa.exe")
        try:
            # Ejecución del motor C++ pasándole la semilla aleatoria
            res = subprocess.check_output([ruta_exe, str(self.semilla)], shell=True, text=True)
            
            # Separamos la matriz de los metadatos
            partes = res.strip().split("---DATA---")
            matriz_raw = partes[0].strip().split('\n')
            data_raw = partes[1].strip().split()

            # Procesamos la información de las palabras ocultas enviada por C++
            for item in data_raw:
                palabra, f, c, ori = item.split(',')
                posiciones = []
                # Reconstruimos las coordenadas según la orientación (Horiz/Vert)
                for k in range(len(palabra)):
                    row, col = (int(f)+k, int(c)) if int(ori) == 1 else (int(f), int(c)+k)
                    posiciones.append((row, col))
                self.datos_palabras[palabra] = {'pos': posiciones, 'hallada': False}

            # Dibujamos la cuadrícula en base a la matriz generada por C++
            for i, fila in enumerate(matriz_raw):
                for j, letra in enumerate(fila.split()):
                    lbl = tk.Label(self.frame_tablero, text=letra, font=("Arial", 14, "bold"), 
                                width=2, height=1, bg="#16213e", fg="white", cursor="hand2")
                    lbl.grid(row=i, column=j, padx=2, pady=2)
                    # Vinculamos el clic del mouse con la lógica de selección
                    lbl.bind("<Button-1>", lambda e, r=i, c=j: self.gestionar_clic(r, c))
                    self.celdas[(i, j)] = lbl
            
            self.lbl_lista.config(text="BUSCAR: " + " | ".join(self.datos_palabras.keys()))
        except Exception as e:
            messagebox.showerror("Error de Sistema", f"No se encontró 'sopa.exe'.\nCompila el código C++ primero.")

    def gestionar_clic(self, r, c):
        if not self.jugando: return
        
        # Agregamos la letra seleccionada si no estaba ya en la lista actual
        if (r, c) not in self.seleccion_actual:
            self.seleccion_actual.append((r, c))
            self.celdas[(r, c)].config(bg="#f1c40f", fg="black") # Color de realce temporal
            
            # Construimos la palabra acumulada
            palabra_actual = "".join([self.celdas[pos]["text"] for pos in self.seleccion_actual])
            self.lbl_sel.config(text=f"Selección: {palabra_actual}")
            
            # Verificamos contra la base de datos de C++
            self.validar_hallazgo(palabra_actual)

    def validar_hallazgo(self, palabra):
        if palabra in self.datos_palabras and not self.datos_palabras[palabra]['hallada']:
            # Si es correcta, marcamos permanentemente
            self.datos_palabras[palabra]['hallada'] = True
            self.encontradas += 1
            for r, c in self.seleccion_actual:
                self.celdas[(r, c)].config(bg="#00d2ff", fg="black") # Azul para halladas
            
            self.reset_seleccion()
            self.actualizar_texto_stats()
            
            if self.encontradas == len(self.datos_palabras):
                self.invocar_reporte_cpp()

    def reset_seleccion(self):
        # Limpiamos el color de las letras que no forman una palabra correcta
        for r, c in self.seleccion_actual:
            if self.celdas[(r, c)]["bg"] == "#f1c40f":
                self.celdas[(r, c)].config(bg="#16213e", fg="white")
        self.seleccion_actual = []
        self.lbl_sel.config(text="Selección: ")

    def actualizar_texto_stats(self):
        t_transcurrido = int(time.time() - self.t_inicio)
        self.lbl_stats.config(text=f"Tiempo: {t_transcurrido}s | Halladas: {self.encontradas}/4")

    def run_reloj(self):
        if self.jugando:
            self.actualizar_texto_stats()
            self.root.after(1000, self.run_reloj)

    def invocar_reporte_cpp(self):
        self.jugando = False
        t_final = int(time.time() - self.t_inicio)
        
        # Aquí es donde C++ genera la lógica del reporte final
        ruta_exe = os.path.join(os.path.dirname(__file__), "sopa.exe")
        try:
            # Enviamos semilla, tiempo y cantidad hallada al ejecutable C++
            reporte_raw = subprocess.check_output([ruta_exe, str(self.semilla), str(t_final), str(self.encontradas)], 
                                                shell=True, text=True)
            # Extraemos solo la sección del reporte definida en C++
            mensaje_final = reporte_raw.split("---REPORTE---")[1].strip()
            messagebox.showinfo("Reporte del Motor C++", mensaje_final)
        except:
            messagebox.showinfo("Fin del Juego", f"¡Ganaste!\nTiempo: {t_final}s")

if __name__ == "__main__":
    root = tk.Tk()
    app = SopaTiyuy(root)
    root.mainloop()