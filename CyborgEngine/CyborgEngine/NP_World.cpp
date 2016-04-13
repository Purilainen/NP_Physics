#include "NP_World.h"


NP_World::NP_World()
{
}


NP_World::~NP_World()
{
}

void NP_World::update(float deltaTime)
{
    //Iterate through objects in the world and call their update function
    for (size_t i = 0; i < m_objects.size(); ++i)
    {
        m_objects[i].update(deltaTime);
    }
}
