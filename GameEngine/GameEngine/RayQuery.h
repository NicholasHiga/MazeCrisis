#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <vector>

class SceneNode;
class SceneGraph;
class BoundingBox;
class AbstractRenderer;

struct Ray
{
    glm::vec3 origin, direction;
};

class RayQuery
{
public:
    // Returning the SceneNode in these ray casts, because the node contains the 
    // information about rotations, scale, and position. Each Renderable such as 
    // an EffectedModel will not be unique, as it is managed by the 
    // ModelManager. If we return a non-unique Renderable, the incorrect
    // Renderable might be accessed.
    static std::vector<std::shared_ptr<SceneNode>> Raycast(glm::vec3 origin,
        glm::vec3 direction,
        bool sortListFromClosest = false, 
        std::vector<float> *intersectionPts = nullptr);
    static std::vector<std::shared_ptr<SceneNode>> Raycast(Ray ray,
        bool sortListFromClosest = false,
        std::vector<float> *intersectionPts = nullptr);
    static void setSceneGraph(SceneGraph &graphRoot);
    static void setRenderer(AbstractRenderer &renderer);
    static Ray getRayFromMouseClick(int mouseX, int mouseY);

    static bool doesRayIntersect(glm::vec3 origin, glm::vec3 direction, 
        glm::vec3 scale, glm::mat4 modelMatrix, BoundingBox* bb,
        float *location = nullptr);
    static bool doesRayIntersect(Ray ray, glm::vec3 scale,
        glm::mat4 modelMatrix, BoundingBox* bb, float *location = nullptr);

private:
    static bool raySlab(float start, float direction, float min, float max,
        float *tfirst, float* tlast);
    static bool rayAABB(glm::vec3 start, glm::vec3 direction, glm::vec3 min,
        glm::vec3 max, float *location = nullptr);
    static bool rayOBB(glm::vec3 start, glm::vec3 dir, glm::vec3 center, 
        glm::vec3 axes[3], glm::vec3 dimensions, float *location = nullptr);
    static bool raySphere(glm::vec3 origin, glm::vec3 direction,
        glm::mat4 ModelMatrix, BoundingBox* bb, float *location = nullptr);

    static SceneGraph* graph;
    static AbstractRenderer *ren;
};