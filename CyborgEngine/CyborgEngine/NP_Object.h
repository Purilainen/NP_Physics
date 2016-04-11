#ifndef NP_OBJECT_H
#define NP_OBJECT_H

#include "Polygon.h"

class Polygon;
class NP_Object
{
public:
	NP_Object();
	~NP_Object();

	void bindPolygon(Polygon *polygon);

private:
	Polygon *m_polygon;
};
#endif