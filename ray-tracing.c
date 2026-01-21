#include "raylib.h"

#define WIDTH 800
#define HEIGHT 450

#define NUM_ENTITIES 2

typedef struct Entity {
    int x, y;
    int radius;
    Color color;
    bool isEmmiter; // is a light source
} Entity;

Entity entities[NUM_ENTITIES];

void InitEntities()
{
    entities[0] = (Entity) { 
        200, 200, 50, RED, true
    };

    entities[1] = (Entity) { 
        600, 200, 75, BLUE, false
    };
}

void DrawEntities()
{
    for (int i = 0; i < NUM_ENTITIES; i++)
    {
        DrawCircle(entities[i].x, entities[i].y, entities[i].radius, entities[i].color);
    }
}

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "Ray Tracing in C");
    InitEntities();

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(BLACK);
            DrawEntities();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}