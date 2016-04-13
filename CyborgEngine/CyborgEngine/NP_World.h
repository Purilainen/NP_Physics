#ifndef NP_WORLD_H
#define NP_WORLD_H

class NP_World
{
public:
	NP_World();
	~NP_World();

    void update();

private:
	float g = 9.81; // Gravity

    //std::vec<objects> m_objects
};
#endif;
