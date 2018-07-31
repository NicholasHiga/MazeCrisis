#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <memory>
#include <string>
#include <vector>

class Image;

// Color RGBA normalized from 0 - 1.0
class Color
{
public:
    Color() : Color(0.0f, 0.0f, 0.0f, 1.0f) {};
    Color(float _r, float _g, float _b, float _a) : r(_r), g(_g),
        b(_b), a(_a) {};
    Color(float _r, float _g, float _b) : r(_r), g(_g), b(_b), a(1.0f) {};

    float r, g, b, a;
};

struct Vertex
{
    glm::vec3 position, normal;
    Color color;
    glm::vec2 uv;
};

enum class TextureStorageType
{
    SINGLE,
    CUBEMAP
};

enum class TextureType
{
    DIFFUSE,
    SPECULAR,
    NORMAL,
    HEIGHT,
    SKYBOX,
    OTHER
};

// Used to associate a image name with a path and the
// image itself. This is so a image can be referred to
// later on using the given name, instead of using the
// path every time.
struct Texture
{
    GLuint textureID;
    std::vector<std::string> paths;
    std::vector<std::shared_ptr<Image>> images;
    TextureStorageType textureStorageType;
    TextureType textureType;
};

// Made to match with OpenGL defines.
enum PrimitiveType
{
    PRIM_POINT = 0x0000,
    PRIM_LINE = 0x0001,
    PRIM_TRIANGLE = 0x0004,
    PRIM_TRIANGLE_FAN = 0x0006,
    PRIM_QUAD = 0x0007,
    PRIM_INVALID = 0x0008
};

class Helper
{
public:
    static PrimitiveType numVertsToPrimType(unsigned char numVerts)
    {
        if (numVerts == 1)
            return PrimitiveType::PRIM_POINT;
        else if (numVerts == 2)
            return PrimitiveType::PRIM_LINE;
        else if (numVerts == 3)
            return PrimitiveType::PRIM_TRIANGLE;
        else if (numVerts == 4)
            return PrimitiveType::PRIM_QUAD;
        else
            return PrimitiveType::PRIM_INVALID;
    }

    static std::string getFullPath(std::string current_path,
        std::string asset_name)
    {
        std::string newPath = current_path.substr(0, 
            current_path.find_last_of("/") + 1);
        newPath += asset_name;
        return newPath;
    }
};
