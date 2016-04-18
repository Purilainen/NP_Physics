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

            m_objects[i]->getBody()->inProximity(m_objects[j]->getBody()); // Check distance between bodies - "proximity"
        }
    }
}

