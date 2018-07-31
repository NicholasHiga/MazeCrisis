#include "GameObject.h"

#include "SceneNode.h"
#include "BoundingBox.h"
#include "MeshManager.h"
#include "ModelManager.h"
#include "ShaderVariable.h"

using std::string;
using std::vector;
using std::function;
using std::unique_ptr;
using std::shared_ptr;

GameObject::GameObject()
{
    init("", "", MESH_TYPE::UNINITIALIZED);
}

GameObject::GameObject(const string &gameObjectName, const string &modelName,
    MESH_TYPE meshType)
{
    init(gameObjectName, modelName, meshType);
}

GameObject::GameObject(const string &gameObjectName, const string &modelName,
    shared_ptr<EffectedModel> model)
{
    init(gameObjectName, modelName, MESH_TYPE::EFFECTED_MODEL);
    try
    {
        bool success = ModelManager::getInstance()->loadModel(modelName, model);
        if (!success)
            throw std::runtime_error("Model " + modelName
                + " did not load successfully.");
    }
    catch (std::exception &e)
    {
        throw e;
    }
}

// Initialize neither - assume both model, material are already loaded.
GameObject::GameObject(const string &gameObjectName, const string &modelName,
    const string &meshName, const string &materialName)
{
    init(gameObjectName, modelName, MESH_TYPE::SINGLE_MESH);
    try
    {
        bool success = MeshManager::getInstance()->loadModel(modelName,
            meshName, materialName);
        if (!success)
            throw std::runtime_error("Model " + modelName +
                " did not load successfully.");
    }
    catch (std::exception &e)
    {
        throw e;
    }
}

// Doesn't initialize mesh or shader, but initializes the material using 
// the given shader and texture.
GameObject::GameObject(const string &gameObjectName, const string &modelName,
    const string &meshName, const string &materialName,
    const string &shaderName, const string &diffuseTexture, bool loadAsCube)
{
    init(gameObjectName, modelName, MESH_TYPE::SINGLE_MESH);
    try
    {
        bool success;
        if (loadAsCube)
            success = MeshManager::getInstance()->loadModel(modelName, 
                meshName, materialName, shaderName, diffuseTexture);
        else
            success = MeshManager::getInstance()->loadCubeModel(modelName,
                meshName, materialName, shaderName, diffuseTexture);

        if (!success)
            throw std::runtime_error("Model " + modelName +
                " did not load successfully.");
    }
    catch (std::exception &e)
    {
        throw e;
    }
}

// Initialize mesh, but not material.
GameObject::GameObject(const string &gameObjectName, const string &modelName,
    const string &meshName, const string &materialName, PrimitiveType prim, 
    vector<Vertex> vertices, vector<GLuint> indices)
{
    init(gameObjectName, modelName, MESH_TYPE::SINGLE_MESH);
    try
    {
        bool success = MeshManager::getInstance()->loadModel(modelName,
            meshName, materialName, prim, vertices, indices);
        if (!success)
            throw std::runtime_error("Model " + modelName + 
                " did not load successfully.");
    }
    catch (std::exception &e)
    {
        throw e;
    }
}

// Initialize material, but not mesh.
GameObject::GameObject(const string &gameObjectName, const string &modelName, 
    const string &meshName, const string &materialName, 
    const string &shaderName, const string &vertexShaderPath,
    const string &fragmentShaderPath, vector<ShaderVariable> shaderVars, 
    const string &diffuseTexture, bool loadAsCube, bool printShaderLoadStatus)
{
    init(gameObjectName, modelName, MESH_TYPE::SINGLE_MESH);
    try
    {
        bool success;
        if (loadAsCube)
            success = MeshManager::getInstance()->loadCubeModel(modelName,
                meshName, materialName, shaderName, vertexShaderPath, 
                fragmentShaderPath, shaderVars, diffuseTexture,
                printShaderLoadStatus);
        else
            success = MeshManager::getInstance()->loadModel(modelName,
                meshName, materialName, shaderName, vertexShaderPath,
                fragmentShaderPath, shaderVars, diffuseTexture,
                printShaderLoadStatus);

        if (!success)
            throw std::runtime_error("Model " + modelName + 
                " did not load successfully.");
    }
    catch (std::exception &e)
    {
        throw e;
    }
}

// Initialization of everything.
GameObject::GameObject(const string &gameObjectName, const string &modelName, 
    const string &meshName, const string &materialName,
    const string &shaderName, PrimitiveType prim, vector<Vertex> vertices,
    vector<GLuint> indices, const string &vertexShaderPath,
    const string &fragmentShaderPath, vector<ShaderVariable> shaderVars,
    const string &diffuseTexture, bool printShaderLoadStatus)
{
    init(gameObjectName, modelName, MESH_TYPE::SINGLE_MESH);
    try
    {
        bool success = MeshManager::getInstance()->loadModel(modelName,
            meshName, materialName, shaderName, prim, vertices, indices,
            vertexShaderPath, fragmentShaderPath, shaderVars, diffuseTexture, 
            printShaderLoadStatus);
        if (!success)
            throw std::runtime_error("Model " + modelName + 
                " did not load successfully.");
    }
    catch (std::exception &e)
    {
        throw e;
    }
}

void
GameObject::setGameObjectName(const std::string &name)
{
    this->gameObjectName = name;
}

string
GameObject::getGameObjectName() const
{
    return gameObjectName;
}

MESH_TYPE 
GameObject::getMeshType() const
{
    return meshType;
}

void 
GameObject::setModelName(const string &modelName, MESH_TYPE meshType)
{ 
    this->modelName = modelName;
    this->meshType = meshType;
}

string
GameObject::getModelName() const
{
    return modelName; 
}

MeshType*
GameObject::getModel() const
{
    if (meshType == MESH_TYPE::SINGLE_MESH)
        return MeshManager::getInstance()->get(modelName);
    else if (meshType == MESH_TYPE::EFFECTED_MODEL)
        return ModelManager::getInstance()->get(modelName);
}

void
GameObject::setParentNode(shared_ptr<SceneNode> parent)
{
    this->parentNode = parent;

    for (size_t i = 0; i < boundingBoxes.size(); ++i)
        boundingBoxes[i].get()->updateBounds(parent.get()->getModelMatrix());
}

SceneNode* 
GameObject::getParentNode()
{
    return parentNode.get();
}

void 
GameObject::setIsVisible(bool visible)
{
    isVisible = visible;
}

bool
GameObject::getIsVisible() const
{
    return isVisible;
}

void 
GameObject::setIsEnabled(bool enabled)
{
    isEnabled = enabled;
}

bool
GameObject::getIsEnabled() const
{
    return isEnabled;
}


void
GameObject::setScript(function<void(SceneNode*, double)> script)
{
    this->script = script;
}

function<void(SceneNode*, double)>
GameObject::getScript() const
{
    return script;
}

vector<BoundingBox*>
GameObject::getBoundingBoxes() const
{
    vector<BoundingBox*> bbs;
    for (size_t i = 0; i < boundingBoxes.size(); ++i)
        bbs.push_back(boundingBoxes[i].get());
    return bbs;
}

void
GameObject::update(double deltaTime)
{
    if (isEnabled)
        script(parentNode.get(), deltaTime);
}

void 
GameObject::init(const string &gameObjectName, const string &modelName,
    MESH_TYPE meshType)
{
    setModelName(modelName, meshType);
    this->gameObjectName = gameObjectName;

    script = [](SceneNode*, double) {};
    isEnabled = true;
    isVisible = true;

    // TODO: Change when PolyMesh implemented.
    if (SingleMesh* m = dynamic_cast<SingleMesh*>(getModel()))
        createBoundingBox(m, BOUNDING_BOX_TYPE::OBB);
    else if (EffectedModel *m = dynamic_cast<EffectedModel*>(getModel()))
    {
        const std::vector<SingleMesh> *meshes = m->getMeshes();
        for (auto a = 0; a < meshes->size(); a++)
        {
            createBoundingBox(&((*meshes)[a]), BOUNDING_BOX_TYPE::OBB);
        }
    }
}

void 
GameObject::createBoundingBox(const SingleMesh* sm, BOUNDING_BOX_TYPE bbt)
{
    boundingBoxes.push_back(std::make_unique<BoundingBox>(*sm, bbt));
}