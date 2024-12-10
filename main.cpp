#include "physics_system.hpp"
#include "ui_system.hpp"
#include "raymath.h" // Add this for Vector3 operations
#include <string>

int main()
{
    // Initialize window and set target FPS
    InitWindow(800, 600, "3D Physics Simulation");
    SetTargetFPS(60);

    // Initialize physics system with default settings
    PhysicsSystem physics({
        -9.81f,                // gravity
        0.01f,                 // air resistance
        {10.0f, 10.0f, 10.0f}, // bounds
        1.0f / 60.0f           // time step
    });

    // Initialize UI system
    UISystem ui;
    ui.addMessage("Welcome to Physics Simulation!", 5.0f);
    ui.addMessage("Left click to place spheres", 5.0f);

    // Sphere placement variables
    Vector3 placementStart = {0, 0, 0};
    bool isPlacing = false;
    int sphereCount = 0;
    const int MAX_SPHERES = 10;

    while (!WindowShouldClose())
    {
        // Handle sphere placement
        if (sphereCount < MAX_SPHERES && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Ray ray = GetMouseRay(GetMousePosition(), ui.getCamera());
            Vector3 groundPoint = {0.0f, 0.0f, 0.0f};

            // Calculate intersection with ground plane
            float t = -(ray.position.y - groundPoint.y) / ray.direction.y;
            if (t >= 0.0f)
            {
                Vector3 hitPoint = {
                    ray.position.x + ray.direction.x * t,
                    ray.position.y + ray.direction.y * t,
                    ray.position.z + ray.direction.z * t};

                if (!isPlacing)
                {
                    // First click - set start position
                    placementStart = hitPoint;
                    isPlacing = true;
                    ui.addMessage("Click again to set direction", 2.0f);
                }
                else
                {
                    // Second click - create sphere with direction
                    Vector3 direction = Vector3Normalize(Vector3Subtract(hitPoint, placementStart));
                    // Make sure direction is not zero
                    if (Vector3Length(direction) > 0.0f)
                    {
                        physics.addSphere(placementStart, direction);

                        // Reset placement state
                        isPlacing = false;
                        sphereCount++;

                        std::string msg = "Sphere added! ";
                        msg += std::to_string(MAX_SPHERES - sphereCount);
                        msg += " spheres remaining";
                        ui.addMessage(msg, 2.0f);
                    }
                }
            }
        }

        // Handle other input
        if (IsKeyPressed(KEY_SPACE))
        {
            physics.togglePause();
            ui.addMessage(physics.isPaused() ? "Simulation paused" : "Simulation resumed", 2.0f);
        }

        if (IsKeyPressed(KEY_R))
        {
            physics.reset();
            sphereCount = 0;
            ui.addMessage("Simulation reset", 2.0f);
        }

        if (IsKeyPressed(KEY_G))
        {
            static bool gravityEnabled = true;
            gravityEnabled = !gravityEnabled;
            physics.setGravity(gravityEnabled ? -9.81f : 0.0f);
            ui.addMessage(gravityEnabled ? "Gravity enabled" : "Gravity disabled", 2.0f);
        }

        if (IsKeyPressed(KEY_H))
        {
            ui.toggleHelp();
        }

        // Update simulation
        float deltaTime = GetFrameTime();
        physics.update(deltaTime);
        ui.update();

        // Render
        ui.render(physics);
    }

    CloseWindow();
    return 0;
}