#include "NP_CollisionInfo.h"

NP_CollisionInfo::NP_CollisionInfo(NP_Body *a, NP_Body *b)
    : m_aBody(a), m_bBody(b)
{}

NP_CollisionInfo::~NP_CollisionInfo()
{}

void NP_CollisionInfo::Solve()
{
    // 1st Get axes for both bodies
    //http://www.dyn4j.org/2010/01/sat/
    // 2nd loop over axes
    bool TEST = true;
    if (TEST)
    {
        contact_count = 0;
        float overl = FLT_MAX;
        glm::vec2 smallest(0, 0);
        NP_Body *A = m_aBody;
        NP_Body *B = m_bBody;

        A->computeAxes();
        B->computeAxes();

        if (A != B)
        {
            //Loop over  A axes
            for (size_t i = 0; i < 4; ++i)
            {                
                glm::vec2 axis = A->m_collider.axes[i];

                // Project both shapes onto the axis
                glm::vec2 p1 = A->m_collider.projectToAxis(axis); // x = min , y = max
                glm::vec2 p2 = B->m_collider.projectToAxis(axis);
                // Check if projections overlap
                if (!overlap(p1, p2))
                {
                    //Shapes do not overlap
                    return;
                }
                else
                {
                    // Get overlap
                    float o = getOverlap(p1, p2);
                    // Check for minimum
                    if (o < overl)
                    {
                        // Then set this to smallest
                        overl = o; // amount of overlapping
                        smallest = axis;
                        normal = smallest;
                    }
                }
            }
            //Loop over B axes
            for (size_t i = 0; i < 4; ++i)
            {
                glm::vec2 axis = B->m_collider.axes[i];

                // Project both shapes onto the axis
                glm::vec2 p1 = A->m_collider.projectToAxis(axis); // x = min , y = max
                glm::vec2 p2 = B->m_collider.projectToAxis(axis);
                // Check if projections overlap
                if (!overlap(p1, p2))
                {
                    //Shapes do not overlap
                    return;
                }
                else
                {
                    // Get overlap
                    float o = getOverlap(p1, p2);
                    // Check for minimum
                    if (o < overl)
                    {
                        // Then set this to smallest
                        overl = o; // amount of overlapping
                        smallest = axis;
                        normal = smallest;

                    }
                }
            }
            contact_count++;       
        }
        else
        {
            return;
        }
    }

    // AABB vs AABB // polyTopoly here
    // http://gamedevelopment.tutsplus.com/tutorials/create-custom-2d-physics-engine-aabb-circle-impulse-resolution--gamedev-6331
    bool AABB = false;
    if (AABB)
    {
        NP_Body *A = m_aBody;
        NP_Body *B = m_bBody;

        glm::vec2 n = B->m_position - A->m_position;

        glm::vec2 aboxMax = A->m_collider.max;
        glm::vec2 aboxMin = A->m_collider.min;
        glm::vec2 bboxMax = B->m_collider.max;
        glm::vec2 bboxMin = B->m_collider.min;

        // Calc half extent along x axis for each obj
        float a_extent = (aboxMax.x - aboxMin.x) / 2;
        float b_extent = (bboxMax.x - bboxMin.x) / 2;        

        // Calc overlap on x axis
        float x_overlap = a_extent + b_extent - glm::abs(n.x);

        // SAT on x axis
        if (x_overlap > 0)
        {
            contact_count = 1;
            // Calc half extents along y axis for each obj
            a_extent = (aboxMax.y - aboxMin.y) / 2;
            a_extent = (aboxMax.y - aboxMin.y) / 2;

            float y_overlap = a_extent + b_extent - abs(n.y);

            if (y_overlap > 0)
            {
                if (x_overlap < y_overlap)
                {

                    if (n.x < 0)
                        normal = glm::vec2(-1, 0);
                    else
                        normal = glm::vec2(1, 0);
                    penetration = x_overlap;
                }
                else
                {
                    if (n.y < 0)
                        normal = glm::vec2(0, -1);
                    else
                        normal = glm::vec2(0, 1);
                    penetration = y_overlap;
                }
            }
        }
    }
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
    //KARVALAKKIMALLI

    NP_Body* A = m_aBody;
    NP_Body* B = m_bBody;
    // Calculate relative velocity
    glm::vec2 rv = B->m_velocity - A->m_velocity;

    // Calculate relative velocity in terms of the normal direction
    float velAlongNormal = Dot(rv, normal);

    // Do not resolve if velocities are seperating
    if (velAlongNormal > 0)
      return;

     //Calculate restitution
    float e = glm::min<float>(A->m_restitution, B->m_restitution);

    // Calculate impulse scalar
    float j = -(1 + e) * velAlongNormal;
    j /= (1 / A->m_mass) + (1 / B->m_mass);

    // Apply impulse
    glm::vec2 impulse = j * normal;
    A->m_velocity -= (1 / A->m_mass) * impulse;
    B->m_velocity += (1 / B->m_mass) * impulse;

    // Check if both objects have infinite mass
    //if (/* A->inverseMass == B->inverseMass*/)
    //    {
    //    InfiniteMassCorrection();
    //    return;
    //    }

    //for (size_t i = 0; i < contact_count; ++i)
    //{
    //    // Calculate radii from COM to contact
    //    glm::vec2 ra = contactPoints[i] - m_aBody->m_position;
    //    glm::vec2 rb = contactPoints[i] - m_bBody->m_position;

    //    glm::vec2 rv = m_bBody->m_velocity + Cross(m_bBody->m_angularVelocity, rb) -
    //        m_aBody->m_velocity + Cross(m_aBody->m_angularVelocity, ra);

    //    //Relative velocity along the normal
    //    float contactVel = Dot(rv, normal);

    //    //Do not resolve if velocities are seperating
    //    if (contactVel > 0)
    //        return;

    //    float raCrossN = Cross(ra, normal);
    //    float rbCrossN = Cross(ra, normal);
    //    
        //real invMassSum = A->im + B->im + Sqr( raCrossN ) * A->iI + Sqr( rbCrossN ) * B->iI;
        //float invMassSum = m_aBody->inverseMass + m_bBody->inverseMass

 
    //}
}

bool NP_CollisionInfo::overlap(glm::vec2 projection1, glm::vec2 projection2)
{
    return (projection1.x < projection2.y || projection2.x > projection1.y);
}

float NP_CollisionInfo::getOverlap(glm::vec2 projection1, glm::vec2 projection2)
{
    // 
    if (projection1.y > projection2.x)
    {
        return projection1.y - projection2.x;
    }
    else if (projection1.x > projection2.y)
    {
        return projection1.y - projection2.x;
    }
    else
    {
        //PROBLEM WITH OVERLAP
    }
}

void NP_CollisionInfo::calcContactPoints()
{
    // http://www.dyn4j.org/2011/11/contact-points-using-clipping/
    // 1st Find features
    // Find the farthest vertex in the shape. Then, we look at the adjacent two vertices to determine which edge is the “closest.”
    // Closest is the edge which is most perpendicular (kohtisuora) to the separation normal.


    // 2nd Clip to get contact points
    // Identify the reference edge and incident edge. 
    // The reference edge is the edge most perpendicular to the separation normal. 
    // The reference edge will be used to clip the incident edge vertices to generate the contact manifold.


}