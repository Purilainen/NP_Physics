#include "NP_Object.h"

NP_Object::NP_Object()
{}

void NP_Object::bindPolygon(Polygon *polygon)
{
	m_polygon = polygon;
}

void NP_Object::bindBody(NP_Body *body)
{
    m_body = body;
}

void NP_Object::setOrient(float orient)
{
    m_body->setOrient(orient);
    m_polygon->setRotation(orient);
}

NP_Object::~NP_Object()
{}

void NP_Object::update(float deltaTime)
{
    m_body->update(deltaTime);
    m_polygon->setRotation(m_body->m_orientation);
}
