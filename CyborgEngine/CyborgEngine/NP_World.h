#ifndef NP_WORLD_H
#define NP_WORLD_H

#include "NP_Object.h"
#include <vector>
class NP_Object;
class NP_World
{
public:
	NP_World();
	~NP_World();

    void update(float deltaTime);
    void addObject(NP_Object* obj) { m_objects.emplace_back(obj); }

    void integrateVelocity(NP_Object* obj, float deltaTime);
    void integrateForces(NP_Object* obj, float deltaTime);
    void integratePosition(NP_Object* obj, float deltaTime);

    
private:
    std::vector<NP_Object*> m_objects;
    const glm::vec2 m_gravity = { 0, 9.81 };
};
#endif;
