#ifndef NP_BODY_H
#define NP_BODY_H

#include "NP_Math.h"
#include "NP_World.h"
#include "Polygon.h"

struct Collider 
{
    int vertexCount = 4;
    glm::vec2 corner[4]; //The points that determine the shape of the collider (4 points for a box)
    glm::vec2 position; //Origin
    glm::vec2 axis[2]; //Two edges of the collider
    glm::vec2 max;
    glm::vec2 min;
    
    float rotation;
    float size = 1.0f;
    glm::vec2 normals[4];
    glm::mat2 u;


    glm::vec2 getSupport(const glm::vec2& direction)
    {
        float bestProjection = -FLT_MAX;
        glm::vec2 bestVertex;

        for (size_t i = 0; i < corner->length(); ++i)
        {
            glm::vec2 v = corner[i];
            float projection = Dot(v, direction);

            if (projection > bestProjection)
            {
                bestVertex = v;
                bestProjection = projection;
            }
        }
        return bestVertex;
    }

    void setOrient(float radians)
    {
        float c = glm::cos(radians);
        float s = glm::sin(radians);

        u = glm::mat2(c, -s, s, c);
    }
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
    void addCircleCollider(float radius);
    void computeAxes(); //Compute axes for collider
    bool overLaps1Way(NP_Body* otherBody);
    bool overLaps(NP_Body* otherB) 
    {
        return overLaps1Way(otherB) && otherB->overLaps1Way(this);
    }
	void addColliderFromPoly(float size);

    glm::vec2 getPos() { return m_position; }
    void setPos(glm::vec2 pos){ m_position = pos; }

    bool isStatic(bool value) { Static = value; }
    bool isDynamic(bool value){ Dynamic = value; }
    bool isKinematic(bool value) { Kinematic = value; }
    bool inProximity(NP_Body* b); //Check in circle radius if there are other bodies near by, if true - do accurate collision check
    //checkCollision()

    
    void setOrient(float orient) { m_orientation = orient; m_collider.setOrient(m_orientation); }

	// Get and set different velocities to different bodies
	glm::vec2 getVelocity(){ return m_velocity; }
	void setVelocity(glm::vec2 velocity){ m_velocity = velocity; }

    void addForce(float forceX, float forceY);

	NP_Body(NP_World world);
	~NP_Body();
    Collider m_collider;
    bool Static, Kinematic, Dynamic;
    glm::vec2 m_velocity;
    glm::vec2 m_position;
    glm::vec2 m_acceleration;
    glm::vec2 m_force;
    NP_World* m_world;
    float m_mass = 1.0f;
    
    float inverseMass;
    float inverseInertia;

    float m_restitution;
    float m_angularVelocity;
    float m_torque;
    float m_orientation;

    bool inProximityWithOtherBody;
};
#endif