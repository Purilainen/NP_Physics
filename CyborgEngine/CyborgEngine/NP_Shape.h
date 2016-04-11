#ifndef NP_SHAPE_H
#define NP_SHAPE_H

#include "glm/glm.hpp"
#include <vector>

class NP_Shape
{
public:
    NP_Shape();
    ~NP_Shape();

    void setPoints();
    std::vector<glm::vec2> getPoints();

private:
    std::vector<glm::vec2> m_points;
};

#endif