#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>

using namespace std;

// Estructura para coordenadas
struct Palabra {
    string texto;
    int f, c, ori;
    bool hallada;
};

class JuegoSopa {
private:
    char** tablero;
    int tam;
    Palabra elegidas[4];
    string banco[8] = {"LIMA", "PERU", "SOPA", "JAVA", "PYTHON", "PROGRAMA", "CODIGO", "GRUPO"};

public:
    // Constructor con Gestión de Memoria Dinámica (Punteros)
    JuegoSopa(int _tam, int semilla) {
        tam = _tam;
        srand(semilla);
        tablero = new char*[tam];
        for (int i = 0; i < tam; i++) {
            tablero[i] = new char[tam];
            for (int j = 0; j < tam; j++) tablero[i][j] = '-';
        }
    }

    void generar() {
        int colocadas = 0;
        bool usadas[8] = {false};
        while (colocadas < 4) {
            int idx = rand() % 8;
            if (usadas[idx]) continue;

            string p = banco[idx];
            int ori = rand() % 2;
            int f = rand() % tam;
            int c = rand() % tam;

            if (ori == 0 && c + p.length() <= tam) { // Horizontal
                bool choque = false;
                for (int j = 0; j < p.length(); j++) if (tablero[f][c+j] != '-') choque = true;
                if (!choque) {
                    for (int j = 0; j < p.length(); j++) tablero[f][c+j] = p[j];
                    elegidas[colocadas] = {p, f, c, 0, false};
                    usadas[idx] = true; colocadas++;
                }
            } else if (ori == 1 && f + p.length() <= tam) { // Vertical
                bool choque = false;
                for (int j = 0; j < p.length(); j++) if (tablero[f+j][c] != '-') choque = true;
                if (!choque) {
                    for (int j = 0; j < p.length(); j++) tablero[f+j][c] = p[j];
                    elegidas[colocadas] = {p, f, c, 1, false};
                    usadas[idx] = true; colocadas++;
                }
            }
        }
        rellenar();
    }

    void rellenar() {
        for (int i = 0; i < tam; i++)
            for (int j = 0; j < tam; j++)
                if (tablero[i][j] == '-') tablero[i][j] = 'A' + (rand() % 26);
    }

    void imprimirTablero() {
        for (int i = 0; i < tam; i++) {
            for (int j = 0; j < tam; j++) cout << tablero[i][j] << " ";
            cout << endl;
        }
    }

    void imprimirData() {
        cout << "---DATA---" << endl;
        for (int i = 0; i < 4; i++) {
            cout << elegidas[i].texto << "," << elegidas[i].f << "," << elegidas[i].c << "," << elegidas[i].ori << " ";
        }
    }

    // Lógica de Reporte en C++
    void generarReporte(int tiempo, int encontradas) {
        cout << "---REPORTE---" << endl;
        cout << "REPORTE FINAL DEL SISTEMA TIYUY" << endl;
        cout << "Tiempo de ejecucion: " << tiempo << " segundos." << endl;
        cout << "Palabras completadas: " << encontradas << " de 4." << endl;
        if (encontradas == 4) cout << "Estado: EXCELENTE DESEMPEÑO" << endl;
        else cout << "Estado: NECESITA PRACTICA" << endl;
    }

    ~JuegoSopa() {
        for (int i = 0; i < tam; i++) delete[] tablero[i];
        delete[] tablero;
    }
};

int main(int argc, char* argv[]) {
    // Si Python pide reporte: sopa.exe <semilla> <tiempo> <encontradas>
    if (argc >= 4) {
        JuegoSopa j(10, atoi(argv[1]));
        j.generarReporte(atoi(argv[2]), atoi(argv[3]));
        return 0;
    }
    
    int semilla = (argc >= 2) ? atoi(argv[1]) : time(NULL);
    JuegoSopa j(10, semilla);
    j.generar();
    j.imprimirTablero();
    j.imprimirData();

    return 0;
}