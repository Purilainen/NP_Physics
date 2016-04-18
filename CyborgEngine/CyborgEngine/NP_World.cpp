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
        for (size_t j = 0; j < m_objects.size(); j++)
        {

        }
    }
}

void NP_World::checkProximity(NP_Object* obj)
{
    glm::vec2 pos1;
    glm::vec2 pos2 = obj->getBody()->m_collider.position;

    //Do with 2 loops just to iterate through everything instead and check all bodies
    //Move this to body
    for (size_t i = 0; i < m_objects.size(); ++i)
    {
        //Calculate distance between two objects
        // d^2 = (x2-x1)^2 + (y2 -y1)^2;

        //Create circle from position with size
        pos1 = m_objects[i]->getBody()->m_collider.position;
        
        //Check if the two calculated circles overlap
        //if overlapping return isInProximity = true!
        obj->getBody()->inProximity(m_objects[i]->getBody());
        //else do nothing
    }
    

}
