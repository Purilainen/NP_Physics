#include "Renderer.h"
#include "common\shader.hpp"

#define PI 3.14159265

namespace {
	GLuint programID;
	GLuint textureProgramID;
	//GLuint vertexbuffer;
	GLuint VertexArrayID;
	//GLuint indexbuffer;
	GLFWwindow* window; 
	glm::mat4 MVP(1.0);
	GLuint MVP_MatrixID;
	GLuint TextureID;
	GLuint uvbuffer;
	GLuint colorbuffer;
	GLuint Texture;
	glm::mat4 VP;
	glm::vec4 DefaultColor;
	int N_shapes;
	TextureManager* TM;
	Sprite *SP;
};

void Renderer::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
	float scale = width / height;
	glViewport(0, 0, width / scale, height);
}

void Renderer::initDraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Tyhjennetään ruutu
	// Koordinaatisto / Viewport
	glm::vec3 x_axis(1.0, 0.0, 0.0);
	glm::vec3 y_axis(0.0, 1.0, 0.0);
	glm::vec3 z_axis(0.0, 0.0, 1.0);

	glm::vec3 cam_pos(0, 0, 0);
	glm::vec3 cam_up = y_axis;
	glm::vec3 cam_right = x_axis;
	glm::vec3 cam_front = -z_axis; //oikeakatinen koordinaatisto
	glm::mat4 P = glm::lookAt(cam_pos, cam_pos + cam_front, cam_up);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glm::mat4 V = glm::ortho(-1.0f, 1.0f, -1.0f*height / width, 1.0f*height / width);
	VP = V*P;

	glUseProgram(programID);
	// ---------------------------
}


void Renderer::render()
{
	glfwSwapBuffers(window);
}

void Renderer::initRender(GLFWwindow* w)
{
	window = w;
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Taustaväri - musta

	//asetetaan piirtoväri oranssiksi
	DefaultColor.r = 1.0f;
	DefaultColor.g = 0.4f;
	DefaultColor.b = 0.0f;
	DefaultColor.a = 1.0f;

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	TM = new TextureManager;
	TM->loadTexture("testi", "./textures/polygon.png");

	//Ladataan shaderit
	//valmiissa ohjelmassa bool setShaders() -funktio ajonaikaiseen shaderien vaihtoon?
	programID = LoadShaders("shaders/VertexShader.vertexshader", "shaders/FragmentShader.fragmentshader");
	textureProgramID = LoadShaders("shaders/TextureVertexShader.txt", "shaders/TextureFragmentShader.txt");
	//----------------

	//luodaan väribufferi. 
	//TODO: siirrä muualle myöhemmin?

	static const GLfloat g_color_buffer_data[] = {
		DefaultColor.r, DefaultColor.g, DefaultColor.b, DefaultColor.a,
		DefaultColor.r, DefaultColor.g, DefaultColor.b, DefaultColor.a,
		DefaultColor.r, DefaultColor.g, DefaultColor.b, DefaultColor.a,
	};

	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	//MVP näkyy shadereille
	MVP_MatrixID = glGetUniformLocation(programID, "MVP");

	//glEnable(jotain)
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
}

void Renderer::uninitRender()
{
	// ???
	//glDeleteBuffers(1, &vertexbuffer);
	//glDeleteBuffers(1, &indexbuffer);
	//glDeleteBuffers(1, &colorbuffer);
}

void Renderer::drawRectangle(float x1, float y1, float x2, float y2)
{
	drawTriangle(x1, y1, x1, y2, x2, y2);
	drawTriangle(x1, y1, x2, y1, x2, y2);
}

void Renderer::drawCircle(float x, float y, float r)
{
	float angle = 2.0*3.14159265 / 64;
	for (int i = 0; i < 64; i++)
	{
		drawTriangle(x, y, x + cos(angle * i)*r, y + sin(angle * i)*r, x + cos(angle * (i + 1))*r, y + sin(angle * (i + 1))*r);
	}
}

void Renderer::drawPie(float x, float y, float r,float a)
{
	float angle = 2.0*3.14159265 / 64 * a;
	for (int i = 0; i < 64; i++)
	{
		drawTriangle(x, y, x + cos(angle * i)*r, y + sin(angle * i)*r, x + cos(angle * (i + 1))*r, y + sin(angle * (i + 1))*r);
	}
}

void Renderer::drawPolygon(Polygon* p, const float x, const float y)
{
	glm::vec2* points = p->getPoints();
	const int numPoints = p->getNumPoints();
	glm::vec2 c = points[0];

	float x0 = p->getOrigin().x;
	float y0 = p->getOrigin().y;

	for (unsigned i = 1; i < numPoints+1; i++) //aloita 1:stä, koska ensimmäinen vec2 on kuvion keskipiste
	{
		glm::vec2 next = (i == numPoints) ? points[1] : points[i + 1];
		glm::mat4 MVP_saved = MVP;
		
		MVP = MVP * glm::translate(glm::vec3(x, y, 0));
		MVP = MVP * glm::rotate(p->getRotation(), glm::vec3(0, 0, 1));
		MVP = MVP * glm::translate(glm::vec3(p->getOrigin(), 0));

		drawTriangle(c.x, c.y, points[i].x, points[i].y, next.x, next.y);
		MVP = MVP_saved;
	}
}

void Renderer::drawMultiColorTriangle(float x1, float y1, float x2, float y2, float x3, float y3, int color1, int color2, int color3)
{
	//Tämä valmiiksi sitten kun saadaan väreille ne nimet
	float r = (float)((color1 >> 16) & 0xFF) / 255;
	float g = (float)((color1 >> 8) & 0xFF) / 255;
	float b = (float)((color1 >> 0) & 0xFF) / 255;

	float r2 = (float)((color2 >> 16) & 0xFF) / 255;
	float g2 = (float)((color2 >> 8) & 0xFF) / 255;
	float b2 = (float)((color2 >> 0) & 0xFF) / 255;

	float r3 = (float)((color3 >> 16) & 0xFF) / 255;
	float g3 = (float)((color3 >> 8) & 0xFF) / 255;
	float b3 = (float)((color3 >> 0) & 0xFF) / 255;

	GLfloat g_color_buffer_data[] =
	{
		r, g, b,
		r2, g2, b2,
		r3, g3, b3,
	};
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	drawTriangle(x1, y1, x2, y2, x3, y3);

}

void Renderer::drawPolygonTextured(Polygon* p, const float x, const float y, std::string textureName)
{
	//get points from polygon
	glm::vec2* points = p->getPoints();
	int numTriangles = p->getNumPoints();

	GLuint uvbuffer;

	glEnable(GL_TEXTURE_2D);

	glUseProgram(textureProgramID);
	TextureID = glGetUniformLocation(textureProgramID, "myTextureSampler");

	GLuint texture = TM->getTexture(textureName);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glUniform1i(TextureID, 0);

	glm::mat4 MVP_temp = MVP;
	MVP = MVP * glm::translate(glm::vec3(x, y, 0));
	MVP = MVP * glm::rotate(p->getRotation(), glm::vec3(0, 0, 1));
	//MVP = MVP * glm::scale(glm::vec3(0.5f, 0.5f, 0.5f));

	float width = p->getMax().x - p->getMin().x;
	float height = p->getMax().y - p->getMin().y;

	GLfloat* uvData = new GLfloat[(numTriangles+1) * 3];
	
	//uvData[0] = points[0].x;
	//uvData[0] = points[0].y;

	float minX = p->getMin().x;
	float minY = p->getMin().y;

	float maxX = p->getMax().x;
	float maxY = p->getMax().y;

	for (int i = 0; i < numTriangles+1; i++)
	{
		uvData[i * 2 + 0] = (points[i].x - minX) / width;
		uvData[i * 2 + 1] = (points[i].y - minY) / height;
	}

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(numTriangles + 1) * 2, uvData, GL_DYNAMIC_DRAW);

	GLuint vb, ib;

	GLfloat* vertexData = new GLfloat[(numTriangles + 1) * 3];
	for (int i = 0; i < (numTriangles + 1); i++)
	{	
		vertexData[i * 3 + 0] = points[i].x + p->getOrigin().x; //x
		vertexData[i * 3 + 1] = points[i].y + p->getOrigin().y; //y
		vertexData[i * 3 + 2] = 1.0f;		 //z
	}

	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(numTriangles+1)*3, vertexData, GL_DYNAMIC_DRAW);

	GLubyte* indexData = new GLubyte[(numTriangles+1) * 3];
	for (int i = 0; i < numTriangles; i++)
	{
		indexData[i * 3 + 0] = 0;
		indexData[i * 3 + 1] = i;
		indexData[i * 3 + 2] = i+1;
	}
	indexData[(numTriangles) * 3 + 0] = 0;
	indexData[(numTriangles) * 3 + 1] = numTriangles;
	indexData[(numTriangles) * 3 + 2] = 1;

	glGenBuffers(1, &ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte)*(numTriangles+1)*3, indexData, GL_DYNAMIC_DRAW);

	glUniformMatrix4fv(MVP_MatrixID, 1, GL_FALSE, &MVP[0][0]);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);


	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,                  // attribute 1
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);


	glDrawElements(GL_TRIANGLES, (numTriangles+1)*3, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);


	glDeleteBuffers(1, &vb);
	glDeleteBuffers(1, &ib);
	glDeleteBuffers(1, &uvbuffer);

	delete[] uvData;
	delete[] indexData;
	delete[] vertexData;

	glDisable(GL_TEXTURE_2D);

	MVP = MVP_temp;
}

void Renderer::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3)
{
	glEnable(GL_BLEND);

	GLuint vb, ib;

	GLfloat g_vertex_buffer_data[] = {
		x1, y1, 1.0f,
		x2, y2, 1.0f,
		x3, y3, 1.0f,
	};

	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);
	
	/*static const */GLubyte g_indices[] =
	{
		0, 1, 2,
	};
	glGenBuffers(1, &ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_indices), g_indices, GL_DYNAMIC_DRAW);

	glUniformMatrix4fv(MVP_MatrixID, 1, GL_FALSE, &MVP[0][0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glVertexAttribPointer(
		0,                  // attribute 
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		1,                  // attribute 1
		4,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);


	glDeleteBuffers(1, &vb);
	glDeleteBuffers(1, &ib);

	glDisable(GL_BLEND);
}

void Renderer::drawTexturedTriangle(float x1, float y1, float x2, float y2, float x3, float y3, std::string textureName)
{
	//way too experimental:

	GLuint uvbuffer;

	glEnable(GL_TEXTURE_2D);

	glUseProgram(textureProgramID);
	TextureID = glGetUniformLocation(textureProgramID, "myTextureSampler");
	
	
	//GLuint texture = loadBMP_custom("textures/default.bmp");
	//glGenTextures(1, &texture);

	GLuint texture = TM->getTexture(textureName);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glUniform1i(TextureID, 0);

	GLfloat* g_uv_buffer_data = new GLfloat[6];

	//static const GLfloat g_uv_buffer_data[] =
	//{
	//	0.0, 0.0,
	//	1.0, 0.0,
	//	0.0, 1.0,
	//};
	g_uv_buffer_data[0] = 0.0; g_uv_buffer_data[1] = 0.0;
	g_uv_buffer_data[2] = 1.0; g_uv_buffer_data[3] = 0.0;
	g_uv_buffer_data[4] = 0.0; g_uv_buffer_data[5] = 1.0;

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, /*sizeof(g_uv_buffer_data)*/6*sizeof(GLfloat), g_uv_buffer_data, GL_DYNAMIC_DRAW);

	GLuint vb, ib;

	GLfloat g_vertex_buffer_data[] = {
		x1, y1, 1.0f,
		x2, y2, 1.0f,
		x3, y3, 1.0f,
	};

	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);

	GLubyte g_indices[] =
	{
		0, 1, 2,
	};
	glGenBuffers(1, &ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_indices), g_indices, GL_DYNAMIC_DRAW);

	glUniformMatrix4fv(MVP_MatrixID, 1, GL_FALSE, &MVP[0][0]);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,                  // attribute 1
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);


	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);


	glDeleteBuffers(1, &vb);
	glDeleteBuffers(1, &ib);
	glDeleteBuffers(1, &uvbuffer);
}

void Renderer::drawTexturedRectangle(float x1, float y1, float x2, float y2, std::string textureName)
{
	//Tekstuuri temput ---------
	glEnable(GL_TEXTURE_2D);
	glUseProgram(textureProgramID);
	TextureID = glGetUniformLocation(textureProgramID, "myTextureSampler");
	GLuint texture = TM->getTexture(textureName);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(TextureID, 0);
	//-----------------------------

	GLuint uvbuffer;

	static const GLfloat g_uv_buffer_data[] =
	{
		0.0, 0.0,
		0.0, 1.0,
		1.0, 0.0,
		1.0, 1.0,
	};

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_DYNAMIC_DRAW);

	GLuint vb, ib;

	GLfloat g_vertex_buffer_data[] = {
		x2, y1, 1.0f,
		x2, y2, 1.0f,
		x1, y1, 1.0f,
		x1, y2, 1.0f,
	};

	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);

	GLubyte g_indices[] =
	{
		0, 1, 2, 3, 2, 1
		//0, 2, 1, 3, 2, 1
	};
	glGenBuffers(1, &ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_indices), g_indices, GL_DYNAMIC_DRAW);

	glUniformMatrix4fv(MVP_MatrixID, 1, GL_FALSE, &MVP[0][0]);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);


	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,                  // attribute 1
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);


	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);


	glDeleteBuffers(1, &vb);
	glDeleteBuffers(1, &ib);
	glDeleteBuffers(1, &uvbuffer);
	glDisable(GL_TEXTURE_2D);
}

void Renderer::drawLine(float startPointX, float startPointY, float endPointX, float endPointY, float width)
{
	//Piirrä viiva:
	//Onko tämä järkevä?????

	GLuint bv, bi;

	GLfloat g_vertex_buffer_data[] = {
		startPointX, startPointY, 1.0f,
		endPointX, endPointY, 1.0f,
	};

	glLineWidth(width);

	glGenBuffers(1, &bv);
	glBindBuffer(GL_ARRAY_BUFFER, bv);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);

	GLubyte g_indices[] =
	{
		0, 1,
	};
	glGenBuffers(1, &bi);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bi);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_indices), g_indices, GL_DYNAMIC_DRAW);

	glUniformMatrix4fv(MVP_MatrixID, 1, GL_FALSE, &MVP[0][0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bi);
	glVertexAttribPointer(
		0,                  // attribute 
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		1,                  // attribute 1
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	glDrawElements(GL_LINE_LOOP, 2, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);


	glDeleteBuffers(1, &bv);
	glDeleteBuffers(1, &bi);


}
void Renderer::drawSprite(float posX, float posY, int rows, int colums, std::string textureName)
{
	glDisable(GL_MULTISAMPLE);
	//Tekstuuri temput ---------
	glEnable(GL_TEXTURE_2D);
	glUseProgram(textureProgramID);
	TextureID = glGetUniformLocation(textureProgramID, "myTextureSampler");
	GLuint texture = TM->getTexture(textureName);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(TextureID, 0);
	GLuint vb, ib;

	float spriteWidth = SP->getSpriteWidth(rows);
	float spriteHeight = SP->getSpriteHeight(colums);

	//Spriteen neliö:
	GLfloat g_vertex_buffer_data[] = {
		posX, posY,
		posX, posY + spriteHeight,
		posX + spriteWidth, posY + spriteHeight,
		posX + spriteWidth, posY,
	};

	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);


	static const GLfloat g_uv_buffer_data[] =
	{
		0.0, 0.0,
		0.5, 0.0,
		0.5, 0.5,
		0.0, 0.5,
	};
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_DYNAMIC_DRAW);

	GLubyte g_indices[] =
	{
		0, 1, 2,
		0, 3, 2,
	};
	glGenBuffers(1, &ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_indices), g_indices, GL_DYNAMIC_DRAW);

	glUniformMatrix4fv(MVP_MatrixID, 1, GL_FALSE, &MVP[0][0]);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);


	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,                  // attribute 1
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);


	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);


	glDeleteBuffers(1, &vb);
	glDeleteBuffers(1, &ib);
	glDeleteBuffers(1, &uvbuffer);
	glDisable(GL_TEXTURE_2D);

	//GLfloat g_vertex_buffer_data[] =
	//{
	//	x1, y1,
	//	x1 + 1, y1,
	//	x1 + 1, y1 + 1,
	//	x1, y1 + 1,
	//};

	//const float tw = float(1) / 1;
	//const float th = float(spriteHeight) / texHeight;
	//const int numPerRow = texWidth / spriteWidth;
	//const float tx = (frameIndex % numPerRow) * tw;
	//const float ty = (frameIndex / numPerRow + 1) * th;
	//const float texVerts[] =
	//{
	//	tx, ty,
	//	tx + tw, ty,
	//	tx + tw, ty + th,
	//	tx, ty + th
	//};
}
void Renderer::setColor(float r, float g, float b, float a)
{
	DefaultColor.r = r;
	DefaultColor.g = g;
	DefaultColor.b = b;
	DefaultColor.a = a;

	GLfloat g_color_buffer_data[] = {
		DefaultColor.r, DefaultColor.g, DefaultColor.b, DefaultColor.a,
		DefaultColor.r, DefaultColor.g, DefaultColor.b, DefaultColor.a,
		DefaultColor.r, DefaultColor.g, DefaultColor.b, DefaultColor.a,
	};
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
}


void Renderer::setColor(int color)
{
	float r = (float)((color >> 16) & 0xFF) / 255;
	float g = (float)((color >> 8) & 0xFF) / 255;
	float b = (float)((color >> 0) & 0xFF) / 255;
	setColor(r, g, b, 1);
}