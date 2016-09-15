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
    void calcContactPoints();
    void clip(glm::vec2 v1, glm::vec2 v0,glm::vec2 n, float offset);


    std::vector<glm::vec2> getContactPoints() const;

    NP_Body *m_aBody;
    NP_Body *m_bBody;

    float penetration; // Depth of penetration
    glm::vec2 normal; // Normal vector from A to B
    std::vector<glm::vec2> contactPoints;
    int contact_count; // Number of contacts during collision
    float e; // Mixed restitution?
    int index;
};

#endif