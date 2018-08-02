#pragma once
#include <vector>
#include <GL/glew.h>

#include "BasicTypes.h"
#include "MeshType.h"

class Material;
class ShaderProgram;
class ShaderVariable;

// It is up to the developer to make sure that the number of total vertices is
// consistent. For example, make sure that the total number of vertexPositions
// is consistent with the total number of vertexNormals.
class SingleMesh : public MeshType 
{
public:
    SingleMesh() {};
    SingleMesh(PrimitiveType prim, const std::vector<Vertex> &vertices,
        const std::vector<GLuint> &indices);

    // Case where we want a mesh without a texture.
    SingleMesh(PrimitiveType prim, const std::vector<Vertex> &vertices,
        const std::vector<GLuint> &indices, bool isBoundingBox);

    // Doesn't initialize mesh, shader or material, assume all are loaded
    // already.
    SingleMesh(const SingleMesh &baseMesh, const std::string &materialName);

    // Doesn't initialize mesh or shader, but initializes the material 
    // using the given shader and texture.
    SingleMesh(const SingleMesh &baseMesh, const std::string &materialName, 
        const std::string &shaderName, const std::string &diffuseTexture = "");

    // Initialize mesh, but not material or shader.
    SingleMesh(const std::string &materialName, PrimitiveType prim, 
        const std::vector<Vertex> &vertices, 
        const std::vector<GLuint> &indices);

    // Initialize material and shader, but not mesh.
    SingleMesh(const SingleMesh &baseMesh, const std::string &materialName,
        const std::string &shaderName, 
        const std::string &vertexShaderPath,
        const std::string &fragmentShaderPath,
        const std::vector<ShaderVariable> &shaderVars,
        const std::string &diffuseTexture = "",
        bool printShaderLoadStatus = false);

    // Initialization of everything, mesh, material and shader.
    SingleMesh(const std::string &materialName,
        const std::string &shaderName, PrimitiveType prim,
        const std::vector<Vertex> &vertices,
        const std::vector<GLuint> &indices,
        const std::string &vertexShaderPath,
        const std::string &fragmentShaderPath,
        const std::vector<ShaderVariable> &shaderVars,
        const std::string &diffuseTexture = "",
        bool printShaderLoadStatus = false);

    // Hard to keep track of multiple vbos, vaos etc.
    SingleMesh(const SingleMesh &other) = delete;
    SingleMesh(SingleMesh &&other) noexcept;

    SingleMesh& operator=(SingleMesh &other) = delete;
    SingleMesh& operator=(SingleMesh &&other);

    std::vector<Vertex> getVertices() const;
    std::vector<GLuint> getIndices() const;
    ShaderProgram* getShader() const;

    const GLuint getVAO() const { return vao; }
    const GLuint getVBO() const { return vbo; }
    const GLuint getIBO() const { return ibo; }
    PrimitiveType getPrimitiveType() const { return primitiveType; }

    void setVertices(std::vector<Vertex> vertices); 
    void setIndices(std::vector<GLuint> indices);
    // Order of ShaderVariables matters in the ShaderProgram.

    void setPrimitiveType(PrimitiveType prim);

    bool addExistingMaterial(const std::string &materialName);
    const std::vector<Material*> getMaterials() const;

    std::string className() const { return "SingleMesh"; }
    bool m_isBoundingBox;

    ~SingleMesh() noexcept;

protected:
    void bindAttributes();
    
    void initModel(const SingleMesh &baseMesh);
    void initModel(PrimitiveType prim, const std::vector<Vertex> &vertices,
        const std::vector<GLuint> &indices);

    void initMaterial(const std::string &materialName,
        const std::string &shaderName, const std::string &vertexShaderPath,
        const std::string &fragmentShaderPath,
        const std::vector<ShaderVariable> &shaderVars,
        const std::string &texturePath = "",
        bool printShaderLoadStatus = false);

    void initMaterialWithLoadedShader(const std::string &materialName,
        const std::string &shaderName, const std::string &texturePath);

    std::string name; 
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Material*> materials;

    GLuint vao, vbo, ibo;			// Vertex buffer, vertex array,
                                    //	index buffer.													
    PrimitiveType primitiveType;	// Using only triangles, or quads 
                                    //	as the main primitive
};