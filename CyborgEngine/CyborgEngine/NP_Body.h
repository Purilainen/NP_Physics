#ifndef NP_BODY_H
#define NP_BODY_H

#include "glm/glm.hpp"
#include "NP_World.h"
#include "Polygon.h"

struct Collider 
{
    glm::vec2* points; //The points that determine the shape of the collider (4 points for a box)
    glm::vec2 position;
    float rotation;
    float size = 1.0f;
};

class NP_World;
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
    void addBoxCollider(float size);
	//void addColliderFromPoly(Polygon *pol);

    glm::vec2 getPos() { return m_position; }
    bool isStatic(bool value) { Static = value; }
    bool isDynamic(bool value){ Dynamic = value; }
    bool isKinematic(bool value) { Kinematic = value; }
    bool inProximity(NP_Body* b); //Check in circle radius if there are other bodies near by, if true - do accurate collision check
    //checkCollision()

	NP_Body(NP_World world);
	~NP_Body();
    Collider m_collider;
protected:
    bool Static, Kinematic, Dynamic;
    
private:
    glm::vec2 m_velocity;
    glm::vec2 m_position;
    glm::vec2 m_acceleration;
    NP_World* m_world;
    float g = 9.81;
    
    bool inProximityWithOtherBody;
};
#endif