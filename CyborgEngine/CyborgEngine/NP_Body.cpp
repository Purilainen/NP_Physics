#include "NP_Body.h"

void NP_Body::update(float deltaTime)
{

    inProximity(m_collider.size * 1.5f); 

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
        //Update position
        //Update velocity
        //Update rotation
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
    glm::vec2 points[] =
    {
        glm::vec2(-0.25, 0.25)*size,
        glm::vec2(-0.25, -0.25)*size,
        glm::vec2(0.25, -0.25)*size,
        glm::vec2(0.25, 0.25)*size
    };

    m_collider.points = points;
    m_collider.size = size;
}

bool NP_Body::inProximity(NP_Body* b)
{
    // Check surroundings in a circle area
    // return true if other bodies found
    
    //radius = size + offset
    return true;
}

NP_Body::NP_Body(NP_World world) : Static(false), Kinematic(false), Dynamic(true), m_world(world)
{
    //Bodies are dynamic by default
}

NP_Body::~NP_Body()
{
}