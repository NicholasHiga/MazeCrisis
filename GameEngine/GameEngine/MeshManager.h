#pragma once
#include <vector>
#include <string>
#include <memory>

#include "SingleMesh.h"
#include "BoundingBox.h"
#include "ResourceManager.h"

class MeshManager : public ResourceManager<SingleMesh>
{
public:
    static MeshManager* getInstance();

    // Doesn't initialize mesh, shader or material, assume all are loaded
    // already.
    bool loadModel(const std::string &modelName,
        const std::string &baseMeshName,
        const std::string &materialName);

    // Doesn't initialize mesh or shader, but initializes the material using
    // the given shader and texture.
    bool loadModel(const std::string &modelName, 
        const std::string &baseMeshName, const std::string &materialName,
        const std::string &shaderName,
        const std::string &diffuseTexture = "");

    // Initialize mesh, but not material or shader.
    bool loadModel(const std::string &modelName,
        const std::string &materialName, PrimitiveType prim,
        const std::vector<Vertex> &vertices,
        const std::vector<GLuint> &indices);

    // Initialize material and shader, but not mesh.
    bool loadModel(const std::string &modelName,
        const std::string &baseMeshName, const std::string& materialName,
        const std::string &shaderName, const std::string &vertexShaderPath,
        const std::string &fragmentShaderPath,
        const std::vector<ShaderVariable> &shaderVars,
        const std::string &diffuseTexture = "",
        bool printShaderLoadStatus = false);

    // Initialization of everything, mesh, material and shader.
    bool loadModel(const std::string &modelName,
        const std::string &materialName, const std::string &shaderName,
        PrimitiveType prim, const std::vector<Vertex> &vertices,
        const std::vector<GLuint> &indices, const std::string &vertexShaderPath,
        const std::string &fragmentShaderPath,
        const std::vector<ShaderVariable> &shaderVars,
        const std::string &diffuseTexture = "",
        bool printShaderLoadStatus = false);

    // Easy load of cube model, where mesh, shader, material names aren't
    // required to retrieve later.
    bool loadTexturedCubeModel(const std::string &modelName,
        const std::string &diffuseTexture);

    // Load cube, shader already loaded, just need material with texture
    bool loadTexturedCubeModel(const std::string &modelName,
        const std::string &meshName, const std::string &materialName,
        const std::string &shaderName, const std::string &diffuseTexture = "");

    // Load cube, first time shader loading.
    bool loadTexturedCubeModel(const std::string &modelName,
        const std::string &meshName, const std::string &materialName,
        const std::string &shaderName, const std::string &vertexShaderPath,
        const std::string &fragmentShaderPath,
        const std::vector<ShaderVariable> &shaderVars,
        const std::string &diffuseTexture = "",
        bool printShaderLoadStatus = false);

    bool loadBoundingBoxWireframe(BoundingBox &bb);

    bool loadMesh(const std::string &meshName,
        std::shared_ptr<SingleMesh> model);
    bool loadMesh(const std::string &meshName,
        PrimitiveType prim, const std::vector<Vertex> &vertices, 
        const std::vector<GLuint> &indices);
    bool loadCubeMesh(const std::string &meshName);
    bool loadSkyboxMesh(const std::string &meshName);

private:
    static std::unique_ptr<MeshManager> myInstance; // Singleton
    static int materialIndex;
    static int boundingBoxIndex;

    MeshManager();
    //    Mesh* buildSingleModel(const string &path, ImageManager &im,
    // int _index = 0);
    //PolyMesh* buildPolyModel(const string &path, ImageManager &im);
    

};