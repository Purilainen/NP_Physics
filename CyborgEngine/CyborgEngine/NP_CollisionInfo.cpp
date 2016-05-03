#include "NP_CollisionInfo.h"




NP_CollisionInfo::NP_CollisionInfo(NP_Body *a, NP_Body *b)
    : m_aBody(a), m_bBody(b)
{

}

NP_CollisionInfo::~NP_CollisionInfo()
{
}

void NP_CollisionInfo::Solve()
{
    // AABB vs AABB // polyTopoly here
    // http://gamedevelopment.tutsplus.com/tutorials/create-custom-2d-physics-engine-aabb-circle-impulse-resolution--gamedev-6331
}

void NP_CollisionInfo::Initialize()
{
    // Calculate average restitution - (ratio of relative speeds after and before an impact)
    e = glm::min(m_aBody->m_restitution, m_bBody->m_restitution);

    for (size_t i = 0; i < contact_count; ++i)
    {
        //Calculate radii from COM (center of mass) to contact
        glm::vec2 ra = contactPoints[i] - m_aBody->m_position;
        glm::vec2 rb = contactPoints[i] - m_bBody->m_position;

        glm::vec2 rv = m_bBody->m_velocity + Cross(m_bBody->m_angularVelocity, rb) -
                       m_aBody->m_velocity + Cross(m_aBody->m_angularVelocity, ra);

        
        // Determine if we should perform a resting collision or not
        // The idea is if the only thing moving this object is gravity,
        // then the collision should be performed without any restitution
        float lenSquared = rv.length() * rv.length();
        float gLenSqr = gravity.length() * gravity.length();

        if (lenSquared < (dt * gLenSqr + EPSILON))
        {
            e = 0.0f;
        }
    }

}

void NP_CollisionInfo::ApplyImpulse()
{
    /*
    //KARVALAKKIMALLI

    NP_Body* A = objA->getBody();
    NP_Body* B = objB->getBody();
    // Calculate relative velocity
    glm::vec2 rv = B->m_velocity - A->m_velocity;

    // Calculate relative velocity in terms of the normal direction
    //float velAlongNormal = Dot(rv, normal);

    // Do not resolve if velocities are seperating
    //if (velAlongNormal > 0)
    //  return;

    // Calculate restitution
    float e = glm::min<float>(A->m_restitution, B->m_restitution);

    // Calculate impulse scalar
    //float j = -(1 + e) * velAlongNormal;
    //j /= (1 / A->m_mass) + (1 / B->m_mass);

    // Apply impulse
    //glm::vec2 impulse = j * normal;
    //A->m_velocity -= (1 / A->m_mass) * impulse;
    //B->m_velocity += (1 / B->m_mass) * impulse;

    */



    // Check if both objects have infinite mass
    //if (/* A->inverseMass == B->inverseMass*/)
    //    {
    //    InfiniteMassCorrection();
    //    return;
    //    }

    for (size_t i = 0; i < contact_count; ++i)
    {
        // Calculate radii from COM to contact
        glm::vec2 ra = contactPoints[i] - m_aBody->m_position;
        glm::vec2 rb = contactPoints[i] - m_bBody->m_position;

        glm::vec2 rv = m_bBody->m_velocity + Cross(m_bBody->m_angularVelocity, rb) -
            m_aBody->m_velocity + Cross(m_aBody->m_angularVelocity, ra);

        //Relative velocity along the normal
        float contactVel = Dot(rv, normal);

        //Do not resolve if velocities are seperating
        if (contactVel > 0)
            return;

        float raCrossN = Cross(ra, normal);
        float rbCrossN = Cross(ra, normal);
        
        //real invMassSum = A->im + B->im + Sqr( raCrossN ) * A->iI + Sqr( rbCrossN ) * B->iI;
        //float invMassSum = m_aBody->inverseMass + m_bBody->inverseMass

 
    }
}
