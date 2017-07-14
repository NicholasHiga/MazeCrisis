#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "GLFW/glfw3.h"

#include <string>
#include <vector>
#include <iterator>
#include <memory>
#include <iostream>

#include "GameEngine.h"
#include "VertexData.h"

using glm::vec3;
using std::vector;
using std::string;
using std::shared_ptr;
using std::unique_ptr;

GLFWwindow* window;
const GLushort WIDTH = 1280;
const GLushort HEIGHT = 720;
int lastMouseState = GLFW_RELEASE;

void window_size_callback(GLFWwindow* window, int width, int height);
static void mouseHandler(GLFWwindow* window, int button, int action, int mods);

class DemoApp : public GameEngine
{
public:
	DemoApp();
	~DemoApp();
	void update(double t);

	vector<Vertex> squareVertices, cubeOneVertices, cubeTwoVertices;

	vector<string> meshNames     = { "ColoredCube", "TexturedCube" };
	vector<string> modelNames	= { "RainbowCube", "YellowCube", "Floor", "WoodSlab", "Crate", "Grass" };
	vector<string> materialNames = { "RainbowCube", "YellowCube", "Floor", "WoodSlab", "Crate", "Grass" };
	vector<string> shaderNames   = { "ColoredCube1", "ColoredCube2", "TexturedCube" };

	vector<shared_ptr<SceneNode>> nodes;
	vector<shared_ptr<GameObject>> gameObjects;
	float ang1 = -30, ang2 = 30, ang3 = 0;
};

DemoApp::DemoApp() : GameEngine(*OpenGLRenderer::getInstance(), WIDTH, HEIGHT)
{
	ShaderManagerLoadStatus status;
	renderer = OpenGLRenderer::getInstance();
	shaderManager = ShaderProgramManager::getInstance();

	camera->setCameraPosition(vec3(0, 0, 3), false);
	camera->setSceneCenter(vec3(0, 0, 0), false);
	camera->setCameraUp(vec3(0, 1, 0), true);

	for (size_t i = 0; i < modelNames.size(); i++)
		nodes.push_back(std::make_shared<SceneNode>());

	nodes[0]->setPosition(vec3(-1.2f, -1.0f, -2.0f));
	nodes[0]->setScale(vec3(2.0f, 1.0f, 1.0f));
	//nodes[0]->setOrientation(Quaternion(ang1, 0, 0, false));

	nodes[1]->setPosition(vec3(1.2f, -1.0f, -2.0f));
	nodes[1]->setScale(vec3(2.0f, 1.0f, 1.0f));
	//nodes[1]->setOrientation(Quaternion(ang2, 0, 0, false));

	nodes[2]->setPosition(vec3(-1.3f, 1.0f, -0.5f));
	nodes[3]->setPosition(vec3(0.0f, 1.0f, -0.5f));
	nodes[4]->setPosition(vec3(1.3f, 1.0f, -0.5f));
	nodes[5]->setPosition(vec3(0.0f, -0.0f, -3.0f));
	nodes[5]->setOrientation(Quaternion(0, 0, ang3, false));

	vector<ShaderVariable> vars;
	vars.push_back(ShaderVariable(ENGINE_VAR::VERTEX_POSITION, "position"));
	vars.push_back(ShaderVariable(ENGINE_VAR::VERTEX_COLOR, "color"));
	vars.push_back(ShaderVariable(ENGINE_VAR::MODEL_VIEW_MATRIX, "modelViewMatrix"));
	vars.push_back(ShaderVariable(ENGINE_VAR::PROJECTION_MATRIX, "projectionMatrix"));

	vector<ShaderVariable> vars2;
	vars2.push_back(ShaderVariable(ENGINE_VAR::VERTEX_POSITION, "position"));
	vars2.push_back(ShaderVariable(ENGINE_VAR::VERTEX_UV, "uv"));
	vars2.push_back(ShaderVariable(ENGINE_VAR::SAMPLER_2D, "textureIn"));
	vars2.push_back(ShaderVariable(ENGINE_VAR::MODEL_VIEW_MATRIX, "modelViewMatrix"));
	vars2.push_back(ShaderVariable(ENGINE_VAR::PROJECTION_MATRIX, "projectionMatrix"));

	for (size_t i = 0; i < coloredCubePositions.size() / 3; i++)
	{
		Vertex v;
		v.position = vec3(coloredCubePositions[i * 3], coloredCubePositions[i * 3 + 1], coloredCubePositions[i * 3 + 2]);
		v.color = vec3(coloredCubeColors[i * 3], coloredCubeColors[i * 3 + 1], coloredCubeColors[i * 3 + 2]);
		cubeOneVertices.push_back(v);
	}

	for (size_t i = 0; i < texturedCubeVertices.size() / 3; i++)
	{
		Vertex v;
		v.position = vec3(texturedCubeVertices[i * 3], texturedCubeVertices[i * 3 + 1], texturedCubeVertices[i * 3 + 2]);
		v.uv = glm::vec2(texturedCubeUVs[i * 2], texturedCubeUVs[i * 2 + 1]);
		cubeTwoVertices.push_back(v);
	}

	// Testing various constructors.
	// NOTE: Need different meshes for a mesh with a different shader.
	try
	{
		/*// Cube one, bottom left
		meshManager->loadMesh(meshNames[0], PrimitiveType::PRIM_TRIANGLE, cubeOneVertices, coloredCubeIndices);
		materialManager->loadMaterial(materialNames[0], shaderNames[0], "ColoredVertex.vert",
			"ColoredFragment.frag", vars, "", true);
	    modelManager->loadModel(modelNames[0], meshNames[0], materialNames[0]);	// Need to make model manager which handles creation of polymodels, EffectedModels
																//// PolyModels = vector of EffectedModels
		// Cube two, bottom right
		modelManager->loadModel(modelNames[1], meshNames[0], materialNames[1], shaderNames[1], "ColoredVertexTwo.vert",
			"ColoredFragmentTwo.frag", vars, "", true);

		// Cube three, top left
		modelManager->loadModel(modelNames[2], meshNames[1], materialNames[2], shaderNames[2],
			PrimitiveType::PRIM_TRIANGLE, cubeTwoVertices, texturedCubeIndices, "TexturedVertex.vert",
			"TexturedFragment.frag", vars2, "floor.png", true);
			
		// Cube four, top center
		materialManager->loadMaterial(materialNames[3], shaderNames[2], "woodCrate.jpg");
		modelManager->loadModel(modelNames[3], meshNames[1], materialNames[3]);

		// Cube five, top right
		modelManager->loadModel(modelNames[4], meshNames[1], materialNames[4], shaderNames[2], "crate.bmp");
		
		// Cube six, bottom center
		modelManager->loadCubeModel(modelNames[5], meshNames[1], materialNames[5], shaderNames[2], "grass.png");

		//ren->setWireframeMode(true);
		for (size_t i = 0; i < modelNames.size(); i++)
			gameObjects.push_back(std::make_shared<GameObject>(modelNames[i]));

		for (size_t i = 0; i < nodes.size(); i++)
		{
			nodes[i]->addRenderable(*gameObjects[i]);
			scene->appendChildNode(nodes[i]);
		}*/
		modelManager->loadCubeModel("Test", "floor.png");
		gameObjects.push_back(std::make_shared<GameObject>("Test"));
		nodes[0]->addRenderable(*gameObjects[0]);
		scene->appendChildNode(nodes[0]);
	
		modelManager->loadCubeModel("Test2", "grass.png");
		gameObjects.push_back(std::make_shared<GameObject>("Test2"));
		nodes[1]->addRenderable(*gameObjects[1]);
		scene->appendChildNode(nodes[1]);
	}
	catch (std::exception &e)
	{
		std::cout << e.what();
	}
}

void
DemoApp::update(double t)
{
	ang1 += (float)(t * 0.001f);
	ang2 -= (float)(t * 0.001f);
	ang3 += (float)(t * 0.001f);
	//nodes[0]->setOrientation(Quaternion(ang1, 0, 0, false));
	//nodes[1]->setOrientation(Quaternion(ang2, 0, 0, false));
	vec3 increment = vec3(0.0001f * t, 0, 0);
	nodes[0]->setPosition(nodes[0]->getPosition() + increment);
	nodes[1]->setPosition(nodes[1]->getPosition() - increment);

	BoundingBox *bb1, *bb2;

	vector<Renderable*> *renderables = nodes[0]->getRenderables();
	vector<Renderable*> *renderable2 = nodes[1]->getRenderables();

	GameObject *go = dynamic_cast<GameObject*>((*renderables)[0]);
	bb1 = go->getBoundingBoxes()[0];

	GameObject *go2 = dynamic_cast<GameObject*>((*renderable2)[0]);
	bb2= go2->getBoundingBoxes()[0];

	if (BoundingBox::doesCollide(*bb1, *bb2))
		std::cout << go->getMaterialName() << " " 
		<< go2->getMaterialName() << std::endl;	

	nodes[5]->setOrientation(Quaternion(0, ang3, ang3, false));
}

DemoApp::~DemoApp()
{
}

DemoApp *engine;

void init()
{
	engine = new DemoApp();
	engine->onStart();
}

int main()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(WIDTH, HEIGHT, "Maze", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. \n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, window_size_callback);

	glewExperimental = GL_TRUE;
	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Clearing error queue, because the above causes GL_INVALID_ENUM even when
	// there is no problem.
	while (glGetError() != GL_NO_ERROR) {};
	
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetMouseButtonCallback(window, mouseHandler);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	init();
	engine->getRenderer()->setWindowSize(WIDTH, HEIGHT, engine->getCamera());

	do {
		//ren->render();
		double deltaTime = Clock::getMilliseconds() - engine->numMilliseconds;
		engine->preRender(deltaTime);
		engine->numMilliseconds = Clock::getMilliseconds();

		engine->update(deltaTime);
		engine->render();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
		engine->postRender();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Close OpenGL window and terminate GLFW, end engine.
	engine->onEnd();
	glfwTerminate();

	return 0;
}

void 
window_size_callback(GLFWwindow* window, int width, int height)
{
	engine->getRenderer()->setWindowSize(width, height, engine->getCamera());
}

void 
mouseHandler(GLFWwindow* window, int button, int action, int mods)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS
		&& lastMouseState == GLFW_RELEASE)
	{
		Ray r = RayQuery::getRayFromMouseClick((int)xpos, (int)ypos);

		vector<shared_ptr<SceneNode>> intersections =
			RayQuery::Raycast(r, true);

		if (!intersections.empty())
		{
			GameObject* go = dynamic_cast<GameObject*>((*intersections[0]->getRenderables())[0]);

			if (go)
			{
				std::cout << go->getMaterialName() << std::endl;
				//engine->getSceneGraph()->removeChildNode(intersections[0]);
			}
		}

		lastMouseState = GLFW_PRESS;
	}

	if (action == GLFW_RELEASE)
		lastMouseState = GLFW_RELEASE;
}