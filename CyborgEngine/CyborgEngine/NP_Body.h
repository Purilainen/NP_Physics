#ifndef NP_BODY_H
#define NP_BODY_H

#include "glm/glm.hpp"

class NP_Body
{
public:

    void update(float step);

    //glm::vec2 getPos()
    //float getRotation()
    //set...
    //bool isStatic
    //bool isKinematic
    //addCollider(Shape &shape)

	NP_Body();
	~NP_Body();

private:
    float m_velocity;
    glm::vec2 m_position;
    float g = 9.81;

};
#endif