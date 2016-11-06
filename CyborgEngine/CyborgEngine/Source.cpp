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
    NP_Object testObj, testObj2, testObj3, testObj4, testObj5, testObj6;
    // Creating bodies to the world
    NP_Body testBody(world, 1.f), testBody2(world, 1.f), testBody3(world, 1.f), testBody4(world, 0.5f), testBody5(world, 0.5f), testBody6(world, 100.f);

    //testBody3.m_mass = 10.f;
    


    // Binding objects to bodies
    testObj.bindBody(&testBody);
    testObj2.bindBody(&testBody2);
    testObj3.bindBody(&testBody3);
    testObj4.bindBody(&testBody4);
    testObj5.bindBody(&testBody5);
    testObj6.bindBody(&testBody6);


    testBody.addBoxCollider(0.5f);
    testBody2.addBoxCollider(0.5f);
    testBody3.addBoxCollider(0.5f);  
    testBody4.addBoxCollider(.25f);
    testBody5.addBoxCollider(.25f);
    testBody6.addBoxCollider(0.5f);

    Polygon poly1, poly2, poly3, poly4, poly5, poly6;
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
        glm::vec2(-0.125, 0.125),
        glm::vec2(-0.125, -0.125),
        glm::vec2(0.125, -0.125),
        glm::vec2(0.125, 0.125)
    };
    poly1.setPoints(pointsp, 4);
    poly2.setPoints(pointsp, 4);
    poly3.setPoints(pointsp, 4);
    poly4.setPoints(pointsp123, 4);
    poly5.setPoints(pointsp123, 4);
    poly6.setPoints(pointsp, 4);



    testObj.bindPolygon(&poly1);
    testObj2.bindPolygon(&poly2);
    testObj3.bindPolygon(&poly3);
    testObj4.bindPolygon(&poly4);
    testObj5.bindPolygon(&poly5);
    testObj6.bindPolygon(&poly6);

	// Adding objects to the wold
    
    world.addObject(&testObj);
	world.addObject(&testObj2);
    world.addObject(&testObj3);
  //  world.addObject(&testObj4);
  //  world.addObject(&testObj5);
  //  world.addObject(&testObj6);
    

    testObj3.getBody()->setPos(glm::vec2(-1.f, 0.4f));
    testObj2.getBody()->setPos(glm::vec2(0.0f, 0.75f));
    testObj.getBody()->setPos(glm::vec2(1.0f, 0.f));

    testObj4.getBody()->setPos(glm::vec2(-0.35f, -0.2f));
    testObj5.getBody()->setPos(glm::vec2(0.35f, -0.2f));
    testObj6.getBody()->setPos(glm::vec2(0.0f, 1.f));
    
    testObj.getBody()->setVelocity(glm::vec2(-25.0f, 1.f));
	testObj3.getBody()->setVelocity(glm::vec2(5.0f, 1.f));
    testObj2.getBody()->setVelocity(glm::vec2(0.f, -5.f));

    testObj4.getBody()->setVelocity(glm::vec2(9.0f, 3.0f));
    testObj5.getBody()->setVelocity(glm::vec2(-9.0f, 3.0f));
    testObj6.getBody()->setVelocity(glm::vec2(0.f, -5.f));
    //testObj.getBody()->m_orientation = glm::radians(30.f);
    //testObj3.getBody()->m_orientation = glm::radians(-45.f);
    //testObj3.getBody()->m_angularVelocity = 10.0f;
    //testObj2.getBody()->m_angularVelocity = 2.5f;
    //testObj.getBody()->m_angularVelocity = -2.f;
    //testObj4.getBody()->m_angularVelocity = -10.f;
    //testObj5.getBody()->m_angularVelocity = 10.f;

    //INITIALIZATION FOR NP_PHYSICS STUFF -------------------------------------------------------------------- END

	do{
		Renderer::do_movement();
		if (isEnterPressed == false)
		{
			if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
			{
				isEnterPressed = true;
				scene = (scene+1) % 2;		// SCENEJEN MÄÄRÄ
				a = 0.0f;
                
                if (scene == 0)
                {
                    world.removeObjects();

                    world.addObject(&testObj);
                    world.addObject(&testObj2);
                    world.addObject(&testObj3);
                }
                else if (scene == 1)

                {
                    world.removeObjects();

                    world.addObject(&testObj4);
                    world.addObject(&testObj5);
                    world.addObject(&testObj6);
                }

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

		if (scene == 0)
		{     

			Renderer::setColor(1, 1, 1, 0.5);

            Renderer::drawPolygon(&poly1, testObj.getBody()->m_collider.position.x, testObj.getBody()->m_collider.position.y);
            Renderer::drawPolygon(&poly2, testObj2.getBody()->m_collider.position.x, testObj2.getBody()->m_collider.position.y);
            Renderer::drawPolygon(&poly3, testObj3.getBody()->m_collider.position.x, testObj3.getBody()->m_collider.position.y);

            Renderer::drawCircle(testObj.getBody()->m_collider.corner[0].x, testObj.getBody()->m_collider.corner[0].y, 0.01f);
            Renderer::drawCircle(testObj.getBody()->m_collider.corner[1].x, testObj.getBody()->m_collider.corner[1].y, 0.01f);
            Renderer::drawCircle(testObj.getBody()->m_collider.corner[2].x, testObj.getBody()->m_collider.corner[2].y, 0.01f);
            Renderer::drawCircle(testObj.getBody()->m_collider.corner[3].x, testObj.getBody()->m_collider.corner[3].y, 0.01f);

            Renderer::drawCircle(testObj3.getBody()->m_collider.corner[0].x, testObj3.getBody()->m_collider.corner[0].y, 0.01f);
            Renderer::drawCircle(testObj3.getBody()->m_collider.corner[1].x, testObj3.getBody()->m_collider.corner[1].y, 0.01f);
            Renderer::drawCircle(testObj3.getBody()->m_collider.corner[2].x, testObj3.getBody()->m_collider.corner[2].y, 0.01f);
            Renderer::drawCircle(testObj3.getBody()->m_collider.corner[3].x, testObj3.getBody()->m_collider.corner[3].y, 0.01f);

            Renderer::drawCircle(testObj2.getBody()->m_collider.corner[0].x, testObj2.getBody()->m_collider.corner[0].y, 0.01f);
            Renderer::drawCircle(testObj2.getBody()->m_collider.corner[1].x, testObj2.getBody()->m_collider.corner[1].y, 0.01f);
            Renderer::drawCircle(testObj2.getBody()->m_collider.corner[2].x, testObj2.getBody()->m_collider.corner[2].y, 0.01f);
            Renderer::drawCircle(testObj2.getBody()->m_collider.corner[3].x, testObj2.getBody()->m_collider.corner[3].y, 0.01f);
            //std::cout << testObj.getBody()->m_collider.corner[0].x << std::endl;

			//testObj.getBody()->setOrient(a);

            world.update(a / 10000);
		}
        else if (scene == 1)
        {
            Renderer::setColor(1, 0.5, 0.75, 0.5);

            Renderer::drawPolygon(&poly4, testObj4.getBody()->m_collider.position.x, testObj4.getBody()->m_collider.position.y);
            Renderer::drawPolygon(&poly5, testObj5.getBody()->m_collider.position.x, testObj5.getBody()->m_collider.position.y);
            Renderer::drawPolygon(&poly6, testObj6.getBody()->m_collider.position.x, testObj6.getBody()->m_collider.position.y);

            Renderer::drawCircle(testObj4.getBody()->m_collider.corner[0].x, testObj4.getBody()->m_collider.corner[0].y, 0.01f);
            Renderer::drawCircle(testObj4.getBody()->m_collider.corner[1].x, testObj4.getBody()->m_collider.corner[1].y, 0.01f);
            Renderer::drawCircle(testObj4.getBody()->m_collider.corner[2].x, testObj4.getBody()->m_collider.corner[2].y, 0.01f);
            Renderer::drawCircle(testObj4.getBody()->m_collider.corner[3].x, testObj4.getBody()->m_collider.corner[3].y, 0.01f);

            Renderer::drawCircle(testObj5.getBody()->m_collider.corner[0].x, testObj5.getBody()->m_collider.corner[0].y, 0.01f);
            Renderer::drawCircle(testObj5.getBody()->m_collider.corner[1].x, testObj5.getBody()->m_collider.corner[1].y, 0.01f);
            Renderer::drawCircle(testObj5.getBody()->m_collider.corner[2].x, testObj5.getBody()->m_collider.corner[2].y, 0.01f);
            Renderer::drawCircle(testObj5.getBody()->m_collider.corner[3].x, testObj5.getBody()->m_collider.corner[3].y, 0.01f);

            world.update(a / 5000);
        }


		//--------------------end draw--------------------
		//swap buffers
		Renderer::render();
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	Renderer::uninitRender();
	glfwTerminate();
	return 0;
}