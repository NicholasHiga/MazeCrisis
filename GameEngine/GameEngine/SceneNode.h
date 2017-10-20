#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "Quaternion.h"
#include "BoundingBox.h"

class Renderable;
class GameObject;

class SceneNode : public std::enable_shared_from_this<SceneNode>
{
public:
	SceneNode();
	// SceneNode(int _value) : value(_value) {}; Used in debugging.
	SceneNode(glm::vec3 _position);
	SceneNode(glm::vec3 _position, Quaternion _orientation, glm::vec3 _scale);
	SceneNode(SceneNode const &) = delete;
	void operator=(SceneNode const &x) = delete;

	void appendChildNode(std::shared_ptr<SceneNode> node);
	bool removeChildNode(std::shared_ptr<SceneNode> node);
	bool removeChildNode(SceneNode *node);
	std::vector<std::shared_ptr<SceneNode>> *getChildren() 
		{ return &children; };

	void addRenderable(Renderable &renderable);
	bool removeRenderable(Renderable &renderable);

	/*void addGameObject(unique_ptr<GameObject> obj);
	bool removeGameObject(unique_ptr<GameObject> obj);*/

	void setPosition(glm::vec3 &pos);
	void setOrientation(Quaternion &quat);
	void setScale(glm::vec3 &_scale);

	glm::vec3 getPosition() const;
	Quaternion getOrientation() const;
	glm::vec3 getScale() const;
	std::vector<Renderable*> *getRenderables();
	//std::vector<std::unique_ptr<BoundingBox>> *getBoundingBoxes();
	SceneNode* getParentNode() const { return parent.get(); }

	glm::mat4 getModelMatrix() const;

	bool isBeingRendered = true;

	// For debugging;
	// int value; 

private:
	void updateBoundingBoxes();

	std::vector<std::shared_ptr<SceneNode>> children;
	std::shared_ptr<SceneNode> parent;

	glm::vec3 position;
	glm::vec3 scale;
	Quaternion orientation;
	std::vector<Renderable*> renderables;
	//std::vector<std::unique_ptr<BoundingBox>> boundingBoxes;
};