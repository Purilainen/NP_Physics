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

    bool PolyToPoly = false;
    if (PolyToPoly)
    {
        NP_Body* A = m_aBody;
        NP_Body* B = m_bBody;
        contact_count = 0;
        //Check for a seperating axis with A's face planes
        glm::uint32 faceA;
        float penetrationA = FindAxisLeastPenetration(&faceA, A, B);
        //if (penetrationA >= 0.0f)
        //    return;

        glm::uint32 faceB;
        float penetrationB = FindAxisLeastPenetration(&faceB, B, A);
        //if (penetrationB >= 0.0f)
        //    return;

        glm::uint32 referenceIndex;
        bool flip; // Always point from a to b

        NP_Body* refBody;
        NP_Body* incBody;

        if (BiasGreaterThan(penetrationA, penetrationB))
        {
            refBody = A;
            incBody = B;
            referenceIndex = faceA;
            flip = false;
        }
        else
        {
            refBody = B;
            incBody = A;
            referenceIndex = faceB;
            flip = true;
        }

        // World space incident face
        glm::vec2 incidentFace[2];
        FindIncidentFace(incidentFace, refBody, incBody, referenceIndex);

        // Setup reference face vertices
        glm::vec2 v1 = refBody->m_collider.corner[referenceIndex];
        referenceIndex = referenceIndex + 1 == refBody->m_collider.vertexCount ? 0 : referenceIndex + 1;
        glm::vec2 v2 = refBody->m_collider.corner[referenceIndex];

        // Transform vertices to world space
        v1 = refBody->m_collider.u * v1 + refBody->m_position;
        v2 = refBody->m_collider.u * v2 + refBody->m_position;

        // Calculate reference face side normal in world space
        glm::vec2 sidePlaneNormal = (v2 - v1);
        glm::normalize(sidePlaneNormal);

        // Orthogonalize
        glm::vec2 refFaceNormal(sidePlaneNormal.y, -sidePlaneNormal.x);

        // ax+by = c (where c is distance from origin)
        float refC = Dot(refFaceNormal, v1);
        float negSide = -Dot(sidePlaneNormal, v1);
        float posSide = Dot(sidePlaneNormal, v2);

        // Clip incident face to reference face side planes
        if (Clip(-sidePlaneNormal, negSide, incidentFace) < 2)
            return;

        if (Clip(sidePlaneNormal, posSide, incidentFace) < 2)
            return;

        // Flip
        normal = flip ? -refFaceNormal : refFaceNormal;

        // Keep points behind reference face
        int cp = 0; // Clipped points
        float seperation = Dot(refFaceNormal, incidentFace[0]) - refC;
        if (seperation <= 0.0f)
        {
            contactPoints[cp] = incidentFace[0];
            penetration = -seperation;
            ++cp;
        }
        else
            penetration = 0;

        seperation = Dot(refFaceNormal, incidentFace[1]) - refC;
        if (seperation <= 0.0f)
        {
            contactPoints[cp] = incidentFace[1];
            penetration += -seperation;
            ++cp;

            // Average penetration
            penetration /= (float)cp;
        }
        contact_count = cp;
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

float NP_CollisionInfo::FindAxisLeastPenetration(glm::uint32 *faceIndex, NP_Body* A, NP_Body* B)
{
    float bestDistance = -FLT_MAX;
    int bestIndex;

    int aVertCount = A->m_collider.vertexCount;
    int bVertCount = B->m_collider.vertexCount;

    for (size_t i = 0; i < aVertCount; ++i)
    {
        //Retrieve face normal from A
        glm::vec2 n = A->m_collider.normals[i];
        glm::vec2 nw = A->m_collider.u * n;

        // Transform face normal into B's model space
        glm::mat2 buT = glm::transpose(B->m_collider.u);
        n = buT * nw;

        // Retrieve support point from B along -n
        glm::vec2 s = B->m_collider.getSupport(-n);

        // Retrieve vertex on face from A, transform into B's model space
        glm::vec2 v = A->m_collider.corner[i];
        v = A->m_collider.u * v + A->m_position; 
        v -= B->m_position;
        v = buT * v;

        // Compute penetration distance (in B's model space)
        float d = Dot(n, s - v);

        if (d > bestDistance)
        {
            bestDistance = d;
            bestIndex = i;
        }
    }

    *faceIndex = bestIndex;
    return bestDistance;
}

void NP_CollisionInfo::FindIncidentFace(glm::vec2* v, NP_Body* refBody, NP_Body* incBody, int refIndex)
{
    glm::vec2 referenceNormal = refBody->m_collider.normals[refIndex];

    // Calc normal in incidents frame of reference
    referenceNormal = refBody->m_collider.u * referenceNormal; // To world space
    referenceNormal = glm::transpose(incBody->m_collider.u) * referenceNormal; // To incidents model space

    // Find most anti-normal face on incident polygon
    int incidentFace = 0;
    float minDot = FLT_MAX;

    for (size_t i = 0; i < incBody->m_collider.vertexCount; ++i)
    {
        float dotProduct = Dot(referenceNormal, incBody->m_collider.normals[i]);

        if (dotProduct < minDot)
        {
            minDot = dotProduct;
            incidentFace = i;
        }
    }

    // Assign face vertices for incidentFace
    v[0] = incBody->m_collider.u * incBody->m_collider.corner[incidentFace] + incBody->m_position;
    incidentFace = incidentFace + 1 >= (int)incBody->m_collider.vertexCount ? 0 : incidentFace + 1;
    v[1] = incBody->m_collider.u * incBody->m_collider.corner[incidentFace] + incBody->m_position;
}

bool NP_CollisionInfo::overlap(glm::vec2 projection1, glm::vec2 projection2)
{
    // x = min , y = max
    //if (projection1.x < projection2.y)
    //{
    //    return true;
    //}
    //if (projection1.y > projection2.x)
    //{
    //    return true;
    //}
    //else
    //    return false;



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

int NP_CollisionInfo::Clip(glm::vec2 n, float c, glm::vec2 *face)
{
    int sp = 0;
    glm::vec2 out[2] = { face[0], face[1] };

    // Retrieve distances from each endpoint to the line
    // distance = ax + by - c
    float d1 = Dot(n, face[0]) - c;
    float d2 = Dot(n, face[1]) - c;

    // If negative (behind plane) clip
    if (d1 <= 0.0f) out[sp++] = face[0];
    if (d2 <= 0.0f) out[sp++] = face[1];

    // If the points are on different sides of the plane
    if (d1 * d2 < 0.0f)
    {
        // Push intersection point
        float alpha = d1 / (d1 - d2);
        out[sp] = face[0] + alpha * (face[1] - face[0]);
        ++sp;
    }

    // Assign our new converted values
    face[0] = out[0];
    face[1] = out[1];

    return sp;

}
