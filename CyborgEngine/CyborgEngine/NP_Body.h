#ifndef NP_BODY_H
#define NP_BODY_H

#include "glm/glm.hpp"

struct Collider 
{
    glm::vec2* points; //The points that determine the shape of the collider (4 points for a box)
    glm::vec2 position;
    float rotation;
    float size = 1.0f;
};

class NP_Body
{
public:

    void update(float deltaTime);

    //glm::vec2 getPosition() //Get collider pos
    //float getRotation() //Get collider rotation
    //set...

    //addCollider(Shape &shape)
    //bool detectCollisions
    //addCollisionBox(points)
    //addColliderFromPoly(polygon)

    bool isStatic(bool value) { Static = value; }
    bool isDynamic(bool value){ Dynamic = value; }
    bool isKinematic(bool value) { Kinematic = value; }
    bool inProximity(float radius); //Check in circle radius if there are other bodies near by, if true - do accurate collision check
    //checkCollision()

	NP_Body();
	~NP_Body();
    Collider m_collider;
protected:
    bool Static, Kinematic, Dynamic;
    
private:
    glm::vec2 m_velocity;
    glm::vec2 m_position;
    float g = 9.81;
    
    bool inProximityWithOtherBody;
};
#endif