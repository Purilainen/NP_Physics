#include "NP_Body.h"
#include <Windows.h>
#include <iostream>

void NP_Body::update(float deltaTime)
{
    m_collider.position = m_position;

    if (Static)
    {}
    else if (Kinematic)
    {}
    else if (Dynamic)
    {}
}

void NP_Body::addBoxCollider(float size)
{
    //TODO : Support for rectangles and not only squares
    //NOTE: Use the same points that are used when making a rectangle poly
    m_collider.position = m_position;

    m_collider.corner[0] = { glm::vec2(-0.25, 0.25)*size };
    m_collider.corner[1] ={glm::vec2(0.25, 0.25)*size};
    m_collider.corner[2] = {glm::vec2(0.25, -0.25)*size};
    m_collider.corner[3] = {glm::vec2(-0.25, -0.25)*size};
    
    
    m_collider.max = glm::vec2(0.25, 0.25);
    m_collider.min = glm::vec2(-0.25, -0.25);

    for (size_t i = 0; i < m_collider.vertexCount; ++i)
    {
        int i2 = i + 1 < m_collider.vertexCount ? i + 1 : 0;
        glm::vec2 face = m_collider.corner[i2] - m_collider.corner[i];

        m_collider.normals[i] = glm::vec2(face.y, -face.x);
        glm::normalize(m_collider.normals[i]);
    }

    m_collider.rotation = m_orientation;
    m_collider.size = size;
    //computeAxes();
}

void NP_Body::computeAxes()
{
    for (size_t i = 0; i < 3; ++i)
    {
        // Get current vertex
        glm::vec2 p1 = m_collider.corner[i];
        // Get next vertex
        glm::vec2 p2 = m_collider.corner[i + 1 == 4 ? 0 : i + 1];
        // Subtract the two to get the edge vector
        glm::vec2 edge = p1 - p2;
        glm::vec2 zero(0, 0);
        if (edge == zero)
        {
            edge = p2 - p1;
        }
        // Get either perpendicular vector
        glm::vec2 leftHandNormal(edge.x, -edge.y);
        glm::vec2 rightHandNormal(-edge.x, edge.y);

        m_collider.axes[i] = glm::normalize(rightHandNormal); // Normalize - Or right handed?

    }

    glm::vec2 testEdge = m_collider.corner[3] - m_collider.corner[0];
    if (testEdge == glm::vec2(0))
    {
        testEdge = m_collider.corner[0] - m_collider.corner[3];
    }

    //glm::vec2 lefty(testEdge.x, -testEdge.y);
    //m_collider.axes[3] = glm::normalize(lefty);

	glm::vec2 righty(-testEdge.x, testEdge.y);
	m_collider.axes[3] = glm::normalize(righty);
}

void NP_Body::addForce(float forceX, float forceY)
{
    m_force.x = forceX;
    m_force.y = forceY;
}

void NP_Body::addImpulse(glm::vec2 impulse, glm::vec2 contactVector)
{
    m_velocity += inverseMass * impulse;
    m_angularVelocity += 0.000001f * Cross(contactVector, impulse);
}

NP_Body::NP_Body(NP_World world) : Static(false), Kinematic(false), Dynamic(true), m_world(&world), m_velocity(0), m_position(0), m_restitution(0.2f)
{
    //Bodies are dynamic by default
    inverseMass = 1 / m_mass;
    float inertia = m_mass * 0.125f * 0.125f;
    inverseInertia = inertia ? 1.0f / inertia : 0.0f;
}

NP_Body::~NP_Body()
{}