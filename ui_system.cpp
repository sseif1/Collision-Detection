#include "ui_system.hpp"
#include "raymath.h"

UISystem::UISystem()
    : showHelp(false)
{
    camera.position = (Vector3){10.0f, 10.0f, 10.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
}

void UISystem::update()
{
    updateCamera();

    // Update message timers
    for (auto it = messages.begin(); it != messages.end();)
    {
        it->timer -= GetFrameTime();
        if (it->timer <= 0)
            it = messages.erase(it);
        else
            ++it;
    }
}

void UISystem::render(const PhysicsSystem &physics)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode3D(camera);

    // Draw grid
    DrawGrid(20, 1.0f);

    // Draw spheres
    for (const auto &sphere : physics.getSpheres())
    {
        DrawSphere(sphere->getPosition(), sphere->getRadius(), sphere->getColor());

        // Draw velocity vector
        Vector3 endPoint = Vector3Add(
            sphere->getPosition(),
            Vector3Scale(sphere->getVelocity(), 0.2f));
        DrawLine3D(sphere->getPosition(), endPoint, BLACK);
    }

    EndMode3D();

    // Draw UI elements
    drawMessages();
    drawStats(physics);
    if (showHelp)
        drawHelp();

    EndDrawing();
}

void UISystem::addMessage(const std::string &text, float duration)
{
    messages.emplace_back(text, duration);
    if (messages.size() > 5) // Keep only last 5 messages
        messages.pop_front();
}

void UISystem::drawMessages()
{
    float y = GetScreenHeight() - 40.0f;
    for (const auto &msg : messages)
    {
        float alpha = msg.timer / msg.duration;
        DrawText(msg.text.c_str(), 10, y, 20, Fade(BLACK, alpha));
        y -= 25.0f;
    }
}

void UISystem::drawHelp()
{
    DrawRectangle(10, 50, 340, 220, Fade(RAYWHITE, 0.9f));
    DrawRectangleLines(10, 50, 340, 220, BLACK);
    DrawText("Controls:", 20, 60, 20, BLACK);
    DrawText("- Left Click: Place sphere", 20, 85, 15, DARKGRAY);
    DrawText("- Right Click + Drag: Rotate camera", 20, 105, 15, DARKGRAY);
    DrawText("- Mouse Wheel: Zoom camera", 20, 125, 15, DARKGRAY);
    DrawText("- [Space]: Pause/Resume", 20, 145, 15, DARKGRAY);
    DrawText("- [R]: Reset simulation", 20, 165, 15, DARKGRAY);
    DrawText("- [G]: Toggle gravity", 20, 185, 15, DARKGRAY);
    DrawText("- [H]: Toggle help", 20, 205, 15, DARKGRAY);
    DrawText("- [ESC]: Exit", 20, 225, 15, DARKGRAY);
}

void UISystem::drawStats(const PhysicsSystem &physics)
{
    DrawFPS(10, 10);
    DrawText(physics.isPaused() ? "PAUSED" : "RUNNING", 10, 30, 20,
             physics.isPaused() ? RED : GREEN);
}

void UISystem::updateCamera()
{
    UpdateCamera(&camera, CAMERA_FREE);
}