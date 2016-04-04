#include "Circle.h"

Circle::Circle()
{
	updatePoints();
}

Circle::~Circle()
{
}

void Circle::updatePoints()
{
	points = new glm::vec2[precision];
	numPoints = precision;
	points[0] = glm::vec2(0, 0);

	float dAlpha = 3.14159265*2 / precision;
	for (int i = 0; i < precision; i++)
	{
		points[i+1] = glm::vec2(cos(dAlpha*i)*range, sin(dAlpha*i)*range);
	}

	maxX = range;
	minX = -range;
	maxY = range;
	minY = -range;
}