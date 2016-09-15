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
    void Initialize(); // Pre-calculations for impulse solving
    void ApplyImpulse(); // Solve impulse and apply it   
    bool overlap(glm::vec2 projection1, glm::vec2 projection2);
    float getOverlap(glm::vec2 projection1, glm::vec2 projection2);    

    NP_Body *m_aBody;
    NP_Body *m_bBody;

    float penetration; // Depth of penetration
    glm::vec2 normal; // Normal vector from A to B
    glm::vec2 contactPoints[2]; // Points of contact during collision
    int contact_count; // Number of contacts during collision
    float e; // Mixed restitution?
};

#endif