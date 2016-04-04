#ifndef P_SPRITE_H
#define P_SPRITE_H

#include <string>

class PointSprite
{
public:
	PointSprite(float x, float y, std::string texture);
	std::string getTexture(){ return texture; }

	~PointSprite();
private:
	float x, y, time;
	std::string texture;
};
#endif