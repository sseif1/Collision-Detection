#include "sphere.hpp"
#include "raymath.h" // Add this for Vector3 operations

Sphere::Sphere(Vector3 pos, Vector3 direction, const Properties &props)
    : position(pos), velocity{direction.x * 5.0f, direction.y * 5.0f, direction.z * 5.0f}, acceleration{0, 0, 0}, properties(props), state(State::ACTIVE), color(RED)
{
}

void Sphere::setState(State newState)
{
    state = newState;
    updateColor();
}

void Sphere::update(float deltaTime)
{
    if (state == State::INACTIVE)
        return;

    // Update velocity with acceleration
    velocity.x += acceleration.x * deltaTime;
    velocity.y += acceleration.y * deltaTime;
    velocity.z += acceleration.z * deltaTime;

    // Update position with velocity
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    position.z += velocity.z * deltaTime;

    // Reset acceleration
    acceleration = {0, 0, 0};
}

void Sphere::applyForce(Vector3 force)
{
    // F = ma, therefore a = F/m
    acceleration.x += force.x / properties.mass;
    acceleration.y += force.y / properties.mass;
    acceleration.z += force.z / properties.mass;
}

bool Sphere::checkCollision(const Sphere &other) const
{
    float dx = position.x - other.position.x;
    float dy = position.y - other.position.y;
    float dz = position.z - other.position.z;
    float distance = sqrt(dx * dx + dy * dy + dz * dz);
    return distance < (properties.radius + other.properties.radius);
}

void Sphere::handleCollision(Sphere &other)
{
    Vector3 response = calculateCollisionResponse(other);

    // Apply collision response with elasticity
    velocity = {
        response.x * properties.elasticity,
        response.y * properties.elasticity,
        response.z * properties.elasticity};

    // Move spheres apart to prevent sticking
    float distance = Vector3Length(Vector3Subtract(position, other.position));
    float overlap = properties.radius + other.properties.radius - distance;

    if (overlap > 0)
    {
        Vector3 direction = Vector3Normalize(Vector3Subtract(position, other.position));
        position = Vector3Add(position, Vector3Scale(direction, overlap * 0.5f));
        other.position = Vector3Add(other.position, Vector3Scale(Vector3Negate(direction), overlap * 0.5f));
    }
}

Vector3 Sphere::calculateCollisionResponse(const Sphere &other) const
{
    // Calculate relative velocity
    Vector3 relativeVel = Vector3Subtract(velocity, other.velocity);

    // Calculate collision normal
    Vector3 normal = Vector3Normalize(Vector3Subtract(position, other.position));

    // Calculate impulse scalar
    float velocityAlongNormal = Vector3DotProduct(relativeVel, normal);

    // Conservation of momentum and energy
    float j = -(1.0f + properties.elasticity) * velocityAlongNormal /
              (1.0f / properties.mass + 1.0f / other.properties.mass);

    return Vector3Add(velocity, Vector3Scale(normal, j / properties.mass));
}

bool Sphere::isOutOfBounds(const Vector3 &bounds) const
{
    return fabs(position.x) > bounds.x ||
           fabs(position.y) > bounds.y ||
           fabs(position.z) > bounds.z;
}

void Sphere::updateColor()
{
    switch (state)
    {
    case State::ACTIVE:
        color = GREEN;
        break;
    case State::COLLIDING:
        color = YELLOW;
        break;
    case State::INACTIVE:
        color = RED;
        break;
    }
}