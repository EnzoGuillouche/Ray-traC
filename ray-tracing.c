#include "raylib.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define WIDTH 1200
#define HEIGHT 800

#define NUM_ENTITIES 2
#define RAY_COUNT 500
#define MAX_RAY_LENGTH 3000
#define MIN_RADIUS 25
#define MAX_RADIUS 75

typedef struct Entity
{
    int x, y;
    int radius;
    Color color;
    bool isEmmiter; // is a light source
} Entity;

Entity entities[NUM_ENTITIES];

Vector2 tangents[2]; // used to store tangent points for each obstacle

void InitEntities()
{
    for (int i = 0; i < NUM_ENTITIES; i++)
    {
        entities[i].radius = rand() % (MAX_RADIUS - MIN_RADIUS + 1) + MIN_RADIUS;

        entities[i].x = rand() % (WIDTH - entities[i].radius - entities[i].radius + 1) + entities[i].radius;
        entities[i].y = rand() % (HEIGHT - entities[i].radius - entities[i].radius + 1) + entities[i].radius;
        entities[i].color = (Color){rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1, 255};
        entities[i].isEmmiter = i == 0 ? 1 : (rand() % 2 + 1) - 1;
    }
}

void DrawEntities()
{
    for (int i = 0; i < NUM_ENTITIES; i++)
    {
        DrawCircle(entities[i].x, entities[i].y, entities[i].radius, entities[i].color);
    }
}

double A, B, C, discriminant, sqrtD, m1, m2, lambda1, lambda2;

void ComputeTangents(double px, double py, double cx, double cy, double r)
{
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

        x = -(-2 * entities[j].x - 2 * m^2 * emmisionPoint.x + 2 * m * (emmisionPoint.y - entities[j].y)) / (2 * (1 + m^2))

        once we have x we can get y by replacing it in the line equation:
        y = m(x - emmisionPoint.x) + emmisionPoint.y
    */

    // Quadratic coefficients for slope m
    A = (cx - px) * (cx - px) - r * r;
    B = 2 * (cx - px) * (py - cy);
    C = (py - cy) * (py - cy) - r * r;

    discriminant = B * B - 4 * A * C;

    if (discriminant < 0)
    {
        printf("No tangents (point inside circle)\n");
    }
    else
    {
        sqrtD = sqrt(discriminant);

        // Two slopes
        m1 = (-B + sqrtD) / (2 * A);
        m2 = (-B - sqrtD) / (2 * A);

        // Tangent 1
        lambda1 = (m1 * (px - cx) + (cy - py)) / (m1 * m1 + 1);

        tangents[0].x = cx + m1 * lambda1;
        tangents[0].y = cy - lambda1;

        // Tangent 2
        lambda2 = (m2 * (px - cx) + (cy - py)) / (m2 * m2 + 1);

        tangents[1].x = cx + m2 * lambda2;
        tangents[1].y = cy - lambda2;
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
    bool shouldDrawRay = true;
    float angle = 0.0f;
    double angleToTangent1, angleToTangent2, angleRay;
    for (int i = 0; i < RAY_COUNT; i++)
    {
        angle = (2.0f * PI / RAY_COUNT) * i;
        shouldDrawRay = true;

        Vector2 end = {
            lightEntity->x + cosf(angle) * MAX_RAY_LENGTH,
            lightEntity->y + sinf(angle) * MAX_RAY_LENGTH};

        // calculate tangent lines of possibles obstacles
        for (int j = 0; j < NUM_ENTITIES; j++)
        {
            if (&entities[j] == lightEntity)
                continue;

            ComputeTangents(lightEntity->x, lightEntity->y, entities[j].x, entities[j].y, entities[j].radius);

            angleToTangent1 = atan2(tangents[0].y - lightEntity->y, tangents[0].x - lightEntity->x);
            angleToTangent2 = atan2(tangents[1].y - lightEntity->y, tangents[1].x - lightEntity->x);
            angleRay = atan2(end.y - lightEntity->y, end.x - lightEntity->x);

            // Ensure tangent angles are sorted
            if (angleToTangent1 > angleToTangent2)
            {
                double temp = angleToTangent1;
                angleToTangent1 = angleToTangent2;
                angleToTangent2 = temp;
            }

            // If ray angle is between tangent angles, skip it
            if (angleRay > angleToTangent1 && angleRay < angleToTangent2)
            {
                // can display a Line, but the end would be the edge of the circle
                Vector2 newEnd = {
                    lightEntity->x + cosf(angle) * sqrt(pow(lightEntity->x - entities[j].x, 2) + pow(lightEntity->y - entities[j].y, 2)),
                    lightEntity->y + sinf(angle) * sqrt(pow(lightEntity->x - entities[j].x, 2) + pow(lightEntity->y - entities[j].y, 2))};

                DrawLineV((Vector2){lightEntity->x, lightEntity->y}, newEnd, Fade(lightEntity->color, 0.5f));
                shouldDrawRay = false;
            }
        }

        if (shouldDrawRay)
        {
            DrawLineV((Vector2){lightEntity->x, lightEntity->y}, end, Fade(lightEntity->color, 0.5f));
        }
    }
}

int main(void)
{
    srand(time(NULL));
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