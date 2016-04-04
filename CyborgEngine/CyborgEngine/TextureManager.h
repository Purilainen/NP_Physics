#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <unordered_map>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <fstream>
#include <SOIL.h>


class TextureManager
{
public:
	static TextureManager* getInstance()
	{
		static TextureManager instance;
		return &instance;
	}
	GLuint getTexture(const std::string name);
	void loadTexture(std::string name, std::string filePath);
private:
	TextureManager()
	{
		//ladataan default-tekstuuri
		loadTexture("default", "./textures/default.png");
		std::cout << "TM-init\n";
	}
	~TextureManager()
	{
		// Tyhjennetään textures array.
		textures.clear();
	}
	 std::vector<unsigned char> png;
	 int width, height;

	 std::unordered_map<std::string, GLuint> textures;
	 void deleteTexture(std::string name);

	 static TextureManager instance;
};

#endif