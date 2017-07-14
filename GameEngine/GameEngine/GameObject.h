#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>

#include "Renderable.h"
#include "BasicTypes.h"

class Material;
class SceneNode;
class SingleMesh;
class BoundingBox;
class EffectedModel;
class ShaderVariable;

class GameObject : public Renderable
{
public:
	GameObject();

	// Model with given name already loaded.
	GameObject(const std::string &modelName);

	GameObject(const std::string &modelName, std::shared_ptr<EffectedModel> model);

	// Doesn't initialize mesh, shader or material, assume all are loaded already.
	GameObject(const std::string &modelName, const std::string &meshName, const std::string &materialName);

	// Doesn't initialize mesh or shader, but initializes the material using the given shader
	// and texture.
	GameObject(const std::string &modelName, const std::string &meshName, const std::string &materialName,
		const std::string &shaderName, const std::string &diffuseTexture = "", bool loadAsCube = false);

	// Initialize mesh, but not material or shader.
	GameObject(const std::string &modelName, const std::string &meshName, const std::string &materialName, PrimitiveType prim,
		std::vector<Vertex> vertices, std::vector<GLuint> indices);

	// Initialize material and shader, but not mesh.
	GameObject(const std::string &modelName, const std::string &meshName, const std::string &materialName,
		const std::string &shaderName, const std::string &vertexShaderPath, const std::string &fragmentShaderPath,
		std::vector<ShaderVariable> shaderVars, const std::string &diffuseTexture = "",
		bool loadAsCube = false, bool printShaderLoadStatus = false);

	// Initialization of everything, mesh, material and shader.
	GameObject(const std::string &modelName, const std::string &meshName, const std::string &materialName,
		const std::string &shaderName, PrimitiveType prim, std::vector<Vertex> vertices,
		std::vector<GLuint> indices, const std::string &vertexShaderPath,
		const std::string &fragmentShaderPath, std::vector<ShaderVariable> shaderVars,
		const std::string &diffuseTexture = "", bool printShaderLoadStatus = false);
		
	void setMaterialName(const std::string &matName);
	std::string getMaterialName();
	Material* getMaterial();

	void setMeshName(const std::string &meshName);
	std::string getMeshName();
	SingleMesh* getMesh(); 	// TODO: Change to MeshType when PolyMesh implemented.

	void setModelName(const std::string &modelName);
	std::string getModelName();
	EffectedModel* getModel();

	void setParentNode(std::shared_ptr<SceneNode> parent);
	SceneNode* getParentNode();

	void setIsVisible(bool visible);
	bool getIsVisible();

	void setIsEnabled(bool enabled);
	bool getIsEnabled();

	void setScript(std::function<void(SceneNode*, double)> script);
	std::function<void(SceneNode*, double)> getScript();

	std::vector<BoundingBox*> getBoundingBoxes();

	// GameObject(SceneNode &parent, string modelPath); When model loader completed.

	void update(double deltaTime);

	std::string name() const { return "GameObject"; }

protected:
	void init(const std::string &modelName);
	std::shared_ptr<SceneNode> parentNode;
	std::string modelName;
	std::function<void(SceneNode*, double)> script;
	std::vector<std::unique_ptr<BoundingBox>> boundingBoxes;
	bool isVisible;		// Determines if being rendered.
	bool isEnabled;		// Determines if scripts active/inactive, collisions.
};