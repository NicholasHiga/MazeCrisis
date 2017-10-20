#pragma once
#include <string>

#include "SingleMesh.h"

class EffectedModel : public MeshType
{
public:
	EffectedModel() {};
	EffectedModel(const std::string &name, std::vector<SingleMesh*> meshes);

	std::string getName() const;
	void setName(const std::string &name);

	void addExistingMesh(const std::string &meshName);
	std::vector<SingleMesh*> getMeshes() const;

	std::string className() const { return "PolyMesh"; }

	//BoundingBox* getBoundingBox() { return boundingBox; }

protected:
	std::string name;
	std::vector<SingleMesh*> meshes;
};