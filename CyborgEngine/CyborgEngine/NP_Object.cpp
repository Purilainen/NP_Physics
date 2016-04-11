#include "NP_Object.h"

NP_Object::NP_Object()
{
}

void NP_Object::bindPolygon(Polygon *polygon)
{
	m_polygon = polygon;
}

NP_Object::~NP_Object()
{
}