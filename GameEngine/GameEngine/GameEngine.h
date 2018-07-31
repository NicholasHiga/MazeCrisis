#pragma once

// Shouldn't have all the headers here.
// The initial thought for doing this was so the
// dev only needs to include "GameEngine.h" to include
// everything, but this includes unnecessary
// header files which the dev doesn't need.
#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Clock.h"
#include "Camera.h"
#include "RayQuery.h"
#include "LogManager.h"

#include "GameObject.h"
#include "SingleMesh.h"

#include "SceneNode.h"
#include "SceneGraph.h"

#include "MeshManager.h"
#include "AudioManager.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "MaterialManager.h"

#include "OpenGLRenderer.h"
#include "AbstractRenderer.h"

#include "ShaderProgram.h"
#include "ShaderVariable.h"
#include "ShaderProgramManager.h"

using std::unique_ptr;

class AbstractRenderer;

class GameEngine
{
public:
	GameEngine(AbstractRenderer &renderer, GLuint windowWidth, 
		GLuint windowHeight);
	~GameEngine();

	virtual void onStart();
	virtual void onEnd();
	virtual void preRender(double timeSinceLastFrame);
	virtual void update(double deltaTime);
	virtual void render();
	virtual void postRender();

	/// info()
	/// Prints the message to the log file if the priority is less than or
	/// equal to info.
	///
	/// @param[in]      message
	///                 Message to print.
	///
	virtual void info(const std::string &message);

	/// warn()
	/// Prints the message to the log file if the priority is less than or
	/// equal to warn.
	///
	/// @param[in]      message
	///                 Message to print.
	///
	virtual void warn(const std::string &message);

	/// trace()
	/// Prints the message to the log file if the priority is less than or
	/// equal to trace.
	///
	/// @param[in]      message
	///                 Message to print.
	///
	virtual void trace(const std::string &message);

	/// error()
	/// Prints the message to the log file if the priority is less than or
	/// equal to error.
	///
	/// @param[in]      message
	///                 Message to print.
	///
	virtual void error(const std::string &message);

	SceneGraph* getSceneGraph() const { return scene; }
	MeshManager* getMeshManager() const { return meshManager; }
	AbstractRenderer* getRenderer() const { return renderer; }
	ModelManager* getModelManager() const { return modelManager; }
	MaterialManager* getMaterialManager()const  { return materialManager; }
	ShaderProgramManager* getShaderProgramManager() const 
		{ return shaderManager; }
	Camera* getCamera()  const { return camera.get(); }

	void setCamera() { }

	static bool loadTexture(const std::string &path,
		const std::string &name, TextureType textureType);
	static bool loadMaterial(const std::string &materialName, 
		const std::string &shaderName, const std::string &diffuseTexture);
	MeshType* loadModel(const std::string &name,
		const std::string &path, int _index = 0);

	double numMilliseconds;

protected:
	SceneGraph *scene;
	MeshManager *meshManager;
	AbstractRenderer *renderer;
	ModelManager *modelManager;
	AudioManager *audioManager;
	MaterialManager *materialManager;
	ShaderProgramManager *shaderManager;

	std::unique_ptr<Camera> camera;
};