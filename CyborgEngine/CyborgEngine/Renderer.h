#ifndef RENDERER_H
#define RENDERER_H

//tarvitaankko n‰it‰ kaikkia?
//TODO: poista ylim‰‰r‰iset, siirr‰ osa .cpp-tiedostoon
#include <cstdio>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "Polygon.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "PointSprite.h"
class Renderer
{
public:

	enum Colors
	{
		CRed = 0xFF0000FF,
		CGreen = 0x00FF00FF,
		CBlue = 0x0000FFFF,
		CYellow = 0xFFFF00FF,
		CMagneta = 0xFF00FFFF,
		CCyan = 0x00FFFFFF,
		COrange = 0xFF8000FF,
		CBlack = 0x000000FF,
		CWhite = 0xFFFFFFFF,
		CGray = 0x808080FF,
	};


	void static render();
	void static initRender(GLFWwindow* w);
	void static uninitRender();
	void static initDraw();
	void static FramebufferSizeCallback(GLFWwindow* window, int width, int height);

	void static drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
	void static drawTexturedTriangle(float x1, float y1, float x2, float y2, float x3, float y3, std::string texture);
	void static drawRectangle(float x1, float y1, float x2, float y2);
	void static drawTexturedRectangle(float x1, float y1, float x2, float y2, std::string textureName);
	void static drawCircle(float x, float y, float r);
	void static drawPie(float x, float y, float r, float percentage, float start);
	void static drawPolygon(Polygon* p, const float x, const float y);
	void static drawPolygonTextured(Polygon* p, const float x, const float y, std::string textureName);
	void static drawMultiColorTriangle(float x1, float y1, float x2, float y2, float x3, float y3, int color1, int color2, int color3);
	void static drawLine(float startPointX, float startPointY, float endPointX, float endPointY, float width);

	void static key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	void static do_movement();
	glm::mat4 static getMVP();

	void static drawSingleSprite(float posX, float posY, float height, float width,std::string textureName);

	void static drawSprite(Sprite *SP,float posX, float posY, float spriteWidth , float spriteHeight,std::string textureName, float alpha = 1.0);


	void static drawPointSprite(float x, float y, float scale, PointSprite p, float alpha = 0);

	void static setColor(float r, float g, float b, float a);
	void static setColor(int color);

	Renderer(){}
	~Renderer(){}
};



#endif