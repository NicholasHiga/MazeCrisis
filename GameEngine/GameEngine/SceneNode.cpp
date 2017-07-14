#include "SceneNode.h"
#include "GameObject.h"
#include <glm/gtc/matrix_transform.hpp>

using glm::vec3;
using glm::mat4;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;

SceneNode::SceneNode()
{
	position = vec3();
	orientation = Quaternion();
	scale = vec3(1.0f, 1.0f, 1.0f);
}

SceneNode::SceneNode(vec3 _position) : position(_position)
{
	scale = vec3(1.0f, 1.0f, 1.0f);
	orientation = Quaternion();
}

SceneNode::SceneNode(vec3 _position, Quaternion _orientation, vec3 _scale = vec3(1.0f, 1.0f, 1.0f))
	: position(_position), orientation(_orientation), scale(_scale)
{
}

void 
SceneNode::appendChildNode(shared_ptr<SceneNode> node)
{
	children.push_back(node);
	node.get()->parent = shared_from_this();
}

bool
SceneNode::removeChildNode(shared_ptr<SceneNode> node)
{
	vector<shared_ptr<SceneNode>>::iterator it;
	for (it = children.begin(); it != children.end(); it++)
	{
		if ((*it).get() == node.get())
		{
			children.erase(it);
			return true;
		}
	}
	return false;
}

bool
SceneNode::removeChildNode(SceneNode *node)
{
	vector<shared_ptr<SceneNode>>::iterator it;
	for (it = children.begin(); it != children.end(); it++)
	{
		if ((*it).get() == node)
		{
			children.erase(it);
			return true;
		}
	}
	return false;
}

void 
SceneNode::addRenderable(Renderable &renderable)
{
	renderables.push_back(&renderable);
	if (GameObject* t = dynamic_cast<GameObject*>(&renderable))
		t->setParentNode(shared_from_this());
}

bool
SceneNode::removeRenderable(Renderable &renderable)
{
	auto it = std::find(renderables.begin(), renderables.end(), &renderable);
	if (it != renderables.end())
	{
		int index = it - renderables.begin();
		renderables.erase(it);
		return true;
	}
	return false;
}

void 
SceneNode::setPosition(vec3 &pos)
{
	position = pos;
	updateBoundingBoxes();
}

void 
SceneNode::setOrientation(Quaternion &quat)
{
	orientation = quat;
	updateBoundingBoxes();
}

void 
SceneNode::setScale(vec3 &_scale)
{
	scale = _scale;
	updateBoundingBoxes();
}

vec3
SceneNode::getPosition()
{
	return position;
}

Quaternion 
SceneNode::getOrientation()
{
	return orientation;
}

vec3
SceneNode::getScale()
{
	return scale;
}

vector<Renderable*> *
SceneNode::getRenderables()
{
	return &renderables;
}

mat4
SceneNode::getModelMatrix()
{
	float angle;
	vec3 rotAxis;

	getOrientation().toRotationAndVectorDegrees(angle, rotAxis);

	mat4 modelM;
	modelM = glm::translate(modelM, vec3(position.x, position.y, position.z));
	if (angle)
		modelM = glm::rotate(modelM, angle, rotAxis);
	modelM = glm::scale(modelM, vec3(scale.x, scale.y, scale.z));
	return modelM;
}

void
SceneNode::updateBoundingBoxes()
{
	for (size_t i = 0; i < renderables.size(); i++)
	{
		if (GameObject* go = dynamic_cast<GameObject*>(renderables[i]))
		{
			std::vector<BoundingBox*> boundingBoxes
				= go->getBoundingBoxes();
			for (size_t j = 0; j < boundingBoxes.size(); j++)
				boundingBoxes[j]->updateBounds(getModelMatrix());
		}
	}
}