#include "NP_World.h"
#include <Windows.h>
#include <iostream>
#include "glm/gtc/matrix_transform.hpp"


NP_World::NP_World()
{}

NP_World::~NP_World()
{}

void NP_World::update(float deltaTime)
{
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
            updateOrientation(m_objects[i]);
            updateOrientation(m_objects[j]);
            cI.Solve(); //Do collision check
            if (cI.contact_count)
                contacts.emplace_back(cI);            
        }
    }
   
    // Integrate forces
    // - Go through objects - calc forces (calc acceleration)
    for (size_t i = 0; i < m_objects.size(); ++i)
        integrateForces(m_objects[i], deltaTime);

    // Init collision
    //for (size_t i = 0; i < contacts.size(); ++i)
    //{
    //    contacts[i].Initialize();
    //}

    //Solve collisions - apply impulse
    for (size_t i = 0; i < contacts.size(); ++i)
    {
        contacts[i].ApplyImpulse();
    }

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
    obj->getBody()->m_velocity += (obj->getBody()->m_force * obj->getBody()->inverseMass - gravity) * deltaTime;

    // angularVelocity += torque * inverseInertia * dt
    obj->getBody()->m_angularVelocity += obj->getBody()->m_torque * obj->getBody()->inverseInertia * deltaTime;
}

void NP_World::updateOrientation(NP_Object* obj)
{
    NP_Body* B = obj->getBody();

    T = glm::translate(glm::mat4(), glm::vec3(B->getPos().x, B->getPos().y, 1.0f));
    R1 = glm::rotate(glm::mat4(), B->m_orientation, glm::vec3(0.0f, 0.0f, 1.0f));
    T_1 = glm::translate(glm::mat4(), glm::vec3(-B->getPos().x, -B->getPos().y, 1.0f));
    muunnos = T*R1*T_1;
    
     temp = (muunnos * glm::vec4(B->getPos().x - B->m_collider.size * 0.5f , B->getPos().y + B->m_collider.size * 0.5f, 0.0f, 1.0f));
     B->m_collider.corner[0] = glm::vec2(temp.x, temp.y);

     temp = (muunnos * glm::vec4(B->getPos().x + B->m_collider.size * 0.5f, B->getPos().y + B->m_collider.size * 0.5f, 0.0f, 1.0f));
     B->m_collider.corner[1] = glm::vec2(temp.x, temp.y);

     temp = (muunnos * glm::vec4(B->getPos().x + B->m_collider.size * 0.5f, B->getPos().y - B->m_collider.size * 0.5f, 0.0f, 1.0f));
     B->m_collider.corner[2] = glm::vec2(temp.x, temp.y);

     temp = (muunnos * glm::vec4(B->getPos().x - B->m_collider.size * 0.5f, B->getPos().y - B->m_collider.size * 0.5f, 0.0f, 1.0f));
     B->m_collider.corner[3] = glm::vec2(temp.x, temp.y);
}

