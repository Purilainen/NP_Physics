#include "NP_World.h"
#include <Windows.h>
#include <iostream>
NP_World::NP_World()
{
}


NP_World::~NP_World()
{
}

void NP_World::update(float deltaTime)
{
    Sleep(50);
    for (size_t i = 0; i < m_objects.size(); ++i)
        m_objects[i]->update(deltaTime);

    //TODO:
    // FOR EACH STEP DO THESE THINGS: 
    //Generate collision info
    contacts.clear();
    for (size_t i = 0; i < m_objects.size(); ++i)
    {
        NP_Body* A = m_objects[i]->getBody();

        for (size_t j = 0; j < m_objects.size(); ++j)
        {
            NP_Body* B = m_objects[j]->getBody();

            if (A->inverseMass == 0 && B->inverseMass == 0)
                continue;
            NP_CollisionInfo cI(A, B);
            cI.Solve(); //Do collision thing here AABBvsAABB / PolyVsPoly
            if (cI.contact_count)
                contacts.emplace_back(cI);
        }
    }

    //Solve collisions - apply impulse
    for (size_t i = 0; i < contacts.size(); ++i)
    {
        contacts[i].ApplyImpulse();
    }


    // Integrate forces
    // - Go through objects - calc forces (calc acceleration)
    for (size_t i = 0; i < m_objects.size(); ++i)
        integrateForces(m_objects[i], deltaTime);


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


