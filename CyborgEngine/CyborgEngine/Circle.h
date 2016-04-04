#ifndef CIRCLE_H
#define CIRCLE_H

#include "Polygon.h"

class Circle
	:public Polygon
{
public:
	Circle();
	~Circle();
	float getRange(){ return range; }
	int getPrecision(){ return precision; }

	void setRange(float range)
	{ 
		this->range = range;
		updatePoints();
	}
	void setPrecision(int num_vertices)
	{
		this->precision = num_vertices;
		updatePoints();
	}
private:
	void updatePoints();
	float range = 1;
	int precision = 16;
};

#endif