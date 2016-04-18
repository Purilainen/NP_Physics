#include "NP_World.h"


NP_World::NP_World()
{
}


NP_World::~NP_World()
{
}

void NP_World::update(float deltaTime)
{
    //Iterate through objects in the world
    for (size_t i = 0; i < m_objects.size(); ++i)
    {
        for (size_t j = 0; j < m_objects.size(); j++)
        {

            if (m_objects[i]->getBody()->inProximity(m_objects[j]->getBody()))
            {
                // Check distance between bodies - "proximity"
                // Do accurate collision checks and thing things
                m_objects[i]->update(deltaTime);
                m_objects[j]->update(deltaTime);
            }
            else
            {
                m_objects[i]->update(deltaTime);
                m_objects[j]->update(deltaTime); //Update objects
            }
        }
    } 
}

