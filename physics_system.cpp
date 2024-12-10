#include "physics_system.hpp"

PhysicsSystem::PhysicsSystem(const Settings &settings)
    : settings(settings), paused(false)
{
}

void PhysicsSystem::update(float deltaTime)
{
    if (paused)
        return;

    for (auto &sphere : spheres)
    {
        applyGravity(*sphere);
        applyAirResistance(*sphere);
        sphere->update(deltaTime);
    }

    checkCollisions();
    checkBounds();
}

void PhysicsSystem::addSphere(Vector3 position, Vector3 direction)
{
    spheres.push_back(std::make_unique<Sphere>(position, direction));
}

void PhysicsSystem::reset()
{
    spheres.clear();
    paused = false;
}

void PhysicsSystem::applyGravity(Sphere &sphere)
{
    sphere.applyForce({0, settings.gravity * sphere.getRadius(), 0});
}

void PhysicsSystem::applyAirResistance(Sphere &sphere)
{
    Vector3 velocity = sphere.getVelocity();
    Vector3 resistance = {
        -velocity.x * settings.airResistance,
        -velocity.y * settings.airResistance,
        -velocity.z * settings.airResistance};
    sphere.applyForce(resistance);
}

void PhysicsSystem::checkCollisions()
{
    for (size_t i = 0; i < spheres.size(); i++)
    {
        for (size_t j = i + 1; j < spheres.size(); j++)
        {
            if (spheres[i]->checkCollision(*spheres[j]))
            {
                spheres[i]->handleCollision(*spheres[j]);
                spheres[i]->setState(Sphere::State::COLLIDING);
                spheres[j]->setState(Sphere::State::COLLIDING);
            }
        }
    }
}

void PhysicsSystem::checkBounds()
{
    for (auto &sphere : spheres)
    {
        if (sphere->isOutOfBounds(settings.bounds))
        {
            sphere->setState(Sphere::State::INACTIVE);
        }
    }
}