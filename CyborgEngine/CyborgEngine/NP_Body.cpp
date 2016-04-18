#include "NP_Body.h"

void NP_Body::update(float deltaTime)
{

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
        m_position.x = m_position.x + m_velocity.x * deltaTime; /*step*/
        m_position.y = m_velocity.y + m_velocity.y * deltaTime; /*replace dTime with step*/

        //Calculate forces / acceleration (F = ma)

        //Update velocity
        m_velocity.x += m_acceleration.x * deltaTime; //Increase / decrease speed with acceleration
        m_velocity.y += m_acceleration.y * deltaTime; //Increase / decrease speed with acceleration
        m_velocity.y = m_velocity.y - g * deltaTime; //Gravity
        
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

void NP_Body::addColliderFromPoly(Polygon *pol)
{
	//pol->getPoints();
	m_collider.points = pol->getPoints();
}

bool NP_Body::inProximity(NP_Body* b)
{
    // Check surroundings in a circle area
    // return true if other bodies found
    //TODO: Move this to body class
    glm::vec2 pos1 = m_position;
    glm::vec2 pos2 = b->m_collider.position;
    float distance;
    
        //Calculate distance between two objects
        // d^2 = (x2-x1)^2 + (y2 -y1)^2;
    float x = pos2.x - pos1.x;
    float y = pos2.y - pos2.y;
    
    distance = std::sqrtf((std::pow(x, 2)) + (std::pow(y, 2)));
        

        //Check if the two calculated circles overlap
        //if overlapping return isInProximity = true!
        
        //else do nothing
    
    //radius = size + offset
    return true;
}

NP_Body::NP_Body(NP_World world) : Static(false), Kinematic(false), Dynamic(true), m_world(&world)
{
    //Bodies are dynamic by default
}

NP_Body::~NP_Body()
{
}