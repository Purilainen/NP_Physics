#ifndef FONT_H
#define FONT_H

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include <iostream>
#include <string>



class Font
{
public:
	Font();

	FT_Library library;
	FT_Face face;

	struct Character
	{
		GLuint TextID;
		glm::ivec2 size; //Size of glyph
		glm::ivec2 bearing; //Offset from baseline to left/top of glyph
		GLuint advance; //offset to advance to next glyph
	};

	void drawText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
	std::map<GLchar, Character> Characters;

	~Font();

	FT_Library initLib();
	FT_Face setFont(FT_Library lib, std::string filePath);
};

#endif