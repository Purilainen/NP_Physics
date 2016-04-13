#include "NP_Body.h"

void NP_Body::update(float deltaTime)
{

    if (isStatic)
    {
        //Don't update anything?
    }
    else if (isKinematic)
    {
        //Check collisions with other bodies
        //Position is locked
        //Velocity = 0;
        //Rotation?
    }
    else if (isDynamic)
    {
        //Check collisions
        //Update position
        //Update velocity
        //Update rotation
    }
    
    // NOTE: Update positions first - then velocities?

    //Update position/speed with euler here (This works only for vertical movement updating)
    //Pos: yn+1 = yn + vn * step
    //Velocity: vn+1 = vn - g * step

    //Acceleration?
    //Rotations?

    //More euler stuff
    //Velocity.x(i+1) = Velocity.x(i) = StartVelocity * cos(theta0) //Without friction/drag etc.
    //Velocity.y(i+1) = Velocity.y(i) - g * step, Velocity.y(0) = StartVelocity * sin(theta0) //Gravity affecting the velocity
    //pos.x(i+1) = x(i) + Velocity.x(i) * step //x(0) = startPos.x
    //pos.y(i+1) = y(i) + Velocity.y(i) * step //y(0) = startPos.y

    //MOVE()
    //velocity += acceleration
    //velocity *= friction/drag
    //position += velocity

}

NP_Body::NP_Body() : isStatic(false), isKinematic(false), isDynamic(true)
{
    //Bodies are dynamic by default
}

NP_Body::~NP_Body()
{
}