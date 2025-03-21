#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <stdexcept> // Para manejo de excepciones

using namespace std;

// Constantes (UPPER_SNAKE_CASE)
const int CELULA_VIVA = 1;
const int CELULA_MUERTA = 0;

// Declaración de funciones (snake_case)
void inicializar_tablero(vector<vector<int>>& tablero, int tamaño, bool predefinido);
int calcular_vecinos_vivos(const vector<vector<int>>& tablero, int fila, int columna, int tamaño);
void actualizar_tablero(vector<vector<int>>& tablero, int tamaño);
void imprimir_tablero(const vector<vector<int>>& tablero, int tamaño);
void limpiar_pantalla();

int main() {
    int tamaño, generaciones;
    char opcion;

    try {
        // Entrada del tamaño del tablero
        cout << "Ingrese tamaño del tablero: ";
        cin >> tamaño;

        if (tamaño < 4) {
            throw invalid_argument("El tamaño del tablero debe ser al menos 4x4.");
        }

        // Entrada del número de generaciones
        cout << "Ingrese número de generaciones: ";
        cin >> generaciones;

        // Elección de patrón predefinido
        cout << "¿Desea usar un patrón predefinido? (s/n): ";
        cin >> opcion;
        bool predefinido = (opcion == 's' || opcion == 'S');

        // Creación e inicialización del tablero
        vector<vector<int>> tablero(tamaño, vector<int>(tamaño, CELULA_MUERTA));
        inicializar_tablero(tablero, tamaño, predefinido);

        // Simulación del Juego de la Vida
        for (int i = 0; i < generaciones; ++i) {
            imprimir_tablero(tablero, tamaño);
            actualizar_tablero(tablero, tamaño);
            this_thread::sleep_for(chrono::milliseconds(500)); // Pausa entre generaciones
            limpiar_pantalla(); // Limpieza de la pantalla
        }
    }
    catch (const invalid_argument& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}

// Función que inicializa el tablero con patrón predefinido o aleatorio
void inicializar_tablero(vector<vector<int>>& tablero, int tamaño, bool predefinido) {
    if (predefinido) {
        if (tamaño < 4) {
            throw invalid_argument("El tablero es muy pequeño para el patrón predefinido.");
        }
        // Inicializar con un "planeador"
        tablero[1][2] = CELULA_VIVA;
        tablero[2][3] = CELULA_VIVA;
        tablero[3][1] = CELULA_VIVA;
        tablero[3][2] = CELULA_VIVA;
        tablero[3][3] = CELULA_VIVA;
    }
    else {
        srand(static_cast<unsigned int>(chrono::steady_clock::now().time_since_epoch().count()));
        for (int i = 0; i < tamaño; ++i) {
            for (int j = 0; j < tamaño; ++j) {
                tablero[i][j] = rand() % 2;
            }
        }
    }
}

// Calcula la cantidad de vecinos vivos alrededor de una celda
int calcular_vecinos_vivos(const vector<vector<int>>& tablero, int fila, int columna, int tamaño) {
    int vecinos = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue; // Ignorar la celda actual
            int x = fila + i;
            int y = columna + j;
            if (x >= 0 && x < tamaño && y >= 0 && y < tamaño) {
                vecinos += tablero[x][y];
            }
        }
    }
    return vecinos;
}

// Aplica las reglas del Juego de la Vida y actualiza el tablero
void actualizar_tablero(vector<vector<int>>& tablero, int tamaño) {
    vector<vector<int>> buffer = tablero; // Copia temporal del tablero
    for (int i = 0; i < tamaño; ++i) {
        for (int j = 0; j < tamaño; ++j) {
            int vecinos = calcular_vecinos_vivos(tablero, i, j, tamaño);
            if (tablero[i][j] == CELULA_VIVA) {
                buffer[i][j] = (vecinos == 2 || vecinos == 3) ? CELULA_VIVA : CELULA_MUERTA;
            }
            else {
                buffer[i][j] = (vecinos == 3) ? CELULA_VIVA : CELULA_MUERTA;
            }
        }
    }
    tablero.swap(buffer); // Actualizar el tablero original
}

// Imprime el tablero en consola
void imprimir_tablero(const vector<vector<int>>& tablero, int tamaño) {
    for (int i = 0; i < tamaño; ++i) {
        for (int j = 0; j < tamaño; ++j) {
            cout << (tablero[i][j] == CELULA_VIVA ? "■ " : "· ");
        }
        cout << endl;
    }
    cout << endl; // Espaciado entre generaciones
}

// Limpia la pantalla sin usar system("cls") ni system("clear")
void limpiar_pantalla() {
    cout << "\033[2J\033[1;1H"; // Código ANSI para limpiar la pantalla
}
