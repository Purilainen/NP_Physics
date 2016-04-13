#include "NP_Body.h"

void NP_Body::update(float step)
{

    //IF isStatic - dont update
    //IF isDynamic - update all (pos, velocity, collisions)
    //IF isKinematic - body is stationary but can affect other bodies through collisions for example (dont update pos/speed)
    
    // NOTE: Update positions first - then velocities

    //Update position/speed with euler here (This works only for vertical movement updating)
    //Pos: yn+1 = yn + vn * step
    //Velocity: vn+1 = vn - g * step

    //More euler stuff
    //Velocity.x(i+1) = Velocity.x(i) = StartVelocity * cos(theta0) //Without friction/drag etc.
    //Velocity.y(i+1) = Velocity.y(i) - g * step, Velocity.y(0) = StartVelocity * sin(theta0) //Gravity affecting the speed
    //pos.x(i+1) = x(i) + Velocity.x(i) * step //x(0) = startPos.x
    //pos.y(i+1) = y(i) + Velocity.y(i) * step //y(0) = startPos.y

}

NP_Body::NP_Body()
{
}

NP_Body::~NP_Body()
{
}