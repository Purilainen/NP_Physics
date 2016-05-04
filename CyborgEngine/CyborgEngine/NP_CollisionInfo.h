#ifndef NP_COLLISIONINFO_H
#define NP_COLLISIONINFO_H

#include "NP_Body.h"

class NP_Body;
class NP_CollisionInfo
{
public:
    NP_CollisionInfo(NP_Body *a, NP_Body *b);
    ~NP_CollisionInfo();

    void Solve(); // Generate collision info
    void Initialize(); // Precalculations for impulse solving
    void ApplyImpulse(); // Solve impulse and apply it
    void PositionalCorrection(); // Correction of positional penetration
    void InfiniteMassCorrection();
    float FindAxisLeastPenetration(glm::uint32 *faceIndex, NP_Body* A, NP_Body* B);
    void FindIncidentFace(glm::vec2* v, NP_Body* refBody, NP_Body* incPoly, int refIndex);

    int Clip(glm::vec2 n, float c, glm::vec2 *face);

    NP_Body *m_aBody;
    NP_Body *m_bBody;

    float penetration; // Depth of penetration
    glm::vec2 normal; // Normal vector from A to B
    glm::vec2 contactPoints[2]; // Points of contanct during collision
    int contact_count; // Number of contacts during collision
    float e; // Mixed restitution?
};

#endif