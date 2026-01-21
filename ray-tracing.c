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

        // calculate tangent lines of possibles obstacles
        for (int j = 0; j < NUM_ENTITIES; j++)
        {
            if (&entities[j] == lightEntity) continue;

            Vector2 emmisionPoint = { lightEntity->x, lightEntity->y };

            /*
                any line going through this the emmision point can be written :
                entities[j].y - emmisionPoint.y = m(entities[j].x - emmisionPoint.x)
                entities[j].y = m(entities[j].x - emmisionPoint.x) + emmisionPoint.y
                m(entities[j].x - emmisionPoint.x) + emmisionPoint.y - entities[j].y = 0
                m*entities[j].x - m*emmisionPoint.x + emmisionPoint.y - entities[j].y = 0

                General line equation: A*x + B*y + C = 0, so:
                A = m, B = -1, C = emmisionPoint.y - entities[j].y

                A line is tangent to a circle if the distance between the center of the circle and the line is equal to the radius.
                so distance({ entities[j].x, entities[j].y }, line) = entities[j].radius
                d = |A*x0 + B*y0 + C| / sqrt(A^2 + B^2)
                d = |m*entities[j].x - entities[j].y + emmisionPoint.y - entities[j].y| / sqrt(m^2 + 1)

                d = entities[j].radius, so:
                entities[j].radius = |m*entities[j].x - entities[j].y + emmisionPoint.y - entities[j].y| / sqrt(m^2 + 1)
                entities[j].radius^2 = (m*entities[j].x - entities[j].y + emmisionPoint.y - entities[j].y)^2 / (m^2 + 1)
                entities[j].radius^2 * (m^2 + 1) = (m*entities[j].x - entities[j].y + emmisionPoint.y - entities[j].y)^2
                entities[j].radius^2 * m^2 + entities[j].radius^2 = (m*entities[j].x - entities[j].y + emmisionPoint.y - entities[j].y)^2
                entities[j].radius^2 * m^2 + entities[j].radius^2 = entities[j].x^2 * m^2 - entities[j].y^2 + emmisionPoint.y^2 - entities[j].y^2
                entities[j].radius^2 * m^2 = entities[j].x^2 * m^2 - entities[j].y^2 + emmisionPoint.y^2 - entities[j].y^2 - entities[j].radius^2
                entities[j].radius^2 * m^2 - entities[j].x^2 * m^2 = - entities[j].y^2 + emmisionPoint.y^2 - entities[j].y^2 - entities[j].radius^2
                m^2(entities[j].radius^2 - entities[j].x^2) = - entities[j].y^2 + emmisionPoint.y^2 - entities[j].y^2 - entities[j].radius^2
                
                Finally we have:
                m = ± sqrt( (emmisionPoint.y^2 - 2*entities[j].y - entities[j].radius^2) / (entities[j].radius^2 - entities[j].x^2) )

                now that we have m we can calculate the intersection point of the ray on the circle.
                so we go back to

                Circle equation:
                (x - entities[j].x)^2 + (y - entities[j].y)^2 = entities[j].radius^2

                and we replace y by the Tangent line equation:
                y = m(x - emmisionPoint.x) + emmisionPoint.y

                so we got this:
                (x - entities[j].x)^2 + ( m(x - emmisionPoint.x) + emmisionPoint.y - entities[j].y)^2 = entities[j].radius^2
                that we can develop to get x


                once we have x we can get y by replacing it in the line equation:
                y = m(x - emmisionPoint.x) + emmisionPoint.y
            */

            DrawLineV((Vector2){ lightEntity->x, lightEntity->y }, end, Fade(lightEntity->color, 0.5f));
        }
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