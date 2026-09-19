#include "raylib.h"

int VentanaMenu(void);
void VentanaLinea(void);
void VentanaPolilinea(void);
void VentanaCirculo(void);
void VentanaPoligono(void);

int main(void)
{
    int opcion;
    do {
        opcion = VentanaMenu();
        switch (opcion) {
            case 1:
                VentanaLinea();
                break;

            case 2:
                VentanaPolilinea();
                break;

            case 3:
                VentanaCirculo();
                break;

            case 4:
                VentanaPoligono();
                break;

            case 5:
                break;
        }
    }while (opcion!=5 && opcion!=0);

    return 0;
}

int VentanaMenu(void)
{
    InitWindow(800, 450, "Menu");
    SetTargetFPS(60);

    int seleccion = 0;

    while (!WindowShouldClose() && seleccion == 0)
    {
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
            DrawText("5 - Salir", 40, 200, 20, MAROON);
        EndDrawing();
    }

    CloseWindow();
    return seleccion;
}

void VentanaLinea(void)
{
    InitWindow(800, 450, "1. Dibujo de Linea");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Ventana 1: Linea (Cierra para continuar)", 20, 20, 20, DARKGRAY);

            DrawLine(100, 200, 700, 200, RED);
        EndDrawing();

    }
    CloseWindow();
}
void VentanaPolilinea(void)
{
    InitWindow(800, 450, "2. Dibujo de Polilinea");
    SetTargetFPS(60);

    Vector2 puntos[] = {
        { 150, 300 },
        { 300, 150 },
        { 450, 300 },
        { 600, 150 }
    };
    int cantidadPuntos = 4;

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Ventana 2: Polilinea", 20, 20, 20, DARKGRAY);

            DrawLineStrip(puntos, cantidadPuntos, BLUE);
        EndDrawing();

    }
    CloseWindow();

}

void VentanaCirculo(void)
{
    InitWindow(800, 450, "3. Dibujo de Circulo");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Ventana 3: Circulo", 20, 20, 20, DARKGRAY);

            DrawCircle(400, 225, 80, LIME);

            DrawCircleLines(400, 225, 85, DARKGREEN);
        EndDrawing();
    }
    CloseWindow();
}


void VentanaPoligono(void)
{
    InitWindow(800, 450, "4. Dibujo de Poligono");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Ventana 4: Poligono Regular (Hexagono)", 20, 20, 20, DARKGRAY);

            Vector2 centro = { 400, 225 };
            int lados = 6;
            float radio = 100.0f;
            float rotacion = 0.0f;

            DrawPoly(centro, lados, radio, rotacion, ORANGE);
        EndDrawing();
    }
    CloseWindow();
}
