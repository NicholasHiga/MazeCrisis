#include "SingleMesh.h"

#include "MeshManager.h"
#include "ShaderProgram.h"
#include "ShaderVariable.h"
#include "MaterialManager.h"

using std::vector;
using std::string;

SingleMesh::SingleMesh(const string &name, PrimitiveType prim,
    const vector<Vertex> &vertices, const vector<GLuint> &indices)
{
    initModel(name, prim, vertices, indices);
}

// Doesn't initialize mesh, shader or material, assume all are loaded already.
SingleMesh::SingleMesh(const SingleMesh &baseMesh, const string &name,
    const string &materialName)
{
    initModel(name, baseMesh);
    addExistingMaterial(materialName);
    bindAttributes();
}

// Doesn't initialize mesh or shader, but initializes the material using 
// the given shader and texture.
SingleMesh::SingleMesh(const SingleMesh &baseMesh, const string &name,
    const string &materialName, const string &shaderName,
    const string &diffuseTexture)
{
    initModel(name, baseMesh);
    initMaterialWithLoadedShader(materialName, shaderName, diffuseTexture);
    bindAttributes();
}

// Initialize mesh, but not material or shader.
SingleMesh::SingleMesh(const string &name, const string &materialName,
    PrimitiveType prim, const vector<Vertex> &vertices,
    const vector<GLuint> &indices)
{
    addExistingMaterial(materialName);
    initModel(name, prim, vertices, indices);
    bindAttributes();
}

// Initialize material and shader, but not mesh.
SingleMesh::SingleMesh(const SingleMesh &baseMesh, const string &name,
    const string &materialName, const string &shaderName,
    const string &vertexShaderPath, const string &fragmentShaderPath, 
    const vector<ShaderVariable> &shaderVars, const string &diffuseTexture,
    bool printShaderLoadStatus)
{
    initModel(name, baseMesh);
    initMaterial(materialName, shaderName, vertexShaderPath, fragmentShaderPath,
        shaderVars, diffuseTexture, printShaderLoadStatus);
    bindAttributes();
}

// Initialization of everything, mesh, material and shader.
SingleMesh::SingleMesh(const string &name,
    const string &materialName, const string &shaderName, PrimitiveType prim,
    const vector<Vertex> &vertices, const vector<GLuint> &indices,
    const string &vertexShaderPath, const string &fragmentShaderPath,
    const vector<ShaderVariable> &shaderVars, const string &diffuseTexture,
    bool printShaderLoadStatus)
{
    initModel(name, prim, vertices, indices);
    initMaterial(materialName, shaderName, vertexShaderPath, fragmentShaderPath,
        shaderVars, diffuseTexture, printShaderLoadStatus);
    bindAttributes();
}

SingleMesh::SingleMesh(SingleMesh &&other) noexcept
{
    name = std::move(other.name);
    vertices = std::move(other.vertices);
    indices = std::move(other.indices);
    materials = std::move(other.materials);
    vao = std::move(other.vao);
    vbo = std::move(other.vbo);
    ibo = std::move(other.ibo);
    other.vao = 0;
    other.vbo = 0;
    other.ibo = 0;
    primitiveType = other.primitiveType;
}

SingleMesh& 
SingleMesh::operator=(SingleMesh &&other)
{
    if (this != &other)
    {
        glDeleteBuffers(1, &ibo);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
        name = std::move(other.name);
        vertices = std::move(other.vertices);
        indices = std::move(other.indices);
        materials = std::move(other.materials);
        vao = other.vao;
        vbo = other.vbo;
        ibo = other.ibo;
        primitiveType = other.primitiveType;
    }
    return *this;
}

std::string
SingleMesh::getName() const
{
    return name;
}

vector<Vertex>
SingleMesh::getVertices() const
{ 
    return vertices;
}

vector<GLuint>
SingleMesh::getIndices() const
{
    return indices;
}

void
SingleMesh::setName(const string &name)
{
    this->name = name;
}

void
SingleMesh::setVertices(vector<Vertex> vertices)
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        &vertices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void 
SingleMesh::setIndices(vector<GLuint> indices)
{
    if (!indices.empty())
    {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(GLuint),
            &indices[0], GL_STATIC_DRAW);

        glBindVertexArray(0);
    }
}

void
SingleMesh::setPrimitiveType(PrimitiveType prim)
{
    try
    {
        if (prim != PrimitiveType::PRIM_TRIANGLE &&
            prim != PrimitiveType::PRIM_QUAD)
            throw std::invalid_argument(
                "PrimitiveType needs to be PRIM_TRIANGLE or PRIM_QUAD");
        this->primitiveType = prim;
    }
    catch (std::exception &e) {
        throw e;
    }
}

bool
SingleMesh::addExistingMaterial(const string &materialName)
{
    if (Material* m = MaterialManager::getInstance()->get(materialName))
    {
        materials.push_back(m);
        return true;
    }
    return false;
}

const std::vector<Material*>
SingleMesh::getMaterials() const
{
    return materials;
}

void
SingleMesh::bindAttributes()
{
    glBindVertexArray(getVAO());

    // TO DO: MAKE SURE WORKING WITH MULTIPLE MATERIALS!!
    vector<ShaderVariable> vars = getMaterials()[0]->getShader()
        ->getShaderVars();

    for (size_t i = 0; i < vars.size(); ++i)
    {
        glEnableVertexAttribArray(i);
        glBindBuffer(GL_ARRAY_BUFFER, getVBO());
        ENGINE_VAR eVar = vars[i].getEngineVarType();

        if (eVar == ENGINE_VAR::VERTEX_POSITION)
            glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE,
                sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
        else if (eVar == ENGINE_VAR::VERTEX_NORMAL)
            glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE,
                sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
        else if (eVar == ENGINE_VAR::VERTEX_UV)
            glVertexAttribPointer(i, 2, GL_FLOAT, GL_FALSE,
                sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));
        else if (eVar == ENGINE_VAR::VERTEX_COLOR)
            glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE,
                sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
    }

    glBindVertexArray(0);
}

void 
SingleMesh::initModel(const std::string &name, const SingleMesh &baseMesh)
{
    initModel(name, baseMesh.getPrimitiveType(), baseMesh.getVertices(),
        baseMesh.getIndices());
}

void
SingleMesh::initModel(const string &name, PrimitiveType prim,
    const vector<Vertex> &vertices, const vector<GLuint> &indices)
{
    setPrimitiveType(prim);
    this->vertices = vertices;
    this->indices = indices;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    if (!indices.empty())
        glGenBuffers(1, &ibo);

    setName(name);
    setIndices(indices);
    setVertices(vertices);
}

void
SingleMesh::initMaterial(const string &materialName,
    const string &shaderName, const string &vertexShaderPath,
    const string &fragmentShaderPath, const vector<ShaderVariable> &shaderVars,
    const string &diffuseTexture, bool printShaderLoadStatus)
{
    try
    {
        MaterialManager::getInstance()->loadMaterial(materialName, shaderName,
            vertexShaderPath, fragmentShaderPath, shaderVars, diffuseTexture,
            printShaderLoadStatus);
        addExistingMaterial(materialName);
    }
    catch (std::exception &e)
    {
        throw e;
    }
}

void
SingleMesh::initMaterialWithLoadedShader(const string &materialName,
    const string &shaderName, const string &diffuseTexture)
{
    try
    {
        MaterialManager::getInstance()->loadMaterial(materialName, shaderName,
            diffuseTexture);
        addExistingMaterial(materialName);
    }
    catch (std::exception &e)
    {
        throw e;
    }
}

SingleMesh::~SingleMesh() noexcept
{
    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}