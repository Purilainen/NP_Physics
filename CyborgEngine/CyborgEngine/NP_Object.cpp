#include "NP_Object.h"

NP_Object::NP_Object()
{
}

void NP_Object::bindPolygon(Polygon *polygon)
{
	m_polygon = polygon;
}

void NP_Object::bindBody(NP_Body *body)
{
    m_body = body;
}

NP_Object::~NP_Object()
{
}

void NP_Object::update()
{
    //Update attached body (Call update func)

    // Update polygons position/rotation to keep it in par with the physics body that its attached to.
    //m_polygon->setOrigin(m_body->getPos());
    //m_polygon->setRotation(m_body->getRotation());
}
