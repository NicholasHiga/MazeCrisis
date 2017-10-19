#include "EffectedModel.h"

#include "MeshManager.h"

using std::string;
using std::vector;

EffectedModel::EffectedModel(const string &name, vector<SingleMesh*> meshes)
{
	setName(name);
	this->meshes = meshes;
}

string 
EffectedModel::getName()
{
	return name;
}

void 
EffectedModel::setName(const string &name)
{
	this->name = name;
}

void 
EffectedModel::addExistingMesh(const string &meshName)
{
	meshes.push_back(MeshManager::getInstance()->get(meshName));
}

vector<SingleMesh*> 
EffectedModel::getMeshes()
{
	return meshes;
}