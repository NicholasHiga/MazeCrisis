#pragma once

#include <memory>

#include "AbstractRenderer.h"

class Camera;
class Skybox;
class SceneNode;
class SceneGraph;
class SingleMesh;
class BoundingBox;
class EffectedModel;

enum PrimitiveType;

class OpenGLRenderer : public AbstractRenderer
{
public:
	/// getInstance()
	/// Singleton, get a single instance of OpenGLRenderer
	///
	static OpenGLRenderer* getInstance();

	/// renderPrimitive()
	/// Render a given primitive.
	///
	/// @param[in]      prim
	///                 Primitive to be rendered.
	///
	void renderFixedPrimitve(PrimitiveType prim);

	//void renderMesh(MeshType &meshType);

	void renderSingleMesh(const SingleMesh &mesh, BoundingBox *boundingBox,
		std::shared_ptr<SceneNode> node, Camera &camera);

	void renderEffectedModel(EffectedModel &model, BoundingBox *boundingBox,
		std::shared_ptr<SceneNode> node, Camera &camera);

	void renderSkybox(Skybox &skybox, std::shared_ptr<SceneNode> node);
	//void renderMesh(Mesh &mesh);

	//void renderMesh(PolyMesh &polymesh);

	void setWireframeMode(bool isOn);

	void renderSceneGraph(SceneGraph &graph, Camera &cam);

	void renderSceneNode(std::shared_ptr<SceneNode> node, Camera &camera);

	void setWindowSize(int width, int height, Camera *cam = 0);

	void prepareToRender(Camera &camera);

private:
	static std::unique_ptr<OpenGLRenderer> myInstance;

	OpenGLRenderer();
};