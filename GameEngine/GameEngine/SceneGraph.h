#pragma once
#include <memory>
#include <functional>

class AbstractRenderer;
class GameObject;
class Renderable;
class Camera;
class SceneNode;

class SceneGraph
{
public:
	SceneGraph();

	std::shared_ptr<SceneNode> getRootSceneNode() const;

	void doSomethingEachElement(std::shared_ptr<SceneNode> firstNode,
		std::function<void(std::shared_ptr<SceneNode>)> func);
	void renderSceneGraph(AbstractRenderer &renderer, Camera &camera);
	void updateSceneGraph(double deltaTime);
	void appendChildNode(std::shared_ptr<SceneNode> node);
	bool removeChildNode(std::shared_ptr<SceneNode> node);
	bool removeChildNode(SceneNode *node);
	bool removeNodeWithRenderable(Renderable &renderable);
	void removeAllNodes();

private:
	std::shared_ptr<SceneNode> root;
};