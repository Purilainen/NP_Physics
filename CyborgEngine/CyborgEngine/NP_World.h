#ifndef NP_WORLD_H
#define NP_WORLD_H

#include "NP_Object.h"
#include <vector>
class NP_Object;
class NP_World
{
public:
	NP_World();
	~NP_World();

    void update(float deltaTime);

    void checkProximity(NP_Object* obj);

private:
    std::vector<NP_Object*> m_objects;
};
#endif;
