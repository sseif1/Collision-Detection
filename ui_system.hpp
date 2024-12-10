#pragma once
#include "raylib.h"
#include "physics_system.hpp" // Add this include
#include <string>
#include <vector>
#include <deque>

class UISystem
{
public:
    struct Message
    {
        std::string text;
        float duration;
        float timer;

        Message(const std::string &t, float d)
            : text(t), duration(d), timer(d) {}
    };

private:
    std::deque<Message> messages;
    bool showHelp;
    Camera3D camera;

public:
    UISystem();

    void update();
    void render(const PhysicsSystem &physics);
    void addMessage(const std::string &text, float duration);
    void toggleHelp() { showHelp = !showHelp; }
    Camera3D &getCamera() { return camera; }

private:
    void drawMessages();
    void drawHelp();
    void drawStats(const PhysicsSystem &physics);
    void updateCamera();
};