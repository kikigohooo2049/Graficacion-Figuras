#include "raylib.h"
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <functional>

using namespace std;


const int ANCHO_VENTANA = 1200;
const int ALTO_VENTANA = 800;
const int MARGEN_SUPERIOR = 90;
const int MARGEN_INFERIOR = 50;
const float ESCALA = 20.0f;

const Vector2 origenPantalla = {
    ANCHO_VENTANA / 2.0f,
    MARGEN_SUPERIOR + (ALTO_VENTANA - MARGEN_INFERIOR - MARGEN_SUPERIOR) / 2.0f
};

int VentanaMenu(void);
void VentanaLinea(void);
void VentanaPolilinea(void);
void VentanaCirculo(void);
void VentanaPoligono(void);
int VentanaMenuPoligono(void);
void DibujarCuadrado(void);
void DibujarRectangulo(void);
void DibujarTriangulo(void);
void DibujarPoligonoIrregular(void);
void TextoAutores(void);

Vector2 CartesianoAPantalla(Vector2 c)
{
    return { origenPantalla.x + c.x * ESCALA, origenPantalla.y - c.y * ESCALA };
}

Vector2 PantallaACartesiano(Vector2 p)
{
    return { (p.x - origenPantalla.x) / ESCALA, (origenPantalla.y - p.y) / ESCALA };
}

Vector2 SnapACartesiano(Vector2 pantalla)
{
    Vector2 c = PantallaACartesiano(pantalla);
    c.x = roundf(c.x);
    c.y = roundf(c.y);
    return c;
}

bool DentroDeAreaDibujo(Vector2 pantalla)
{
    return pantalla.x >= 0 && pantalla.x <= ANCHO_VENTANA &&
           pantalla.y >= MARGEN_SUPERIOR && pantalla.y <= ALTO_VENTANA - MARGEN_INFERIOR;
}

bool MismoPunto(Vector2 a, Vector2 b)
{
    return fabs(a.x - b.x) < 0.01f && fabs(a.y - b.y) < 0.01f;
}

void DibujarPlanoCartesiano(void)
{
    int yTop = MARGEN_SUPERIOR;
    int yBottom = ALTO_VENTANA - MARGEN_INFERIOR;

    DrawRectangle(0, yTop, ANCHO_VENTANA, yBottom - yTop, Color{ 250, 250, 250, 255 });

    for (int x = 0; x < 1000; x++) {
        Vector2 posDer = CartesianoAPantalla({ (float)x, 0 });
        if (posDer.x > ANCHO_VENTANA) break;
        Color color = (x % 5 == 0) ? LIGHTGRAY : Fade(LIGHTGRAY, 0.5f);
        DrawLine((int)posDer.x, yTop, (int)posDer.x, yBottom, color);
        if (x != 0) {
            Vector2 posIzq = CartesianoAPantalla({ (float)-x, 0 });
            if (posIzq.x >= 0) DrawLine((int)posIzq.x, yTop, (int)posIzq.x, yBottom, color);
        }
    }

    for (int y = 0; y < 1000; y++) {
        Vector2 posAbajo = CartesianoAPantalla({ 0, (float)-y });
        if (posAbajo.y > yBottom) break;
        Color color = (y % 5 == 0) ? LIGHTGRAY : Fade(LIGHTGRAY, 0.5f);
        DrawLine(0, (int)posAbajo.y, ANCHO_VENTANA, (int)posAbajo.y, color);
        if (y != 0) {
            Vector2 posArriba = CartesianoAPantalla({ 0, (float)y });
            if (posArriba.y >= yTop) DrawLine(0, (int)posArriba.y, ANCHO_VENTANA, (int)posArriba.y, color);
        }
    }

    DrawLine(0, (int)origenPantalla.y, ANCHO_VENTANA, (int)origenPantalla.y, DARKGRAY);
    DrawLine((int)origenPantalla.x, yTop, (int)origenPantalla.x, yBottom, DARKGRAY);
    DrawText("0,0", (int)origenPantalla.x + 4, (int)origenPantalla.y + 4, 12, GRAY);
}


bool SonColineales(Vector2 a, Vector2 b, Vector2 c)
{
    float cruz = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    return fabs(cruz) < 0.01f;
}

int Orientacion(Vector2 p, Vector2 q, Vector2 r)
{
    float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (fabs(val) < 0.0001f) return 0;
    return (val > 0) ? 1 : 2;
}

bool EnSegmento(Vector2 p, Vector2 q, Vector2 r)
{
    return q.x <= fmax(p.x, r.x) && q.x >= fmin(p.x, r.x) &&
           q.y <= fmax(p.y, r.y) && q.y >= fmin(p.y, r.y);
}

bool SegmentosSeCruzan(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4)
{
    int o1 = Orientacion(p1, p2, p3);
    int o2 = Orientacion(p1, p2, p4);
    int o3 = Orientacion(p3, p4, p1);
    int o4 = Orientacion(p3, p4, p2);

    if (o1 != o2 && o3 != o4) return true;

    if (o1 == 0 && EnSegmento(p1, p3, p2)) return true;
    if (o2 == 0 && EnSegmento(p1, p4, p2)) return true;
    if (o3 == 0 && EnSegmento(p3, p1, p4)) return true;
    if (o4 == 0 && EnSegmento(p3, p2, p4)) return true;

    return false;
}

bool NuevoSegmentoValido(const vector<Vector2>& v, Vector2 nuevo)
{
    if (v.size() < 2) return true;
    Vector2 a = v.back();
    Vector2 b = nuevo;
    int n = (int)v.size();
    for (int i = 0; i < n - 1; i++) {
        if (i == n - 2) continue;
        if (SegmentosSeCruzan(a, b, v[i], v[i + 1])) return false;
    }
    return true;
}


bool CierreValido(const vector<Vector2>& v)
{
    int n = (int)v.size();
    if (n < 3) return false;
    Vector2 a = v.back();
    Vector2 b = v.front();
    for (int i = 0; i < n - 1; i++) {
        if (i == 0) continue;
        if (i == n - 2) continue;
        if (SegmentosSeCruzan(a, b, v[i], v[i + 1])) return false;
    }
    return true;
}

void TextoAutores(void)
{
    const char* texto = "Hecho por Daniel, Keith y Alexis";
    int fontSize = 14;
    int margen = 10;
    int textWidth = MeasureText(texto, fontSize);
    int x = margen;
    int y = GetScreenHeight() - fontSize - margen;

    DrawRectangle(x - 4, y - 4, textWidth + 8, fontSize + 8, Fade(BLACK, 0.15f));
    DrawText(texto, x, y, fontSize, Fade(GRAY, 0.8f));
}



Vector2 PedirPunto(const string& mensaje, function<bool(Vector2)> validar,
                    const string& errorSiInvalido,
                    const vector<Vector2>& puntosGuia,
                    const vector<pair<Vector2, Vector2>>& segmentosGuia,
                    bool& cancelado)
{
    Vector2 resultado = { 0, 0 };
    cancelado = false;

    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();
        bool dentro = DentroDeAreaDibujo(mouse);
        Vector2 cartActual = SnapACartesiano(mouse);
        Vector2 pantallaSnap = CartesianoAPantalla(cartActual);
        bool valido = dentro && validar(cartActual);

        if (valido && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            resultado = cartActual;
            break;
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DibujarPlanoCartesiano();

            for (size_t i = 0; i < segmentosGuia.size(); i++)
                DrawLineV(CartesianoAPantalla(segmentosGuia[i].first),
                          CartesianoAPantalla(segmentosGuia[i].second), BLUE);
            for (size_t i = 0; i < puntosGuia.size(); i++)
                DrawCircleV(CartesianoAPantalla(puntosGuia[i]), 4, RED);

            if (dentro) {
                Color colorPreview = valido ? Fade(GREEN, 0.6f) : Fade(RED, 0.6f);
                DrawCircleV(pantallaSnap, 4, colorPreview);
                string coordTxt = "(" + to_string((int)cartActual.x) + ", " + to_string((int)cartActual.y) + ")";
                DrawText(coordTxt.c_str(), (int)pantallaSnap.x + 8, (int)pantallaSnap.y - 8, 14, DARKGRAY);
            }

            DrawText(mensaje.c_str(), 20, 20, 18, DARKGRAY);
            if (dentro && !valido && !errorSiInvalido.empty())
                DrawText(errorSiInvalido.c_str(), 20, 45, 16, RED);

            TextoAutores();
        EndDrawing();
    }

    if (WindowShouldClose()) cancelado = true;
    return resultado;
}

int LeerEntero(const string& mensaje, int minVal, int maxVal, bool& cancelado)
{
    string entrada = "";
    cancelado = false;

    while (!WindowShouldClose()) {
        int tecla = GetCharPressed();
        while (tecla > 0) {
            if (tecla >= '0' && tecla <= '9' && entrada.size() < 3) entrada += (char)tecla;
            tecla = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && !entrada.empty()) entrada.pop_back();

        int valorActual = entrada.empty() ? (minVal - 1) : atoi(entrada.c_str());
        bool enRango = valorActual >= minVal && valorActual <= maxVal;

        if (IsKeyPressed(KEY_ENTER) && enRango) return valorActual;

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DibujarPlanoCartesiano();
            DrawText(mensaje.c_str(), 20, 20, 18, DARKGRAY);
            string rango = "Valor entre " + to_string(minVal) + " y " + to_string(maxVal) + " (ENTER para confirmar)";
            DrawText(rango.c_str(), 20, 45, 16, GRAY);
            string mostrar = entrada.empty() ? "_" : entrada;
            DrawText(mostrar.c_str(), 20, 90, 30, DARKBLUE);
            if (!entrada.empty() && !enRango) DrawText("Fuera de rango", 150, 95, 20, RED);
            TextoAutores();
        EndDrawing();
    }

    cancelado = true;
    return minVal;
}

int main(void)
{
    int opcion;
    do {
        opcion = VentanaMenu();
        switch (opcion) {
            case 1: VentanaLinea(); break;
            case 2: VentanaPolilinea(); break;
            case 3: VentanaCirculo(); break;
            case 4: VentanaPoligono(); break;
            case 5: break;
        }
    } while (opcion != 5 && opcion != 0);

    return 0;
}

int VentanaMenu(void)
{
    InitWindow(ANCHO_VENTANA, ALTO_VENTANA, "Menu");
    SetTargetFPS(60);

    int seleccion = 0;

    while (!WindowShouldClose() && seleccion == 0) {
        if (IsKeyPressed(KEY_ONE))   seleccion = 1;
        if (IsKeyPressed(KEY_TWO))   seleccion = 2;
        if (IsKeyPressed(KEY_THREE)) seleccion = 3;
        if (IsKeyPressed(KEY_FOUR))  seleccion = 4;
        if (IsKeyPressed(KEY_FIVE))  seleccion = 5;

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Elige una figura:", 20, 20, 20, DARKGRAY);
            DrawText("1 - Linea",     40,  80, 20, BLACK);
            DrawText("2 - Polilinea", 40, 110, 20, BLACK);
            DrawText("3 - Circulo",   40, 140, 20, BLACK);
            DrawText("4 - Poligono",  40, 170, 20, BLACK);
            DrawText("5 - Salir",     40, 200, 20, MAROON);
            TextoAutores();
        EndDrawing();
    }

    CloseWindow();
    return seleccion;
}


void VentanaLinea(void)
{
    InitWindow(ANCHO_VENTANA, ALTO_VENTANA, "1. Dibujo de Linea");
    SetTargetFPS(60);

    bool cancelado;
    Vector2 p1 = PedirPunto("Linea: clic para el primer punto",
                             [](Vector2) { return true; }, "", {}, {}, cancelado);
    if (cancelado) { CloseWindow(); return; }

    Vector2 p2 = PedirPunto("Linea: clic para el segundo punto (distinto al primero)",
                             [p1](Vector2 c) { return !MismoPunto(c, p1); },
                             "El segundo punto debe ser distinto al primero",
                             { p1 }, {}, cancelado);
    if (cancelado) { CloseWindow(); return; }

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DibujarPlanoCartesiano();
            DrawLineV(CartesianoAPantalla(p1), CartesianoAPantalla(p2), RED);
            DrawCircleV(CartesianoAPantalla(p1), 4, DARKGRAY);
            DrawCircleV(CartesianoAPantalla(p2), 4, DARKGRAY);
            DrawText("Ventana 1: Linea (cierra la ventana para continuar)", 20, 20, 18, DARKGRAY);
            TextoAutores();
        EndDrawing();
    }
    CloseWindow();
}

void VentanaPolilinea(void)
{
    InitWindow(ANCHO_VENTANA, ALTO_VENTANA, "2. Dibujo de Polilinea");
    SetTargetFPS(60);

    bool cancelado;
    int cantidad = LeerEntero("Polilinea: cuantos puntos tendra? (2 a 10)", 2, 10, cancelado);
    if (cancelado) { CloseWindow(); return; }

    vector<Vector2> puntos;
    for (int i = 0; i < cantidad; i++) {
        string msg = "Polilinea: clic para el punto " + to_string(i + 1) + " de " + to_string(cantidad);

        vector<pair<Vector2, Vector2>> segmentos;
        for (size_t j = 0; j + 1 < puntos.size(); j++) segmentos.push_back({ puntos[j], puntos[j + 1] });

        Vector2 p = PedirPunto(msg,
            [&puntos](Vector2 c) {
                for (size_t k = 0; k < puntos.size(); k++)
                    if (MismoPunto(puntos[k], c)) return false;
                return true;
            },
            "Ese punto ya fue usado",
            puntos, segmentos, cancelado);
        if (cancelado) { CloseWindow(); return; }
        puntos.push_back(p);
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DibujarPlanoCartesiano();
            for (size_t i = 0; i + 1 < puntos.size(); i++)
                DrawLineV(CartesianoAPantalla(puntos[i]), CartesianoAPantalla(puntos[i + 1]), BLUE);
            for (size_t i = 0; i < puntos.size(); i++)
                DrawCircleV(CartesianoAPantalla(puntos[i]), 4, DARKGRAY);
            DrawText("Ventana 2: Polilinea (cierra la ventana para continuar)", 20, 20, 18, DARKGRAY);
            TextoAutores();
        EndDrawing();
    }
    CloseWindow();
}

void VentanaCirculo(void)
{
    InitWindow(ANCHO_VENTANA, ALTO_VENTANA, "3. Dibujo de Circulo");
    SetTargetFPS(60);

    bool cancelado;
    Vector2 origen = PedirPunto("Circulo: clic para el punto de origen (centro)",
                                 [](Vector2) { return true; }, "", {}, {}, cancelado);
    if (cancelado) { CloseWindow(); return; }

    int diametro = LeerEntero("Circulo: indica el diametro (1 a 30 unidades)", 1, 30, cancelado);
    if (cancelado) { CloseWindow(); return; }

    float radioPixeles = (diametro / 2.0f) * ESCALA;

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DibujarPlanoCartesiano();
            Vector2 centroPantalla = CartesianoAPantalla(origen);
            DrawCircleV(centroPantalla, radioPixeles, Fade(LIME, 0.5f));
            DrawCircleLines((int)centroPantalla.x, (int)centroPantalla.y, radioPixeles, DARKGREEN);
            DrawCircleV(centroPantalla, 3, DARKGRAY);
            DrawText("Ventana 3: Circulo (cierra la ventana para continuar)", 20, 20, 18, DARKGRAY);
            TextoAutores();
        EndDrawing();
    }
    CloseWindow();
}


int VentanaMenuPoligono(void)
{
    int seleccion = 0;
    while (!WindowShouldClose() && seleccion == 0) {
        if (IsKeyPressed(KEY_ONE))   seleccion = 1;
        if (IsKeyPressed(KEY_TWO))   seleccion = 2;
        if (IsKeyPressed(KEY_THREE)) seleccion = 3;
        if (IsKeyPressed(KEY_FOUR))  seleccion = 4;

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Elige el tipo de poligono:", 20, 20, 20, DARKGRAY);
            DrawText("1 - Cuadrado",           40,  80, 20, BLACK);
            DrawText("2 - Triangulo",          40, 110, 20, BLACK);
            DrawText("3 - Rectangulo",         40, 140, 20, BLACK);
            DrawText("4 - Poligono irregular", 40, 170, 20, BLACK);
            TextoAutores();
        EndDrawing();
    }
    return seleccion;
}

void DibujarCuadrado(void)
{
    bool cancelado;
    Vector2 p1 = PedirPunto("Cuadrado: clic para el primer punto del lado",
                             [](Vector2) { return true; }, "", {}, {}, cancelado);
    if (cancelado) return;

    Vector2 p2 = PedirPunto("Cuadrado: clic para el segundo punto del lado (distinto al primero)",
                             [p1](Vector2 c) { return !MismoPunto(c, p1); },
                             "El segundo punto debe ser distinto al primero",
                             { p1 }, {}, cancelado);
    if (cancelado) return;

    Vector2 d = { p2.x - p1.x, p2.y - p1.y };
    Vector2 perpIzq = { -d.y, d.x };
    Vector2 perpDer = { d.y, -d.x };

    Vector2 candIzq[4] = { p1, p2, { p2.x + perpIzq.x, p2.y + perpIzq.y }, { p1.x + perpIzq.x, p1.y + perpIzq.y } };
    Vector2 candDer[4] = { p1, p2, { p2.x + perpDer.x, p2.y + perpDer.y }, { p1.x + perpDer.x, p1.y + perpDer.y } };

    Vector2 eleccion = PedirPunto("Cuadrado: clic a un lado de la linea para elegir hacia donde se forma",
        [p1, d](Vector2 c) {
            float cruz = d.x * (c.y - p1.y) - d.y * (c.x - p1.x);
            return fabs(cruz) > 0.01f;
        },
        "Elige un punto que no este sobre la misma linea",
        { p1, p2 }, { { p1, p2 } }, cancelado);
    if (cancelado) return;

    float cruz = d.x * (eleccion.y - p1.y) - d.y * (eleccion.x - p1.x);
    Vector2* elegido = (cruz > 0) ? candIzq : candDer;

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DibujarPlanoCartesiano();
            for (int i = 0; i < 4; i++)
                DrawLineV(CartesianoAPantalla(elegido[i]), CartesianoAPantalla(elegido[(i + 1) % 4]), ORANGE);
            for (int i = 0; i < 4; i++)
                DrawCircleV(CartesianoAPantalla(elegido[i]), 4, DARKGRAY);
            DrawText("Cuadrado (cierra la ventana para continuar)", 20, 20, 18, DARKGRAY);
            TextoAutores();
        EndDrawing();
    }
}

void DibujarRectangulo(void)
{
    bool cancelado;
    Vector2 p1 = PedirPunto("Rectangulo: clic para el primer punto del lado",
                             [](Vector2) { return true; }, "", {}, {}, cancelado);
    if (cancelado) return;

    Vector2 p2 = PedirPunto("Rectangulo: clic para el segundo punto del lado (distinto al primero)",
                             [p1](Vector2 c) { return !MismoPunto(c, p1); },
                             "El segundo punto debe ser distinto al primero",
                             { p1 }, {}, cancelado);
    if (cancelado) return;

    Vector2 d = { p2.x - p1.x, p2.y - p1.y };
    float largo = sqrtf(d.x * d.x + d.y * d.y);
    Vector2 perpUnit = { -d.y / largo, d.x / largo };

    Vector2 tercero = PedirPunto("Rectangulo: clic para definir hacia donde y que tan alto",
        [p1, d](Vector2 c) {
            float cruz = d.x * (c.y - p1.y) - d.y * (c.x - p1.x);
            return fabs(cruz) > 0.01f;
        },
        "Elige un punto fuera de la linea del primer lado",
        { p1, p2 }, { { p1, p2 } }, cancelado);
    if (cancelado) return;

    float cruz = d.x * (tercero.y - p1.y) - d.y * (tercero.x - p1.x);
    float signo = (cruz > 0) ? 1.0f : -1.0f;
    float distancia = fabs(cruz) / largo;
    float altura = roundf(distancia);
    if (altura < 1) altura = 1;

    Vector2 offset = { perpUnit.x * signo * altura, perpUnit.y * signo * altura };
    Vector2 p3 = { p2.x + offset.x, p2.y + offset.y };
    Vector2 p4 = { p1.x + offset.x, p1.y + offset.y };
    Vector2 rect[4] = { p1, p2, p3, p4 };

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DibujarPlanoCartesiano();
            for (int i = 0; i < 4; i++)
                DrawLineV(CartesianoAPantalla(rect[i]), CartesianoAPantalla(rect[(i + 1) % 4]), SKYBLUE);
            for (int i = 0; i < 4; i++)
                DrawCircleV(CartesianoAPantalla(rect[i]), 4, DARKGRAY);
            DrawText("Rectangulo (cierra la ventana para continuar)", 20, 20, 18, DARKGRAY);
            TextoAutores();
        EndDrawing();
    }
}

void DibujarTriangulo(void)
{
    bool cancelado;
    Vector2 p1 = PedirPunto("Triangulo: clic para el primer punto",
                             [](Vector2) { return true; }, "", {}, {}, cancelado);
    if (cancelado) return;

    Vector2 p2 = PedirPunto("Triangulo: clic para el segundo punto (distinto al primero)",
                             [p1](Vector2 c) { return !MismoPunto(c, p1); },
                             "Debe ser distinto al primer punto", { p1 }, {}, cancelado);
    if (cancelado) return;

    Vector2 p3 = PedirPunto("Triangulo: clic para el tercer punto (que no este alineado con los otros dos)",
        [p1, p2](Vector2 c) {
            if (MismoPunto(c, p1) || MismoPunto(c, p2)) return false;
            return !SonColineales(p1, p2, c);
        },
        "Debe ser distinto y no estar alineado con los otros dos puntos",
        { p1, p2 }, { { p1, p2 } }, cancelado);
    if (cancelado) return;

    Vector2 tri[3] = { p1, p2, p3 };

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DibujarPlanoCartesiano();
            for (int i = 0; i < 3; i++)
                DrawLineV(CartesianoAPantalla(tri[i]), CartesianoAPantalla(tri[(i + 1) % 3]), GOLD);
            for (int i = 0; i < 3; i++)
                DrawCircleV(CartesianoAPantalla(tri[i]), 4, DARKGRAY);
            DrawText("Triangulo (cierra la ventana para continuar)", 20, 20, 18, DARKGRAY);
            TextoAutores();
        EndDrawing();
    }
}

void DibujarPoligonoIrregular(void)
{
    vector<Vector2> vertices;
    const int MAX_PUNTOS = 10;
    const float RADIO_CIERRE = 14.0f;

    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();
        bool dentro = DentroDeAreaDibujo(mouse);
        Vector2 cartActual = SnapACartesiano(mouse);
        Vector2 pantallaSnap = CartesianoAPantalla(cartActual);

        bool cercaDelInicio = false;
        if (vertices.size() >= 3) {
            Vector2 inicioPantalla = CartesianoAPantalla(vertices.front());
            float dx = mouse.x - inicioPantalla.x;
            float dy = mouse.y - inicioPantalla.y;
            cercaDelInicio = sqrtf(dx * dx + dy * dy) <= RADIO_CIERRE;
        }

        bool esRepetido = false;
        for (size_t k = 0; k < vertices.size(); k++)
            if (MismoPunto(vertices[k], cartActual)) esRepetido = true;

        bool segmentoValido = NuevoSegmentoValido(vertices, cartActual);
        bool cierrePosibleAhora = vertices.size() >= 3 && CierreValido(vertices);

        string errorMsg = "";
        if (dentro && !cercaDelInicio) {
            if (esRepetido) errorMsg = "Ese punto ya fue usado";
            else if (!segmentoValido) errorMsg = "Esa linea cruzaria otra existente";
        }
        if (cercaDelInicio && !cierrePosibleAhora) {
            errorMsg = "No se puede cerrar: alguna linea se cruzaria";
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (cercaDelInicio && cierrePosibleAhora) {
                break;
            } else if (dentro && !cercaDelInicio && !esRepetido && segmentoValido &&
                       (int)vertices.size() < MAX_PUNTOS) {
                vertices.push_back(cartActual);
            }
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DibujarPlanoCartesiano();

            for (size_t i = 0; i + 1 < vertices.size(); i++)
                DrawLineV(CartesianoAPantalla(vertices[i]), CartesianoAPantalla(vertices[i + 1]), PURPLE);
            for (size_t i = 0; i < vertices.size(); i++) {
                bool esInicioCerrable = (i == 0 && vertices.size() >= 3);
                Color c = esInicioCerrable ? ORANGE : RED;
                DrawCircleV(CartesianoAPantalla(vertices[i]), esInicioCerrable ? 7.0f : 4.0f, c);
            }
            if (!vertices.empty() && dentro && !cercaDelInicio)
                DrawLineV(CartesianoAPantalla(vertices.back()), pantallaSnap, Fade(PURPLE, 0.4f));
            if (!vertices.empty() && cercaDelInicio)
                DrawLineV(CartesianoAPantalla(vertices.back()), CartesianoAPantalla(vertices.front()), Fade(GREEN, 0.6f));

            DrawText("Poligono irregular: clic para cada vertice (min. 3, max. 10)", 20, 20, 18, DARKGRAY);
            DrawText("Con 3+ puntos, haz clic cerca del primer punto (naranja) para cerrar", 20, 45, 16, GRAY);
            if (!errorMsg.empty()) DrawText(errorMsg.c_str(), 20, 70, 16, RED);

            TextoAutores();
        EndDrawing();
    }

    if (WindowShouldClose()) return;

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DibujarPlanoCartesiano();
            for (size_t i = 0; i < vertices.size(); i++) {
                Vector2 a = vertices[i];
                Vector2 b = vertices[(i + 1) % vertices.size()];
                DrawLineV(CartesianoAPantalla(a), CartesianoAPantalla(b), PURPLE);
            }
            for (size_t i = 0; i < vertices.size(); i++)
                DrawCircleV(CartesianoAPantalla(vertices[i]), 4, RED);
            DrawText("Poligono irregular (cierra la ventana para continuar)", 20, 20, 18, DARKGRAY);
            TextoAutores();
        EndDrawing();
    }
}

void VentanaPoligono(void)
{
    InitWindow(ANCHO_VENTANA, ALTO_VENTANA, "4. Dibujo de Poligono");
    SetTargetFPS(60);

    int tipo = VentanaMenuPoligono();
    if (WindowShouldClose()) { CloseWindow(); return; }

    switch (tipo) {
        case 1: DibujarCuadrado(); break;
        case 2: DibujarTriangulo(); break;
        case 3: DibujarRectangulo(); break;
        case 4: DibujarPoligonoIrregular(); break;
    }

    CloseWindow();
}
