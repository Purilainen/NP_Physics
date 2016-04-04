#include "TextureManager.h"

TextureManager::TextureManager()
{
	//Ladataan oletus tekstuuri kun texturemanager luodaan.
	loadTexture("default", "./textures/default.png");
	loadTexture("sprite", "./textures/spriteTest.bmp");
	loadTexture("sheet", "./textures/spriteSheetTest.png");
}

TextureManager::~TextureManager()
{
	// Tyhjennet‰‰n textures array.
	//glDeleteTextures(textures.size(), textures);
	textures.clear();
}

void TextureManager::loadTexture(std::string name, std::string filePath)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//ladataan kuva tiedostosta (PNG, BMP, JPG, TGA, DDS, PSD, HDR)
	const char* c = filePath.c_str();
	unsigned char* image = SOIL_load_image(c, &width, &height, 0, SOIL_LOAD_RGB);

	if (image == NULL) // Error jos kuvan lataus ep‰onnistuu
	{
		std::cout << "Could not load image \"" + name + "\"\n" + SOIL_last_result();
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//luodaan tekstuuri

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	textures.insert(make_pair(name, texture));

	//Vapautetaan kuva data muistista k‰ytˆn j‰lkeen

	SOIL_free_image_data(image);
	std::cout << "Loaded texture: " << name << std::endl;
}

void TextureManager::deleteTexture(std::string name)
{
	auto i = textures.find(name);

	if (i == textures.end())
	{
		std::cout << "Texture with this name doesn't exist." << std::endl;
		return;
	}

	GLuint id = i->second;
	//poistetaan openGL texture t‰ss‰
	glDeleteTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, NULL);

	textures.erase(i);
}

GLuint TextureManager::getTexture(std::string name)
{
		auto i = textures.find(name);

		if (i == textures.end())
		{
			std::cout << "Texture: " << name << " does not exist. Replaced with default texture" << std::endl;
			return getTexture("default");
		}
		else
		{
			return i->second;
		}
}