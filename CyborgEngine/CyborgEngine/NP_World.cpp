#include "NP_World.h"


NP_World::NP_World()
{
}


NP_World::~NP_World()
{
}

void NP_World::update(float deltaTime)
{
    
    for (size_t i = 0; i < m_objects.size(); ++i)
        m_objects[i]->update(deltaTime);

    //TODO:
    // FOR EACH STEP DO THESE THINGS: 

    // Integrate forces
    // - Go through objects - calc forces (calc acceleration)
    for (size_t i = 0; i < m_objects.size(); ++i)
        integrateForces(m_objects[i], deltaTime);

    // Solve collisions
    // Iterate through objects again

    // Integrate velocities
    for (size_t i = 0; i < m_objects.size(); ++i)
        integrateVelocity(m_objects[i], deltaTime);

    for (size_t i = 0; i < m_objects.size(); ++i)
    {
        m_objects[i]->getBody()->m_force = glm::vec2(0, 0);
        m_objects[i]->getBody()->m_torque = 0;

    }

    
}

void NP_World::integrateVelocity(NP_Object* obj, float deltaTime)
{
    // Position
    // pos += velocity * dt
    obj->getBody()->m_position += obj->getBody()->m_velocity * deltaTime;
     
    
    // Orientation
    // orient += angularVelocity * dt
    obj->getBody()->m_orientation += obj->getBody()->m_angularVelocity * deltaTime;

    obj->getBody()->setOrient(obj->getBody()->m_orientation);

    //IntegrateForces(obj, deltatime)
    integrateForces(obj, deltaTime);

}

void NP_World::integrateForces(NP_Object* obj, float deltaTime)
{
    // velocity += force * (inverseMass) + gravity * dt
    obj->getBody()->m_velocity += (obj->getBody()->m_force * obj->getBody()->m_mass - m_gravity) * deltaTime;

    // angularVelocity += torque * inverseInertia * dt
    obj->getBody()->m_angularVelocity += obj->getBody()->m_torque * deltaTime;
}

