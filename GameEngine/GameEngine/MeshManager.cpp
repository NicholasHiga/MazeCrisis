#include "MeshManager.h"

#include <glm/glm.hpp>
#include "BasicTypes.h"
#include "BoundingBox.h"
#include "ShaderVariable.h"
#include "MaterialManager.h"
#include "DefaultEngineVars.h"
#include "ShaderProgramManager.h"
#include "PredefinedVertices.h"

using glm::vec3;
using std::string;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;

unique_ptr<MeshManager> MeshManager::myInstance = nullptr;

int MeshManager::materialIndex = 0;
int MeshManager::boundingBoxIndex = 0;

MeshManager*
MeshManager::getInstance()
{
    if (myInstance == nullptr)
        myInstance = std::unique_ptr<MeshManager>(new MeshManager());

    return myInstance.get();
}

MeshManager::MeshManager()
{

}

// Initialize neither - assume both model, material are already loaded.
bool
MeshManager::loadModel(const string &modelName, const string &baseMeshName,
    const string &materialName)
{
    return store(modelName, std::make_shared<SingleMesh>(
        *MeshManager::getInstance()->get(baseMeshName), materialName));
}

// Doesn't initialize mesh or shader, but initializes the material using the
// given shader and texture.
bool
MeshManager::loadModel(const string &modelName, const string &baseMeshName,
    const string &materialName, const string &shaderName,
    const string &diffuseTexture)
{
    return store(modelName,
        std::make_shared<SingleMesh>(
            *MeshManager::getInstance()->get(baseMeshName), materialName, 
            shaderName, diffuseTexture));
}

// Initialize mesh, but not material or shader.
bool
MeshManager::loadModel(const string &modelName,
    const string &materialName, PrimitiveType prim,
    const vector<Vertex> &vertices, const vector<GLuint> &indices)
{
    return store(modelName,
        std::make_shared<SingleMesh>(materialName, prim, vertices,
            indices));
}

// Initialize material and shader, but not mesh.
bool
MeshManager::loadModel(const string &modelName, const string &baseMeshName,
    const string &materialName, const string &shaderName,
    const string &vertexShaderPath, const string &fragmentShaderPath,
    const vector<ShaderVariable> &shaderVars, const string &diffuseTexture,
    bool printShaderLoadStatus)
{
    return store(modelName,
        std::make_shared<SingleMesh>(
            *MeshManager::getInstance()->get(baseMeshName),
            materialName, shaderName,
            vertexShaderPath, fragmentShaderPath, shaderVars,
            diffuseTexture, printShaderLoadStatus));
}

// Initialization of everything.
bool
MeshManager::loadModel(const string &modelName,
    const string &materialName, const string &shaderName, PrimitiveType prim,
    const vector<Vertex> &vertices, const vector<GLuint> &indices,
    const string &vertexShaderPath, const string &fragmentShaderPath,
    const vector<ShaderVariable> &shaderVars, const string &diffuseTexture,
    bool printShaderLoadStatus)
{
    return store(modelName,
        std::make_shared<SingleMesh>(materialName,
            shaderName, prim, vertices, indices, vertexShaderPath,
            fragmentShaderPath, shaderVars, diffuseTexture,
            printShaderLoadStatus));
}

bool
MeshManager::loadTexturedCubeModel(const std::string &modelName,
    const std::string &diffuseTexture)
{
    vector<ShaderVariable> defaultCubeShaderVars
        = ShaderProgramManager::getDefaultShaderVars();

    if (!MeshManager::getInstance()->get(defaultMeshName))
        MeshManager::getInstance()->loadCubeMesh(defaultMeshName);

    std::string matName = defaultMaterialName + std::to_string(materialIndex);
    if (!MaterialManager::getInstance()->get(matName))
    {
        MaterialManager::getInstance()->loadMaterial(matName,
            defaultShaderName, diffuseTexture);
        MeshManager::materialIndex++;
    }

    if (!ShaderProgramManager::getInstance()->get(defaultShaderName))
        ShaderProgramManager::getInstance()->loadShader(defaultShaderName,
            defaultVertexShaderPath, defaultFragmentShaderPath,
            defaultCubeShaderVars);

    return loadModel(modelName, defaultMeshName, matName);
}

// Load cube, shader already loaded, just need material with texture
bool
MeshManager::loadTexturedCubeModel(const string &modelName, 
    const string &meshName, const string &materialName,
    const string &shaderName, const string &diffuseTexture)
{
    if (!MeshManager::getInstance()->get(meshName))
        MeshManager::getInstance()->loadCubeMesh(meshName);

    return loadModel(modelName, meshName, materialName, shaderName,
        diffuseTexture);
}

// Load cube, first time shader loading.
bool
MeshManager::loadTexturedCubeModel(const string &modelName, 
    const string &meshName, const string &materialName, 
    const string &shaderName, const string &vertexShaderPath, 
    const string &fragmentShaderPath, const vector<ShaderVariable> &shaderVars,
    const string &diffuseTexture, bool printShaderLoadStatus)
{
    if (!MeshManager::getInstance()->get(meshName))
        MeshManager::getInstance()->loadCubeMesh(meshName);

    return loadModel(modelName, meshName, materialName, shaderName,
        vertexShaderPath, fragmentShaderPath, shaderVars, diffuseTexture,
        printShaderLoadStatus);
}

bool 
MeshManager::loadBoundingBoxWireframe(BoundingBox &bb)
{
    bb.openGLVerts = std::vector<Vertex>(24, Vertex());
    bb.openGLVerts[0].position = vec3(bb.baseMins.x, bb.baseMins.y, bb.baseMaxes.z);
    bb.openGLVerts[1].position = vec3(bb.baseMaxes.x, bb.baseMins.y, bb.baseMaxes.z);
    bb.openGLVerts[2].position = vec3(bb.baseMaxes.x, bb.baseMaxes.y, bb.baseMaxes.z);
    bb.openGLVerts[3].position = vec3(bb.baseMins.x, bb.baseMaxes.y, bb.baseMaxes.z);

    bb.openGLVerts[4].position = vec3(bb.baseMins.x, bb.baseMaxes.y, bb.baseMaxes.z);
    bb.openGLVerts[5].position = vec3(bb.baseMaxes.x, bb.baseMaxes.y, bb.baseMaxes.z);
    bb.openGLVerts[6].position = vec3(bb.baseMaxes.x, bb.baseMaxes.y, bb.baseMins.z);
    bb.openGLVerts[7].position = vec3(bb.baseMins.x, bb.baseMaxes.y, bb.baseMins.z);

    bb.openGLVerts[8].position = vec3(bb.baseMaxes.x, bb.baseMins.y, bb.baseMins.z);
    bb.openGLVerts[9].position = vec3(bb.baseMins.x, bb.baseMins.y, bb.baseMins.z);
    bb.openGLVerts[10].position = vec3(bb.baseMins.x, bb.baseMaxes.y, bb.baseMins.z);
    bb.openGLVerts[11].position = vec3(bb.baseMaxes.x, bb.baseMaxes.y, bb.baseMins.z);

    bb.openGLVerts[12].position = vec3(bb.baseMins.x, bb.baseMins.y, bb.baseMins.z);
    bb.openGLVerts[13].position = vec3(bb.baseMaxes.x, bb.baseMins.y, bb.baseMins.z);
    bb.openGLVerts[14].position = vec3(bb.baseMaxes.x, bb.baseMins.y, bb.baseMaxes.z);
    bb.openGLVerts[15].position = vec3(bb.baseMins.x, bb.baseMins.y, bb.baseMaxes.z);

    bb.openGLVerts[16].position = vec3(bb.baseMins.x, bb.baseMins.y, bb.baseMins.z);
    bb.openGLVerts[17].position = vec3(bb.baseMins.x, bb.baseMins.y, bb.baseMaxes.z);
    bb.openGLVerts[18].position = vec3(bb.baseMins.x, bb.baseMaxes.y, bb.baseMaxes.z);
    bb.openGLVerts[19].position = vec3(bb.baseMins.x, bb.baseMaxes.y, bb.baseMins.z);

    bb.openGLVerts[20].position = vec3(bb.baseMaxes.x, bb.baseMins.y, bb.baseMaxes.z);
    bb.openGLVerts[21].position = vec3(bb.baseMaxes.x, bb.baseMins.y, bb.baseMins.z);
    bb.openGLVerts[22].position = vec3(bb.baseMaxes.x, bb.baseMaxes.y, bb.baseMins.z);
    bb.openGLVerts[23].position = vec3(bb.baseMaxes.x, bb.baseMaxes.y, bb.baseMaxes.z);

    for (auto &v : bb.openGLVerts)
    {
        v.color = Color(1.0f, 1.0f, 1.0f, 1.0f);
    }

    if (!ShaderProgramManager::getInstance()->get(boundingBoxShaderName))
    {
        ShaderProgramManager::getInstance()->loadShader(boundingBoxShaderName,
            boundingBoxVertexShaderPath, boundingBoxFragmentShaderPath,
            ShaderProgramManager::getBoundingBoxShaderVars());
    }

    std::string meshName = "boundingBox" + std::to_string(boundingBoxIndex++);
    bool ret = store(meshName, std::make_shared<SingleMesh>
        (PrimitiveType::PRIM_TRIANGLE, bb.openGLVerts, CUBE_INDICES, true));
    bb.associatedMesh = get(meshName);

    return ret;
}

bool
MeshManager::loadMesh(const string &meshName, shared_ptr<SingleMesh> model)
{
    return store(meshName, model);
}

bool
MeshManager::loadMesh(const string &meshName, PrimitiveType prim,
    const vector<Vertex> &vertices, const vector<GLuint> &indices)
{
    return store(meshName, std::make_shared<SingleMesh>(prim, 
        vertices, indices));
}

bool 
MeshManager::loadCubeMesh(const string &meshName)
{
    vector<Vertex> cubeVertices;
    for (size_t i = 0; i < CUBE_POSITIONS.size() / 3; ++i)
    {
        Vertex v;
        v.position = glm::vec3(CUBE_POSITIONS[i * 3], CUBE_POSITIONS[i * 3 + 1],
            CUBE_POSITIONS[i * 3 + 2]);
        v.uv = glm::vec2(CUBE_UVS[i * 2], CUBE_UVS[i * 2 + 1]);
        v.color = Color(CUBE_COLORS[i * 3], CUBE_COLORS[i * 3 + 1],
            CUBE_COLORS[i * 3 + 2]);
        cubeVertices.push_back(v);
    }

    return store(meshName, std::make_shared<SingleMesh>
        (PrimitiveType::PRIM_TRIANGLE, cubeVertices, CUBE_INDICES));
}

bool 
MeshManager::loadSkyboxMesh(const std::string &meshName)
{
    vector<Vertex> skyboxVerts;
    vector<GLuint> empty;
    for (size_t i = 0; i < SKYBOX_VERTS.size() / 3; ++i)
    {
        Vertex v;
        v.position = glm::vec3(SKYBOX_VERTS[i * 3], SKYBOX_VERTS[i * 3 + 1],
            SKYBOX_VERTS[i * 3 + 2]);
        skyboxVerts.push_back(v);
    }

    return store(meshName, std::make_shared<SingleMesh>
         (PrimitiveType::PRIM_TRIANGLE, skyboxVerts, empty));
}