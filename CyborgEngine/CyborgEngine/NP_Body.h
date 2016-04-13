#ifndef NP_BODY_H
#define NP_BODY_H

#include "glm/glm.hpp"

class NP_Body
{
public:

    void update(float deltaTime);

    //glm::vec2 getPos()
    //float getRotation()
    //set...

    //addCollider(Shape &shape)
    //bool detectCollisions


	NP_Body();
	~NP_Body();

protected:
    bool isStatic, isKinematic, isDynamic;
private:
    glm::vec2 m_velocity;
    glm::vec2 m_position;
    float g = 9.81;
};
#endif