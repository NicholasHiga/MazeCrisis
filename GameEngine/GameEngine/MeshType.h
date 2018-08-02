#pragma once
#include <string>

enum MESH_TYPE
{
    UNINITIALIZED,
    EFFECTED_MODEL,
    SINGLE_MESH
};

class MeshType
{
public:
    virtual std::string className() const = 0;
    virtual ~MeshType() {};
};