#ifndef NP_OBJECT_H
#define NP_OBJECT_H

#include "Polygon.h"
#include "NP_Body.h"



class NP_Body;
class Polygon;
class NP_Object
{
public:
	NP_Object();
	~NP_Object();

    void update(float deltaTime);

	void bindPolygon(Polygon *polygon);
    void bindBody(NP_Body *body);
    void setOrient(float orient);

    NP_Body* getBody() { return m_body; }

private:
	Polygon *m_polygon;
    NP_Body *m_body;
};
#endif