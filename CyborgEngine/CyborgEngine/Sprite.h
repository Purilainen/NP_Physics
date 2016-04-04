#ifndef Sprite_H
#define Sprite_H
#include <unordered_map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <string>
#include <iostream>

class Sprite
{

public:

	Sprite();
	~Sprite();


	void setUp(int c,int d)
	{ 
		rows = c; 
		columns = d;
		setSprite();
	}
	void setSprite();

	int getRows(){ return rows; }
	int getColumns(){ return columns; }
	float getSpriteWidth(){ return  (1.0 / rows); }
	float getSpriteHeight(){ return (1.0 / columns); }
	glm::vec2 getSprite(int a);


	private:
	int rows, columns;
	std::string texture;
	std::unordered_map<int, glm::vec2> coordinates;
	


};

#endif;