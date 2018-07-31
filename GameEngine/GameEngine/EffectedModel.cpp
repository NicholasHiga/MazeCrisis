#include "EffectedModel.h"

#include "MeshManager.h"

using std::string;
using std::vector;

EffectedModel::EffectedModel(const string &name, vector<SingleMesh> &&_meshes)
{
    setName(name);
    meshes = std::move(_meshes);
}

string 
EffectedModel::getName() const
{
    return name;
}

void 
EffectedModel::setName(const string &name)
{
    this->name = name;
}

/*void 
EffectedModel::addExistingMesh(const string &meshName)
{
    meshes->push_back(*MeshManager::getInstance()->get(meshName));
}*/

const vector<SingleMesh>* 
EffectedModel::getMeshes()
{
    return &meshes;
}