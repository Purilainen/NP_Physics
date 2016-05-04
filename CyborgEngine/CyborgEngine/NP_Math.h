#ifndef NP_MATH_H
#define NP_MATH_H

#include "glm/glm.hpp"

const float gravityScale = 1.f;
const glm::vec2 gravity(0, 10.f * gravityScale);
const float dt = (1.0f / 60.f);
const float PIE = 3.141592741f;
const float EPSILON = 0.0001f;

    inline glm::vec2 Cross(float a, const glm::vec2& v)
    {
        return glm::vec2(-a * v.y, a * v.x);
    }

    inline glm::vec2 Cross(const glm::vec2& v, float a)
    {
        return glm::vec2(a * v.y, -a * v.x);
    }

    inline float Cross(const glm::vec2& vA, const glm::vec2& vB)
    {
        return vA.x * vB.y - vA.y * vB.x;
    }

    inline float Dot(const glm::vec2& vA, const glm::vec2& vB)
    {
        return vA.x * vB.x + vA.y * vB.y;
    }

    inline bool BiasGreaterThan(float a, float b)
    {
        const float k_biasRelative = 0.95f;
        const float k_biasAbsolute = 0.01f;

        return a >= b * k_biasRelative + a * k_biasAbsolute;
    }



#endif