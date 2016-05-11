#ifndef NP_WORLD_H
#define NP_WORLD_H

#include "NP_Object.h"
#include "NP_CollisionInfo.h"
#include <vector>
class NP_Object;
class NP_CollisionInfo;
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
    void updateOrientation(NP_Object* obj);

private:
    std::vector<NP_Object*> m_objects;
    std::vector<NP_CollisionInfo> contacts;
    const glm::vec2 m_gravity = { 0, 9.81 };
    glm::mat4 T;
    glm::mat4 R1;
    glm::mat4 T_1;
    glm::mat4 muunnos;
    glm::vec4 temp;

};
#endif;
