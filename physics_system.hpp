#pragma once
#include "sphere.hpp"
#include "/System/Volumes/Data/Users/samaaalotaibi/Desktop/collision-system/raylib/src/raylib.h"
//#include "raymath.h"
#include <vector>
#include <memory>

class PhysicsSystem
{
public:
    struct Settings
    {
        float gravity;
        float airResistance;
        Vector3 bounds;
        float timeStep;
    };

private:
    std::vector<std::unique_ptr<Sphere>> spheres;
    Settings settings;
    bool paused;

public:
    PhysicsSystem(const Settings &settings = {-9.81f, 0.01f, {10.0f, 10.0f, 10.0f}, 1.0f / 60.0f});

    void update(float deltaTime);
    void addSphere(Vector3 position, Vector3 direction);
    void reset();
    void togglePause() { paused = !paused; }
    bool isPaused() const { return paused; }

    const std::vector<std::unique_ptr<Sphere>> &getSpheres() const { return spheres; }
    void setGravity(float g) { settings.gravity = g; }
    void setAirResistance(float r) { settings.airResistance = r; }

private:
    void applyGravity(Sphere &sphere);
    void applyAirResistance(Sphere &sphere);
    void checkCollisions();
    void checkBounds();
};