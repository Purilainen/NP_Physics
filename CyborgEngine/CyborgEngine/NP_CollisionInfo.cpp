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
            calcContactPoints();
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
    //e = glm::min(m_aBody->m_restitution, m_bBody->m_restitution);

    //for (size_t i = 0; i < contact_count; ++i)
    //{
    //    //Calculate radii from COM (center of mass) to contact
    //    glm::vec2 ra = contactPoints[i] - m_aBody->m_position;
    //    glm::vec2 rb = contactPoints[i] - m_bBody->m_position;

    //    glm::vec2 rv = m_bBody->m_velocity + Cross(m_bBody->m_angularVelocity, rb) -
    //                   m_aBody->m_velocity + Cross(m_aBody->m_angularVelocity, ra);
    //    
    //    // Determine if we should perform a resting collision or not
    //    // The idea is if the only thing moving this object is gravity,
    //    // then the collision should be performed without any restitution
    //    float lenSquared = rv.length() * rv.length();
    //    float gLenSqr = gravity.length() * gravity.length();

    //    if (lenSquared < (dt * gLenSqr + EPSILON))
    //    {
    //        e = 0.0f;
    //    }
    //}
}

void NP_CollisionInfo::ApplyImpulse()
{    
    
    
    NP_Body* A = m_aBody;
    NP_Body* B = m_bBody;

    /*
    //KARVALAKKIMALLI
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
    */

    
    
    // Work in progress

    for (size_t i = 0; i < contact_count; ++i)
    {
        e = 0.2f;
        // Calculate radii from center of mass to contact for both bodies
        glm::vec2 ra = contactPoints[i] - A->m_position;
        glm::vec2 rb = contactPoints[i] - B->m_position;

        // Calculate relative velocity
        glm::vec2 rv = B->m_velocity + Cross(B->m_angularVelocity, rb) -
            A->m_velocity + Cross(A->m_angularVelocity, ra);

        //Relative velocity along the normal
        float contactVel = Dot(rv, normal);

        //Do not resolve if velocities are separating
        if (contactVel > 0)
            return;

        // Cross products for relative velocities and the contact normal
        float raCrossN = Cross(ra, normal);
        float rbCrossN = Cross(ra, normal);
        
        if (raCrossN < 0)
        {
            raCrossN = raCrossN * -1.f;
        }
        if (rbCrossN < 0)
        {
            rbCrossN = rbCrossN * -1.f;
        }



        float halfSize = 0.5f;
        float aI = 1.0f /  (A->m_mass * (halfSize * halfSize + halfSize * halfSize));
        float bI = 1.0f /  (B->m_mass * (halfSize * halfSize + halfSize * halfSize));

        //real invMassSum = A->im + B->im + Sqr( raCrossN ) * A->iI + Sqr( rbCrossN ) * B->iI;
        //float invMassSum = A->inverseMass + B->inverseMass;
        float invMassSum = A->inverseMass + B->inverseMass + glm::sqrt(raCrossN) * aI + glm::sqrt(rbCrossN) * bI;
        //invMassSum = invMassSum * -1.f;

        // Calculate impulse scalar
        float iScalar = -(1.0f + e) * contactVel;
        iScalar /= invMassSum;
        iScalar /= (float)contact_count;

        // Apply impulse
        glm::vec2 impulse = normal * iScalar;
        //A->m_velocity -= (1 / A->m_mass) * impulse;
        //B->m_velocity += (1 / B->m_mass) * impulse;
        A->addImpulse(-impulse, ra);
        B->addImpulse(impulse, rb);


        // Friction impulse
        glm::vec2 tangent = rv - (normal * Dot(rv, normal));
        glm::normalize(tangent);

        float jt = -Dot(rv, tangent);
        jt /= invMassSum;
        jt /= (float)contact_count;

        // COULUMBIN LAKI?
        glm::vec2 tangentImpulse;

        if (std::abs(jt) < iScalar * 0.25f)
            tangentImpulse = tangent * jt;
        else
            tangentImpulse = tangent * -iScalar * 0.15f;

        //glm::vec2 tangentImpulse = tangent * -iScalar;

        //if (tangentImpulse.x < 0)
        //    tangentImpulse.x = tangentImpulse.x * -1.f;
        //if (tangentImpulse.y < 0)
        //    tangentImpulse.y = tangentImpulse.y * -1.f;

        A->addImpulse(-tangentImpulse, ra);
        B->addImpulse(tangentImpulse, rb);
     }
    
    
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
        //IF WE GET HERE THERE IS PROBLEM WITH OVERLAP
    }
}

void NP_CollisionInfo::calcContactPoints()
{
    // http://www.dyn4j.org/2011/11/contact-points-using-clipping/
    // 1st Find features
    // Find the farthest vertex in the shape. Then, we look at the adjacent two vertices to determine which edge is the “closest.”
    // Closest is the edge which is most perpendicular (kohtisuora) to the separation normal.
    index = 0;
    glm::vec2 edge;


    for (int i = 0; i < 4; ++i)
    {        
        float max = -FLT_MAX;
        float projection = Dot(normal, m_aBody->m_collider.corner[i]);
        if (projection > max)
        {
            max = projection;
            index = i;
        }
    }

    // Get closest corner and its neighboring corners
    glm::vec2 v = m_aBody->m_collider.corner[index];
    glm::vec2 v1 = m_aBody->m_collider.corner[index + 1];
    glm::vec2 v0 = m_aBody->m_collider.corner[index - 1];

    // v1 to v0
    glm::vec2 l = v - v1;
    // v0 to v
    glm::vec2 r = v - v0;

    // normalize left and right
    glm::normalize(l);
    glm::normalize(r);

    if (Dot(normal, r) <= Dot(normal, l))
    {
        edge = r;
    }
    else
    {
        edge = l;
    }

    index = 0;
    glm::vec2 edge2;

    for (int i = 0; i < 4; ++i)
    {
        float max = -FLT_MAX;
        float projection = Dot(normal, m_bBody->m_collider.corner[i]);
        if (projection > max)
        {
            max = projection;
            index = i;
        }
    }

    // Get closest corner and its neighboring corners for second edge
    v = m_bBody->m_collider.corner[index];
    v1 = m_bBody->m_collider.corner[index + 1];
    v0 = m_bBody->m_collider.corner[index - 1];

    // v1 to v0
    l = v - v1;
    // v0 to v
    r = v - v0;

    // normalize
    glm::normalize(l);
    glm::normalize(r);

    if (Dot(normal, r) <= Dot(normal, l))
    {
        edge2 = r;
    }
    else
    {
        edge2 = l;
    }



    // Reference and incident edges
    glm::vec2 ref, inc;

    // Should we flip?
    bool flip = false;

    if (glm::abs(Dot(normal, edge) <= glm::abs(Dot(normal, edge2))))
    {
        ref = edge;
        inc = edge2;
        v = m_aBody->m_collider.corner[index];
        v1 = m_aBody->m_collider.corner[index + 1];
        v0 = m_aBody->m_collider.corner[index - 1];
    }
    else
    {
        ref = edge2;
        inc = edge;
        v = m_bBody->m_collider.corner[index];
        v1 = m_bBody->m_collider.corner[index + 1];
        v0 = m_bBody->m_collider.corner[index - 1];
        flip = true;
    }

    // 2nd Clip to get contact points
    // Identify the reference edge and incident edge. 
    // The reference edge is the edge most perpendicular to the separation normal. 
    // The reference edge will be used to clip the incident edge vertices to generate the contact manifold.
    glm::normalize(ref);

    float o1 = Dot(ref, v1);

    clip(v1, v0, ref, o1);

    if (contactPoints.size() < 2)
        return;

    float o2 = Dot(ref, v0);

    clip(contactPoints[0], contactPoints[1], -ref, -o2);

    if (contactPoints.size() < 2)
        return;

    glm::vec2 refNorm = Cross(ref, -1.0f);

    if (flip)
    {
        refNorm = glm::vec2(refNorm.x * -1, refNorm.y * -1);
    }

    float max = Dot(refNorm, v1); // v0?       

    if (Dot(refNorm, contactPoints[0]) - max < 0.0f && !contactPoints.empty())
    {
        contactPoints.erase(contactPoints.begin());
    }
    if (Dot(refNorm, contactPoints[1]) - max < 0.0f && !contactPoints.empty())
    {
        contactPoints.erase(contactPoints.begin() + 1); // might be wrong?
        //contactPoints.erase(contactPoints.end() - 1);
    }
}

void NP_CollisionInfo::clip(glm::vec2 v1, glm::vec2 v0, glm::vec2 n, float offset)
{

    float d1 = Dot(n, v0) - offset;
    float d2 = Dot(n, v1) - offset;

    
    if (d1 >= 0.0f)
    {
        /*for (int i = 0; i < contactPoints.size(); ++i)
        {
            if (contactPoints[i] == v1)
                return;
        }*/
        contactPoints.emplace_back(v1);
    }
    if (d2 >= 0.0f)
    {
        /*for (int i = 0; i < contactPoints.size(); ++i)
        {
        if (contactPoints[i] == v0)
        return;
        }*/
        contactPoints.emplace_back(v0);
    }
        

    if (d1 * d2 < 0.0f)
    {
        glm::vec2 e = v0 - v1;

        float u = d1 / (d1 - d2);
        e = e * u;
        e = e + v1;

        for (int i = 0; i < contactPoints.size(); ++i)
        {
            if (contactPoints[i] == e)
                return;
        }

        contactPoints.emplace_back(e);
    }
}

std::vector<glm::vec2> NP_CollisionInfo::getContactPoints() const
{
    return contactPoints;
}
