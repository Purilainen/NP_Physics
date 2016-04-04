#ifndef Sprite_H
#define Sprite_H

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

class Sprite
{
	int spriteWidth, spriteHeight;
	int texWidth = 1, texHeight = 1;

public:

	Sprite(int tW, int tH, int sW, int sH);
	~Sprite();

	float getSpriteWidth(int rows){ return  1.0 / rows; }
	float getSpriteHeight(int columns){ return 1.0 / columns; }

};

#endif;