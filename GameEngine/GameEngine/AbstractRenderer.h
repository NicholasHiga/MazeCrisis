#pragma once

#include <memory>

#include "MatrixStack.h"

class EffectedModel;
class BoundingBox;
class SceneGraph;
class SingleMesh;
class SceneNode;
class Camera;

enum PrimitiveType;

class AbstractRenderer
{
public:
	AbstractRenderer() { isWireframe = false; };

	/// renderPrimitive()
	/// Render a dot, line, triangle or quad which will always
	/// be drawn at a fixed location.
	///
	/// @param[in]      prim
	///                 Primitive to be drawn.
	///
	virtual void renderFixedPrimitve(PrimitiveType prim) = 0;

	virtual void renderSingleMesh(const SingleMesh &mesh,
		BoundingBox *boundingBox, std::shared_ptr<SceneNode> node,
		Camera &camera) = 0;

	virtual void renderEffectedModel(EffectedModel &model,
		BoundingBox *boundingBox, std::shared_ptr<SceneNode> node,
		Camera &camera) = 0;

	virtual void renderSceneGraph(SceneGraph &graph, Camera &cam) = 0;

	virtual void renderSceneNode(std::shared_ptr<SceneNode> node,
		Camera &camera) = 0;

	virtual void setWireframeMode(bool isOn) { isWireframe = isOn; }

	virtual bool getWireframeMode() const { return isWireframe; }

	virtual void setWindowSize(int width, int height,
		Camera *cam = nullptr) = 0;

	virtual void prepareToRender(Camera &camera) = 0;

	MatrixStack getMatrixStack() const { return matrixStack; }

	glm::vec2 getWindowSize() const { return glm::vec2(winWidth, winHeight); }

protected:
	bool isWireframe;
	int winHeight, winWidth;
	MatrixStack matrixStack;
};