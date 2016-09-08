#include "NP_Body.h"
#include <Windows.h>
#include <iostream>

void NP_Body::update(float deltaTime)
{
    m_collider.position = m_position;
    //std::cout << m_collider.position.x;
    if (Static)
    {
        //Don't update anything?
        //Don't check collisions
    }
    else if (Kinematic)
    {
        if (inProximityWithOtherBody)
        {
            //Check accurate collisions with other bodies
        }
        
        //Position is locked
        //Velocity = 0;
        //Rotation?
    }
    else if (Dynamic)
    {
        if (inProximityWithOtherBody)
        {
            //Check accurate collisions with other bodies
        }
        
        ////Update position
        //m_position.x = m_position.x + m_velocity.x * deltaTime; /*step*/
        //m_position.y = m_position.y + m_velocity.y * deltaTime; /*replace dTime with step*/

        ////Calculate accelerations (F = ma)
        //m_acceleration.x = m_force.x / m_mass;
        //m_acceleration.y = (m_force.y - m_gravity) / m_mass;

        //m_velocity.y += m_acceleration.y * deltaTime;
        //m_velocity.x += m_acceleration.x * deltaTime;

        ////Update velocity
        ////m_velocity.x += m_acceleration.x * deltaTime; //Increase / decrease speed with acceleration
        ////m_velocity.y += m_acceleration.y * deltaTime; //Increase / decrease speed with acceleration
        ////m_velocity.y = m_velocity.y - g * deltaTime; //Gravity
        //Sleep(5);
        ////Update rotation
        
    }
    
    // NOTE: Update positions first - then velocities

    //Acceleration? - Calculate acceleration for body and use it to determine velocity in each new step
    //Rotations? - Get rotations to use in collisions and such

    //More euler stuff

    //Calculate new positions
    //pos.x(i+1) = x(i) + Velocity.x(i) * step //x(0) = startPos.x
    //pos.y(i+1) = y(i) + Velocity.y(i) * step //y(0) = startPos.y

    //Calculate new velocity.x -> use acceleration
    //Velocity.x(i+1) = Velocity.x(i) = StartVelocity * cos(theta0) //Without friction/drag etc.
    //Velx += accel.x * step
    
    //Calculate new velocity.y -> use acceleration
    //Velocity.y(i+1) = Velocity.y(i) - g * step 
    //Velocity.y(0) = StartVelocity * sin(theta0) //Gravity affecting the velocity
    //vel.y += accel.y * step

    //MOVE()
    //velocity += acceleration
    //velocity *= friction/drag
    //position += velocity

}

void NP_Body::addBoxCollider(float size)
{
    //TODO : Support for rectangles and not only squares
    //NOTE: Use the same points that are used when making a rectangle poly
    m_collider.position = m_position;

    m_collider.corner[0] = { glm::vec2(-0.25, 0.25)*size };
    m_collider.corner[1] ={glm::vec2(0.25, 0.25)*size};
    m_collider.corner[2] = {glm::vec2(0.25, -0.25)*size};
    m_collider.corner[3] = {glm::vec2(-0.25, -0.25)*size};
    
    
    m_collider.max = glm::vec2(0.25, 0.25);
    m_collider.min = glm::vec2(-0.25, -0.25);

    for (size_t i = 0; i < m_collider.vertexCount; ++i)
    {
        int i2 = i + 1 < m_collider.vertexCount ? i + 1 : 0;
        glm::vec2 face = m_collider.corner[i2] - m_collider.corner[i];

        m_collider.normals[i] = glm::vec2(face.y, -face.x);
        glm::normalize(m_collider.normals[i]);
    }

    m_collider.rotation = m_orientation;
    m_collider.size = size;
    //computeAxes();
}

void NP_Body::addCircleCollider(float radius)
{
    
}


void NP_Body::computeAxes()
{
    // The axes we want to test are the normals of each shape's edges
    /*
    glm::vec2 zero(0, 0);
    glm::vec2 edge;
    // Axis 1
    glm::vec2 ax1p1 = m_collider.corner[0];
    glm::vec2 ax1p2 = m_collider.corner[1];

    edge = ax1p1 - ax1p2;

    if (edge == zero)
    {
        edge = ax1p2 - ax1p1;
    }
    glm::vec2 ax1leftHandNormal(edge.x, -edge.y);
    glm::vec2 ax1rightHandNormal(-edge.x, edge.y);

    m_collider.axes[0] = glm::normalize(edge);

    // Axis 2
    glm::vec2 ax2p1 = m_collider.corner[1];
    glm::vec2 ax2p2 = m_collider.corner[2];

    edge = ax2p1 - ax2p2;

    if (edge == zero)
    {
        edge = ax2p2 - ax2p1;
    }
    glm::vec2 ax2leftHandNormal(edge.x, -edge.y);
    glm::vec2 ax2rightHandNormal(-edge.x, edge.y);

    m_collider.axes[1] = glm::normalize(edge);

    // Axis 3
    glm::vec2 ax3p1 = m_collider.corner[2];
    glm::vec2 ax3p2 = m_collider.corner[3];

    edge = ax3p1 - ax3p2;

    if (edge == zero)
    {
        edge = ax3p2 - ax3p1;
    }
    glm::vec2 ax3leftHandNormal(edge.x, -edge.y);
    glm::vec2 ax3rightHandNormal(-edge.x, edge.y);

    m_collider.axes[2] = glm::normalize(edge);

    // Axis 4
    glm::vec2 ax4p1 = m_collider.corner[3];
    glm::vec2 ax4p2 = m_collider.corner[0];

    edge = ax4p1 - ax4p2;

    if (edge == zero)
    {
        edge = ax4p2 - ax4p1;
    }
    glm::vec2 ax4leftHandNormal(edge.x, -edge.y);
    glm::vec2 ax4rightHandNormal(-edge.x, edge.y);

    m_collider.axes[3] = glm::normalize(edge);

    */
    for (size_t i = 0; i < 3; ++i)
    {
        // Get current vertex
        glm::vec2 p1 = m_collider.corner[i];
        // Get next vertex
        glm::vec2 p2 = m_collider.corner[i + 1 == 4 ? 0 : i + 1];
        // Subtract the two to get the edge vector
        glm::vec2 edge = p1 - p2;
        glm::vec2 zero(0, 0);
        if (edge == zero)
        {
            edge = p2 - p1;
        }
        // Get either perpendicular vector
        glm::vec2 leftHandNormal(edge.x, -edge.y);
        glm::vec2 rightHandNormal(-edge.x, edge.y);

        m_collider.axes[i] = glm::normalize(leftHandNormal); // Normalize - Or right handed?

    }

    glm::vec2 testEdge = m_collider.corner[3] - m_collider.corner[0];
    if (testEdge == glm::vec2(0))
    {
        testEdge = m_collider.corner[0] - m_collider.corner[3];
    }

    glm::vec2 lefty(testEdge.x, -testEdge.y);
    m_collider.axes[3] = glm::normalize(lefty);

}


bool NP_Body::overLaps1Way(NP_Body* otherBody)
{
    for (size_t i = 0; i < 2; i++)
    {
        float t = glm::dot(m_collider.corner[0], m_collider.axes[i]);

        float tMin = t;
        float tMax = t;

        for (size_t j = 1; j < 4; ++j)
        {
            
            t = glm::dot(otherBody->m_collider.corner[j], m_collider.axes[i]);

            if (t < tMin)
            {
                tMin = t;
            }
            else if (t > tMax)
            {
                tMax = t;
            }
        }

        if ((tMin > 1 + m_collider.position[i]) || (tMax < m_collider.position[i]))
        {
            //No intersection in this dimension
            //Boxes cant overlap
            return false;
        }
    }
    //Boxes overlap
    return true;
}

void NP_Body::addColliderFromPoly(float size)
{
	//TODO: Get points from polygon and give them to the collider
}

bool NP_Body::inProximity(NP_Body* b)
{
    //Basic SAT?

    glm::vec2 pos1 = m_position;
    glm::vec2 pos2 = b->m_collider.position;
    float distance;
    
        //Calculate distance between two objects
        // d^2 = (x2-x1)^2 + (y2 -y1)^2;
    float x = pos2.x - pos1.x;
    float y = pos2.y - pos2.y;
    
    // calculate distance between the 2 objects 
    //distance = std::sqrtf((std::pow(x, 2)) + (std::pow(y, 2)));
        
    //if overlapping return isInProximity = true!
    //else return false
    
    return true;
}

void NP_Body::addForce(float forceX, float forceY)
{
    m_force.x = forceX;
    m_force.y = forceY;
}

NP_Body::NP_Body(NP_World world) : Static(false), Kinematic(false), Dynamic(true), m_world(&world), m_velocity(0), m_position(0), m_restitution(0.2f)
{
    //Bodies are dynamic by default
    inverseMass = 1 / m_mass;
}

NP_Body::~NP_Body()
{
}