#include "Font.h"
#include "common\shader.hpp"

#include "TextureManager.h"

//---Freetype
GLuint fontProgramID;
GLuint TextID;
GLuint VAO, VBO;

TextureManager* TM;
//-------


Font::Font()
{
	fontProgramID = LoadShaders("shaders/FontVertexShader.txt", "shaders/FontFragmentShader.txt");
	library = initLib();
	setFont(library, "./fonts/Subway.ttf");
}


Font::~Font()
{
	FT_Done_Face(face);
	FT_Done_FreeType(library);
}

FT_Library Font::initLib()
{
	

	if (FT_Init_FreeType(&library))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;



	return library;
}

FT_Face Font::setFont(FT_Library lib, std::string filePath)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	//const char* fileP = filePath.c_str();

	//FT_New_Face(library, fileP, 0, &face);
	
	if (FT_New_Face(library, "./fonts/arial.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	//FT_Set_Char_Size()
	FT_Set_Pixel_Sizes(face, 0, 48);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


	//Looppi säilöö 128 ensimmäistä ASCII merkkiä
	for (GLubyte c = 0; c < 128; c++)
	{
		//Ladataan merkki "character glyph"
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}

		//Tehdään tekstuuri
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer);

		//Texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	
		//Säilötään merkki myöhempää käyttöä varten
		Character character =
		{
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	//glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(face);
	FT_Done_FreeType(library);

	

	return face;
}

void Font::drawText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	//Aktivoidaan oikea renderi
	glUseProgram(fontProgramID);
	TextID = glGetUniformLocation(fontProgramID, "myTextSampler");
	glUniform3f(glGetUniformLocation(TextID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);

	//läpinäkyvyydet
	

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	
	


	//Käydään läpi annetut merkit
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.bearing.x * scale;
		GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;

		GLfloat w = ch.size.x * scale;
		GLfloat h = ch.size.y * scale;
		// Päivitetään vertex buffer jokaiselle merkille
	/*	GLfloat vertices[6][4] = {
			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos, ypos, 0.0, 1.0 },
			{ xpos + w, ypos, 1.0, 1.0 },

			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos + w, ypos, 1.0, 1.0 },
			{ xpos + w, ypos + h, 1.0, 0.0 }
		};*/
		GLfloat vertices[6][4] = {
			{1, 0.5, 0.0, 0.0 },
			{0.5,0.5, 0.0, 1.0 },
			{ 0.7, 0.7, 1.0, 1.0 },

			{1,0, 0.0, 0.0 },
			{0.5, 0.5, 1.0, 1.0 },
			{1, 1, 1.0, 0.0 }
		};
		// Renderöidään merkin tekstuuri neliön päälle
		glBindTexture(GL_TEXTURE_2D, /*ch.TextID*/ TM->getTexture("testi"));
		// Päivitetään vertex bufferin sisältö
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Renderöidään neliö
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// otetaan askel seuraavaa merkkiä varten
		x += (ch.advance >> 6) * scale;
		
	}
	
	//glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	//glDisableVertexAttribArray(0);
	//glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
};