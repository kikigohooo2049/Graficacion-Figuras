#include <iostream>
#include <vector>
#include <limits>
#include <string>

using namespace std;

// Dimensiones del plano donde luego se graficaran las figuras.
// x va de 0 a ANCHO-1, y va de 0 a ALTO-1.
const int ANCHO = 41;
const int ALTO  = 21;

// ---------------------------------------------------------
// Funciones auxiliares para leer datos validados
// ---------------------------------------------------------

// Lee un entero validado dentro de un rango [minimo, maximo]
int leerEntero(const string &mensaje, int minimo, int maximo) {
    int valor;
    while (true) {
        cout << mensaje << " (" << minimo << " a " << maximo << "): ";
        cin >> valor;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada invalida. Debe ingresar un numero entero.\n";
            continue;
        }
        if (valor < minimo || valor > maximo) {
            cout << "Valor fuera de rango. Intente de nuevo.\n";
            continue;
        }
        return valor;
    }
}

// Lee una coordenada (x, y) validada dentro de los limites del plano
void leerCoordenada(int &x, int &y, const string &etiqueta) {
    cout << "-- " << etiqueta << " --\n";
    x = leerEntero("  x", 0, ANCHO - 1);
    y = leerEntero("  y", 0, ALTO - 1);
}

// ---------------------------------------------------------
// Clases de figuras: solo capturan y guardan los datos
// ---------------------------------------------------------

class Linea {
public:
    int x1, y1, x2, y2;

    void ingresarDatos() {
        leerCoordenada(x1, y1, "Primer punto");
        leerCoordenada(x2, y2, "Segundo punto");
    }
};

class Polilinea {
public:
    vector<pair<int, int>> puntos;

    void ingresarDatos() {
        int n = leerEntero("Cuantos puntos tendra la polilinea", 2, 20);
        for (int i = 0; i < n; i++) {
            int x, y;
            leerCoordenada(x, y, "Punto " + to_string(i + 1));
            puntos.push_back({x, y});
        }
    }
};

class Circulo {
public:
    int cx, cy, radio;

    void ingresarDatos() {
        leerCoordenada(cx, cy, "Centro del circulo");
        // El radio maximo depende de que tan cerca esta el centro del borde del plano,
        // para que la circunferencia completa quepa sin salirse.
        int radioMax = min(min(cx, ANCHO - 1 - cx), min(cy, ALTO - 1 - cy));
        if (radioMax < 1) {
            cout << "Aviso: el centro esta muy cerca del borde. Radio maximo posible: 1\n";
            radioMax = 1;
        }
        radio = leerEntero("Radio", 1, radioMax);
    }
};

class Poligono {
public:
    vector<pair<int, int>> puntos;

    void ingresarDatos() {
        int n = leerEntero("Cuantos lados tendra el poligono", 3, 20);
        for (int i = 0; i < n; i++) {
            int x, y;
            leerCoordenada(x, y, "Vertice " + to_string(i + 1));
            puntos.push_back({x, y});
        }
    }
};

// ---------------------------------------------------------
// Programa principal: solo menu + captura de datos
// ---------------------------------------------------------
int main() {
    int opcion;

    do {
        cout << "----------------------------------\n";
        cout << "*****GRAFICADORA DE POLIGONOS*****\n";
        cout << "Plano disponible: x [0-" << ANCHO - 1 << "], y [0-" << ALTO - 1 << "]\n";
        cout << "Escoja una figura para crear:\n";
        cout << "1. Linea\n";
        cout << "2. Polilinea\n";
        cout << "3. Circulo\n";
        cout << "4. Poligono\n";
        cout << "0. Salir\n";
        opcion = leerEntero("Opcion", 0, 4);

        switch (opcion) {
            case 1: {
                Linea l;
                l.ingresarDatos();
                cout << "Datos capturados: (" << l.x1 << "," << l.y1 << ") - ("
                     << l.x2 << "," << l.y2 << ")\n";
                break;
            }
            case 2: {
                Polilinea p;
                p.ingresarDatos();
                cout << "Puntos capturados:";
                for (auto &pt : p.puntos) cout << " (" << pt.first << "," << pt.second << ")";
                cout << "\n";
                break;
            }
            case 3: {
                Circulo c;
                c.ingresarDatos();
                cout << "Centro: (" << c.cx << "," << c.cy << "), Radio: " << c.radio << "\n";
                break;
            }
            case 4: {
                Poligono pg;
                pg.ingresarDatos();
                cout << "Vertices capturados:";
                for (auto &pt : pg.puntos) cout << " (" << pt.first << "," << pt.second << ")";
                cout << "\n";
                break;
            }
            case 0:
                cout << "Saliendo...\n";
                break;
        }
    } while (opcion != 0);

    return 0;
}

	  
