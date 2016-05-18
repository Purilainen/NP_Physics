#define GLEW_STATIC
#define GLM_FORCE_RADIANS

#define PI 3.14159265



#include "Renderer.h"
#include "Circle.h"
GLFWwindow* window;
#include "PointSprite.h"
#include "Sprite.h"

#include "NP_World.h"
#include "NP_Body.h"
#include "NP_Object.h"




int main()
{

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//int w = 1024;
	int w = 768;
	int h = 768;
	window = glfwCreateWindow(w, h,
		"Cyborg-Engine", NULL, NULL);
	if (window == NULL){
		fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, Renderer::FramebufferSizeCallback);
	glfwSetKeyCallback(window, Renderer::key_callback);
	Renderer::FramebufferSizeCallback(window, w, h);
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	Renderer::initRender(window);

	//load resources:
	TextureManager* TM = TextureManager::getInstance();

	TM->loadTexture("testi", "./textures/polygon.png");
	TM->loadTexture("part", "./textures/particle2.png");
	TM->loadTexture("sheet","./textures/spriteSheetTest.png");
	float a = 0;
	float b = 0;			// For rotation
	float spriteTimer = 0;
	int spriteNumber = 0;

	PointSprite ps(0, 0, "part");
	
	//sprite stuff
	Sprite aa;
	aa.setUp(6,5);
	glm::vec2 rain[50];
	for (int i = 0; i < 50; i++)
	{
		rain[i] = glm::vec2((float)(rand() % 2000) / 1000 - 1.0, (float)(rand() % 2000) / 1000 - 1.2);
	}

	Polygon p;
	
	glm::vec2 points[]
	{

		glm::vec2(0.0, 0.21),
			glm::vec2(0.05, 0.05),
			glm::vec2(0.4, 0.0),
			glm::vec2(0.05, -0.05),
			glm::vec2(0.0, -0.21),
			glm::vec2(-0.05, -0.05),
			glm::vec2(-0.4, 0.0),
			glm::vec2(-0.05, 0.05),
	};
	p.setPoints(points, 8);
	p.setOrigin(-0.5, 0.0);

	Circle c;
	c.setPrecision(8);

	bool isEnterPressed = false;
	int scene = 0;

    //INITIALIZATION FOR NP_PHYSICS STUFF -------------------------------------------------------------------- START
    // Creating physics world
    NP_World world;
    // Creating objects
    NP_Object testObj, testObj2, testObj3;
    // Creating bodies to the world
    NP_Body testBody(world), testBody2(world), testBody3(world);

    // Binding objects to bodies
    testObj.bindBody(&testBody);
    testObj2.bindBody(&testBody2);
    testObj3.bindBody(&testBody3);

    testBody.addBoxCollider(0.5f);
    testBody2.addBoxCollider(0.5f);
    testBody3.addBoxCollider(0.5f);

  

    Polygon poly1, poly2, poly3;
    glm::vec2 pointsp[] =
    {
        // Box collider size
        glm::vec2(-0.25, 0.25),
        glm::vec2(-0.25, -0.25),
        glm::vec2(0.25, -0.25),
        glm::vec2(0.25, 0.25)
    };
    glm::vec2 pointsp123[] =
    {
        // Box collider size
        glm::vec2(-0.5, 0.5),
        glm::vec2(-0.5, -0.5),
        glm::vec2(0.5, -0.5),
        glm::vec2(0.5, 0.5)
    };
    poly1.setPoints(pointsp, 4);
    poly2.setPoints(pointsp, 4);
    poly3.setPoints(pointsp, 4);



    testObj.bindPolygon(&poly1);
    testObj2.bindPolygon(&poly2);
    testObj3.bindPolygon(&poly3);

	// Adding objects to the wold
    world.addObject(&testObj);
	world.addObject(&testObj2);
    world.addObject(&testObj3);

    testObj3.getBody()->setPos(glm::vec2(-1.f, 0.25f));
    testObj2.getBody()->setPos(glm::vec2(0.0f, 0.75f));
    testObj.getBody()->setPos(glm::vec2(1.0f, 0.f));

    
    testObj.getBody()->setVelocity(glm::vec2(-20.0f, 1.f));
	testObj3.getBody()->setVelocity(glm::vec2(5.0f, 1.f));
    //testObj.getBody()->m_orientation = glm::radians(45.f);
    //testObj3.getBody()->m_orientation = glm::radians(-45.f);

    //INITIALIZATION FOR NP_PHYSICS STUFF -------------------------------------------------------------------- END

	do{
		Renderer::do_movement();
		if (isEnterPressed == false)
		{
			if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
			{
				isEnterPressed = true;
				scene = (scene+1) % 4;		// SCENEJEN MÄÄRÄ?
				a = 0.0f;
			}
		}
		else
		{
			if (glfwGetKey(window, GLFW_KEY_ENTER) != GLFW_PRESS)
			{
				isEnterPressed = false;
			}
		}
		
		a += 0.02;
		//clear screen
		Renderer::initDraw();

		//--------------------draw--------------------
		if (scene == 3)
		{
			a += 0.2;
			if (a > PI * 2)
			{
				a -= PI * 2;
			}
			float add = a / PI / 6 - 0.3;
			float step = 1.0f / 3.0f;
			Renderer::setColor(0xFFA040FF);
			for (int i = 0; i < 3; i++)
			{
				Renderer::drawCircle(step*(i + 1) - add, 0, 0.1);
			}

			Renderer::setColor(0xFFFF00FF);
			Renderer::drawPie(0, 0, 0.5, 0.75 + 0.125 + sin(a) / 8, -PI / 4 * (sin(a) / 2 + 0.5) + PI / 4);

			//Renderer::setColor(0x000FFF);
			Renderer::setColor(0, 0, 1, 1);
			Renderer::drawLine(-1.0, 0.9, 1.0, 0.9, 10);
			Renderer::drawLine(-1.0, 0.8, 1.0, 0.8, 10);

			Renderer::drawLine(-1.0, -0.9, 1.0, -0.9, 10);
			Renderer::drawLine(-1.0, -0.8, 1.0, -0.8, 10);
		}
		else if (scene == 2)
		{
			Renderer::setColor(1.0, 0, 0, (sin(a) + 1) / 10);
			Renderer::drawPolygon(&c, 0, 0);

			c.setRotation(a);

			Renderer::setColor(0.0f, 0.0f, 0.0f, 1);

			for (int i = 0; i < 8; i++)
			{
				p.setRotation(a + i*3.14159265 / 4);
				Renderer::drawPolygon(&p, 0, 0);
			}
			p.setOrigin(-0.5, 0.0);
			for (int i = 0; i < 64; i++)
			{
				p.setOrigin(-0.5 - (float)i / 256, 0.0);
				p.setScale((float)(i + 1) / 16);
				p.setRotation(a + i*3.14159265 / 8);
				Renderer::drawPolygonTextured(&p, 0.0, 0.0, "testi");
			}
			int num = 64;
			float step = 3.14159265 * 2 / num;
			for (int i = 0; i < num; i++)
			{
				Renderer::drawPointSprite(cos((i*step + a) * 2 + PI / 2), sin(i*step + a), (sin(step*i - a) / 2.2 + 0.51) / 2, ps, cos(i*step + a * 2) / 2 + 0.5);
			}
		}
		else if (scene == 1)
		{
			//sprites
			Renderer::setColor(0x000040FF);
			Renderer::drawRectangle(-1, -1, 1, 1);

			spriteTimer += 1;
			if (spriteTimer > 1)
			{
				spriteNumber++;
				spriteTimer = 0;
			}
			for (int i = 0; i < 50; i++)
			{
				float aX = aa.getSprite(spriteNumber % 30).x;
				float aY = aa.getSprite(spriteNumber % 30).y;
				Renderer::drawSprite(&aa, rain[i].x, rain[i].y, aX, aY, "sheet", 1);
				rain[i].y -= 0.03 + sin(rain[i].x * cos(a) * 4) / 120;
				if (rain[i].y < -1.0)
				{
					rain[i].y = 1.2;
				}
			}
		}
		else if (scene == 0)
		{
			

			Renderer::setColor(1, 1, 1, 0.5);;
;
            //if (glfwGetKey(window, GLFW_KEY_O))
            //testObj.getBody()->addForce(0, 9.81);

            //testObj2.getBody()->addForce(0, 9.81);

            
        


            
            
            //NP_Physics
            /*testObj.bindPolygon(&p3);
            testObj2.bindPolygon(&p2);
            testObj3.bindPolygon(&p4);*/
            
			// You can now add own velocity to different bodies
			//testObj2.getBody()->setVelocity(testObj2.getBody()->getVelocity() + glm::vec2(0.f, 1.f));
            //testObj3.getBody()->setVelocity(testObj3.getBody()->getVelocity() + glm::vec2(1.f, 0.f));
            

            //Renderer::drawPolygon(&poly1, testObj.getBody()->m_position.x, testObj.getBody()->m_position.y);
            /*Renderer::drawPolygon(&poly2, testObj2.getBody()->m_position.x, testObj2.getBody()->m_position.y);
            Renderer::drawPolygon(&poly3, testObj3.getBody()->m_position.x, testObj3.getBody()->m_position.y);*/

            Renderer::drawPolygon(&poly1, testObj.getBody()->m_collider.position.x, testObj.getBody()->m_collider.position.y);
            Renderer::drawPolygon(&poly2, testObj2.getBody()->m_collider.position.x, testObj2.getBody()->m_collider.position.y);
            Renderer::drawPolygon(&poly3, testObj3.getBody()->m_collider.position.x, testObj3.getBody()->m_collider.position.y);

            Renderer::drawCircle(testObj.getBody()->m_collider.corner[0].x, testObj.getBody()->m_collider.corner[0].y, 0.01f);
            Renderer::drawCircle(testObj.getBody()->m_collider.corner[1].x, testObj.getBody()->m_collider.corner[1].y, 0.01f);

            Renderer::drawCircle(testObj3.getBody()->m_collider.corner[0].x, testObj3.getBody()->m_collider.corner[0].y, 0.01f);
            Renderer::drawCircle(testObj3.getBody()->m_collider.corner[1].x, testObj3.getBody()->m_collider.corner[1].y, 0.01f);
            Renderer::drawCircle(testObj3.getBody()->m_collider.corner[2].x, testObj3.getBody()->m_collider.corner[2].y, 0.01f);
            Renderer::drawCircle(testObj3.getBody()->m_collider.corner[3].x, testObj3.getBody()->m_collider.corner[3].y, 0.01f);

            Renderer::drawCircle(testObj2.getBody()->m_collider.corner[0].x, testObj2.getBody()->m_collider.corner[0].y, 0.01f);
            Renderer::drawCircle(testObj2.getBody()->m_collider.corner[1].x, testObj2.getBody()->m_collider.corner[1].y, 0.01f);
            Renderer::drawCircle(testObj2.getBody()->m_collider.corner[2].x, testObj2.getBody()->m_collider.corner[2].y, 0.01f);
            Renderer::drawCircle(testObj2.getBody()->m_collider.corner[3].x, testObj2.getBody()->m_collider.corner[3].y, 0.01f);
            std::cout << testObj.getBody()->m_collider.corner[0].x << std::endl;

            world.update(a / 500);
		}
		//std::cout << spriteNumber % 30 << ": " << aX << ", " << aY << "\n";
		//Renderer::drawSprite(&aa, -0.5, 0.5, aX, aY, "sheet", 0.5);
		
		//--------------------end draw--------------------
		//swap buffers
		Renderer::render();
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	//Camera stuff
	/*computeMatrices();
	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix; */
	Renderer::uninitRender();
	glfwTerminate();
	return 0;
}