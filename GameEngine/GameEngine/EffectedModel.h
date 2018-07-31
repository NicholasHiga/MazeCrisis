#pragma once
#include <string>

#include "SingleMesh.h"

class EffectedModel : public MeshType
{
public:
    EffectedModel() {};
    EffectedModel(const EffectedModel &other) = delete;
    EffectedModel(EffectedModel &&other) = delete;
    EffectedModel(const std::string &name, std::vector<SingleMesh> &&_meshes);

    EffectedModel& operator=(EffectedModel &other) = delete;
    EffectedModel& operator=(EffectedModel &&other) = delete;

    std::string getName() const;
    void setName(const std::string &name);

    //void addExistingMesh(const std::string &meshName);
    const std::vector<SingleMesh>* getMeshes();

    std::string className() const { return "PolyMesh"; }

    //BoundingBox* getBoundingBox() { return boundingBox; }

protected:
    std::string name;
    std::vector<SingleMesh> meshes;
};