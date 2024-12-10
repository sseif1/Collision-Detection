#pragma once
#include "/System/Volumes/Data/Users/samaaalotaibi/Desktop/collision-system/raylib/src/raylib.h"
//#include "raymath.h"
#include <cmath>

class Sphere
{
public:
    enum class State
    {
        ACTIVE,
        COLLIDING,
        INACTIVE
    };

    struct Properties
    {
        float mass;
        float elasticity;
        float radius;
    };

private:
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;
    Properties properties;
    State state;
    Color color;

public:
    Sphere(Vector3 pos, Vector3 direction, const Properties &props = {1.0f, 0.8f, 1.0f});

    // Getters
    Vector3 getPosition() const { return position; }
    Vector3 getVelocity() const { return velocity; }
    float getRadius() const { return properties.radius; }
    State getState() const { return state; }
    Color getColor() const { return color; }

    // Setters
    void setState(State newState);
    void setVelocity(Vector3 newVel) { velocity = newVel; }
    void setColor(Color newColor) { color = newColor; }

    // Physics methods
    void update(float deltaTime);
    void applyForce(Vector3 force);
    void handleCollision(Sphere &other);
    bool checkCollision(const Sphere &other) const;
    bool isOutOfBounds(const Vector3 &bounds) const;

private:
    Vector3 calculateCollisionResponse(const Sphere &other) const;
    void updateColor();
};