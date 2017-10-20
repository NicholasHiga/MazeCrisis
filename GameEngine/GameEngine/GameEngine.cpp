#include "GameEngine.h"

using std::string;

GameEngine::GameEngine(AbstractRenderer &renderer, GLuint windowWidth,
	GLuint windowHeight)
{
	shaderManager = ShaderProgramManager::getInstance();
	materialManager = MaterialManager::getInstance();
	modelManager = ModelManager::getInstance();
	meshManager = MeshManager::getInstance();
	audioManager = AudioManager::getInstance();
	this->renderer = &renderer;
	camera = std::make_unique<Camera>(windowWidth, windowHeight);
	scene = new SceneGraph();
	RayQuery::setSceneGraph(*scene);
	RayQuery::setRenderer(renderer);
}

GameEngine::~GameEngine()
{
	delete scene;
}

void 
GameEngine::onStart()
{
	numMilliseconds = Clock::getMilliseconds();
}

void 
GameEngine::onEnd()
{

}

void 
GameEngine::preRender(double timeSinceLastFrame)
{

}

void
GameEngine::render()
{
	scene->renderSceneGraph(*renderer, *camera);
}

void 
GameEngine::update(double deltaTime)
{
	scene->updateSceneGraph(deltaTime);
	audioManager->update();
}

void 
GameEngine::postRender()
{

}

void 
GameEngine::info(const string &message)
{
	LogManager::getInstance().info(message);
}

void
GameEngine::trace(const string &message)
{
	LogManager::getInstance().trace(message);
}

void
GameEngine::warn(const string &message)
{
	LogManager::getInstance().warn(message);
}

void
GameEngine::error(const string &message)
{
	LogManager::getInstance().error(message);
}

bool
GameEngine::loadTexture(const string &path, const string &name)
{
	return TextureManager::getInstance()->loadTexture(path, name);
}

bool 
GameEngine::loadMaterial(const string &materialName, const string &shaderName,
	const string &diffuseTexture)
{
	return MaterialManager::getInstance()->loadMaterial(materialName, 
		shaderName, diffuseTexture);
}

MeshType* 
GameEngine::loadModel(const string &name, const string &path, int _index)
{
	//return ModelManager::getInstance()->loadModel(path, name,
	//*ImageManager::getInstance(), _index);
	return nullptr;
}