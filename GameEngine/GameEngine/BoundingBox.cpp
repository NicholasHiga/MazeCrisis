#include "BoundingBox.h"
#include "SingleMesh.h"
#include "SceneGraph.h"
#include "GameObject.h"
#include "SceneNode.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using std::vector;
using std::shared_ptr;
using std::unique_ptr;

BoundingBox::BoundingBox(const SingleMesh &mesh, BOUNDING_BOX_TYPE bbType, 
	bool isVisible)
{
	calculateBaseBounds(mesh, bbType);
	this->bbType = bbType;
	setIsVisible(isVisible);
}

void
BoundingBox::updateBounds(mat4 &transform)
{
	if (bbType == BOUNDING_BOX_TYPE::OBB
		|| bbType == BOUNDING_BOX_TYPE::AABB)
	{
		vec4 t = transform * vec4(baseMins, 1.0f);
		aabbMins = vec3(t.x, t.y, t.z);
		t = transform * vec4(baseMaxes, 1.0f);
		aabbMaxes = vec3(t.x, t.y, t.z);

		if (aabbMins.x > aabbMaxes.x)
		{
			float w = aabbMins.x;
			aabbMins.x = aabbMaxes.x;
			aabbMaxes.x = w;
		}

		if (aabbMins.y > aabbMaxes.y)
		{
			float w = aabbMins.y;
			aabbMins.y = aabbMaxes.y;
			aabbMaxes.y = w;
		}

		if (aabbMins.z > aabbMaxes.z)
		{
			float w = aabbMins.z;
			aabbMins.z = aabbMaxes.z;
			aabbMaxes.z = w;
		}

		center.x = (aabbMins.x + aabbMaxes.x) / 2.0f;
		center.y = (aabbMins.y + aabbMaxes.y) / 2.0f;
		center.z = (aabbMins.z + aabbMaxes.z) / 2.0f;
	}
	else
	{
		vec4 t = transform * vec4(center, 1.0f);
		center = vec3(t.x, t.y, t.z);
		// TODO Change radius when scale changes.
		// Can be done by using min/max corners difference.
	}
}

BOUNDING_BOX_TYPE 
BoundingBox::getBoundingBoxType() const
{
	return bbType;
}

bool 
BoundingBox::doesCollide(const BoundingBox &bb) const
{
	return BoundingBox::doesCollide(*this, bb);
}

bool
BoundingBox::doesCollide(const BoundingBox &bb1, const BoundingBox &bb2)
{
	return (bb1.aabbMins.x <= bb2.aabbMaxes.x 
		&& bb1.aabbMaxes.x >= bb2.aabbMins.x) &&
		(bb1.aabbMins.y <= bb2.aabbMaxes.y 
			&& bb1.aabbMaxes.y >= bb2.aabbMins.y) &&
		(bb1.aabbMins.z <= bb2.aabbMaxes.z
			&& bb1.aabbMaxes.z >= bb2.aabbMins.z);
}

std::vector<std::shared_ptr<SceneNode>> 
BoundingBox::doesCollide(SceneGraph *graph)
{
	return BoundingBox::doesCollide(graph, *this);
}

// TODO:: Test
std::vector<std::shared_ptr<SceneNode>>
BoundingBox::doesCollide(SceneGraph *graph, const BoundingBox &bb)
{
	vector<shared_ptr<SceneNode>> intersections;
	graph->doSomethingEachElement(graph->getRootSceneNode(),
		[=, &intersections](shared_ptr<SceneNode> n)
	{
		vector<Renderable*> *renderables = n.get()->getRenderables();
		for (size_t i = 0; i < (*renderables).size(); ++i)
		{
			if (GameObject* go = dynamic_cast<GameObject*>((*renderables)[i]))
			{
				if (go->getIsEnabled())
				{
					vector<BoundingBox*> bbs = go->getBoundingBoxes();
					for (size_t j = 0; j < go->getBoundingBoxes().size(); ++j)
					{
						if (doesCollide(bb, *bbs[j]))
						{
							intersections.push_back(n);
							break;
						}
					}
				}
			}
		}
	});
	return intersections;
}

std::vector<std::shared_ptr<SceneNode>> 
BoundingBox::doesCollide(std::vector<std::shared_ptr<SceneNode>> &nodes,
	std::vector<unsigned int> *indices)
{
	return BoundingBox::doesCollide(nodes, *this, indices);
}

std::vector<std::shared_ptr<SceneNode>>
BoundingBox::doesCollide(std::vector<std::shared_ptr<SceneNode>> &nodes, 
	const BoundingBox &bb, std::vector<unsigned int> *indices)
{
	vector<shared_ptr<SceneNode>> intersections;
	for (size_t z = 0; z < nodes.size(); ++z)
	{
		vector<Renderable*> *renderables = nodes[z].get()->getRenderables();
		for (size_t i = 0; i < (*renderables).size(); ++i)
		{
			if (GameObject* go = dynamic_cast<GameObject*>((*renderables)[i]))
			{
				if (go->getIsEnabled())
				{
					vector<BoundingBox*> bbs = go->getBoundingBoxes();
					for (size_t j = 0; j < bbs.size(); ++j)
					{
						if (doesCollide(bb, *bbs[j]))
						{
							if (indices)
								indices->push_back(z);
							intersections.push_back(nodes[z]);
							break;
						}
					}
				}
			}
		}
	}
	return intersections;
}

bool 
BoundingBox::getIsVisible() const
{
	return isVisible;
}

void 
BoundingBox::setIsVisible(bool isVisible)
{
	this->isVisible = isVisible;
}

void
BoundingBox::calculateBaseBounds(const SingleMesh &mesh, BOUNDING_BOX_TYPE bbType)
{
	vector<Vertex> verts = mesh.getVertices();
	//int numVerts = mesh.getUniqueVertices(&verts);

	vec3 pos = verts[0].position;
	baseMins = vec3(pos.x, pos.y, pos.z);
	baseMaxes = vec3(pos.x, pos.y, pos.z);

	for (size_t i = 1; i < verts.size(); ++i)
	{
		if (verts[i].position.x > baseMaxes.x)
			baseMaxes.x = verts[i].position.x;

		if (verts[i].position.x < baseMins.x)
			baseMins.x = verts[i].position.x;

		if (verts[i].position.y > baseMaxes.y)
			baseMaxes.y = verts[i].position.y;

		if (verts[i].position.y < baseMins.y)
			baseMins.y = verts[i].position.y;

		if (verts[i].position.z > baseMins.z)
			baseMaxes.z = verts[i].position.z;

		if (verts[i].position.z < baseMins.z)
			baseMins.z = verts[i].position.z;
	}

	aabbMins = baseMins;
	aabbMaxes = baseMaxes;

	if (bbType == BOUNDING_BOX_TYPE::SPHERE)
	{
		radius = aabbMaxes.x - aabbMins.x;

		if (aabbMaxes.y - aabbMins.y > radius)
			radius = aabbMaxes.y - aabbMins.y;

		if (aabbMaxes.z - aabbMins.z > radius)
			radius = aabbMaxes.z - aabbMins.z;

		center.x = (aabbMins.x + aabbMaxes.x) / 2.0f;
		center.y = (aabbMins.y + aabbMaxes.y) / 2.0f;
		center.z = (aabbMins.z + aabbMaxes.z) / 2.0f;
	}
}