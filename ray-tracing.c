#include "raylib.h"
#include <math.h>

#define WIDTH 800
#define HEIGHT 450

#define NUM_ENTITIES 2
#define RAY_COUNT 500
#define MAX_RAY_LENGTH 2000

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

void DrawLight()
{
    Entity *lightEntity = 0;
    // identify light source
    for (int i = 0; i < NUM_ENTITIES; i++)
    {
        if (entities[i].isEmmiter)
        {
            lightEntity = &entities[i];
            break;
        }
    }

    // draw rays
    for (int i = 0; i < RAY_COUNT; i++)
    {
        float angle = (2.0f * PI / RAY_COUNT) * i;

        Vector2 end = {
            lightEntity->x + cosf(angle) * MAX_RAY_LENGTH,
            lightEntity->y + sinf(angle) * MAX_RAY_LENGTH
        };

        DrawLineV({ lightEntity->x, lightEntity->y }, end, Fade(lightEntity->color, 0.5f));
    }
}

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "Ray Tracing in C");
    InitEntities();

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(BLACK);
            DrawLight();
            DrawEntities();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}