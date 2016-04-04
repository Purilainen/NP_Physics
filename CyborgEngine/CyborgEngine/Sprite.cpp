#include "Sprite.h"



Sprite::Sprite()
{
	rows = 0;
	columns = 0;

}


Sprite::~Sprite()
{
	coordinates.clear();
}

void Sprite::setSprite()
{

	float spriteW = getSpriteWidth();
	float spriteH = getSpriteHeight();
	int size = rows*columns;
	/*std::cout << numberPerRow << std::endl;*/
	for (int i = 0; i < size; i++)
	{
		float tX = (i%rows)*spriteW;
		float tY = (i / rows)*spriteH;
		glm::vec2 spriteCoords = { tX, tY };
		coordinates.insert(std::make_pair(i, spriteCoords));
	}
}
glm::vec2 Sprite::getSprite(int a)
{
	auto i = coordinates.find(a);
	if (i == coordinates.end())
	{
		std::cout << "Allah on ainoa jumala ja Muhammed on hänen profeettansa\n";
		return glm::vec2(0, 0);
	}
	else
	{
		return i->second;
	}
}
