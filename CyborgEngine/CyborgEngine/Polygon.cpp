#include "Polygon.h"
#include "Renderer.h"

Polygon::Polygon()
{
	setRotation(0);
	setOrigin(0, 0);
	setScale(1.0);
}

Polygon::~Polygon()
{
	//how do I not leak memory?
}

void Polygon::setPoints(glm::vec2* p, int size)
{
	//float maxX = 0, maxY = 0, minX = 0, minY = 0;
	points = new glm::vec2[size+1];
	numPoints = 0;
	points[0] = glm::vec2(0, 0);
	for (int i = 0; i < size; i++)
	{
		points[numPoints+1] = p[i];
		numPoints++;
		if (p[i].x > maxX){ maxX = p[i].x; }
		if (p[i].y > maxY){ maxY = p[i].y; }
		if (p[i].x < minX){ minX = p[i].x; }
		if (p[i].y < minY){ minY = p[i].y; }
		points[0].x += p[i].x;
		points[0].y += p[i].y;
	}

	points[0].x /= size;
	points[0].y /= size;
}

//std::vector<glm::vec2>* Polygon::getPoints()
//{
//	rotatedPoints.clear();
//	//TODO: rotation matrix
//	for (auto point : points)
//	{
//		float dx = (point.x - origin.x);
//		float dy = (point.y - origin.y);
//		float l = sqrt(dx*dx + dy*dy);
//		float d = atan2(dy, dx) + rotation;
//		glm::vec2 temp(origin.x + cos(d)*l, origin.y + sin(d)*l);
//		rotatedPoints.push_back(temp);
//	}
//	return &rotatedPoints;
//}