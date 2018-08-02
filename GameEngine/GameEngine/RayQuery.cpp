#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "RayQuery.h"
#include "SceneNode.h"
#include "SceneGraph.h"
#include "GameObject.h"
#include "BoundingBox.h"
#include "EffectedModel.h"
#include "AbstractRenderer.h"

using glm::vec3;
using glm::mat4;
using std::vector;
using std::shared_ptr;

SceneGraph* RayQuery::graph;
AbstractRenderer* RayQuery::ren;

vector<shared_ptr<SceneNode>>
RayQuery::Raycast(vec3 origin, vec3 direction, bool sortListFromClosest,
    std::vector<float> *intersectionPts)
{
    vector<shared_ptr<SceneNode>> intersections;
    vector<float> intersectionPoints;
    graph->doSomethingEachElement(graph->getRootSceneNode(),
        [=, &intersections, &intersectionPoints](shared_ptr<SceneNode> n)
    {
        vector<Renderable*> *renderables = n.get()->getRenderables();
        for (size_t i = 0; i < (*renderables).size(); ++i)
        {
            if (GameObject* go = dynamic_cast<GameObject*>((*renderables)[i]))
            {                
                if (go->getIsEnabled())
                {
                    float intersectionLocation;
                    if (RayQuery::doesRayIntersect(origin, direction, 
                        n.get()->getScale(), n.get()->getModelMatrix(), 
                        go->getBoundingBoxes()[i], &intersectionLocation))
                    {
                        if (!sortListFromClosest)
                        {
                            intersections.push_back(n);
                            intersectionPoints.push_back(intersectionLocation);
                        }
                        else
                        {
                            auto lowBound = std::lower_bound(
                                intersectionPoints.begin(),
                                intersectionPoints.end(),
                                intersectionLocation);
                            int index = lowBound - intersectionPoints.begin();
                            intersectionPoints.insert(lowBound, 
                                intersectionLocation);
                            intersections.insert(intersections.begin() + index,
                                n);
                        }
                    }
                }
            }
        }
    });
    if (intersectionPts)
        *intersectionPts = intersectionPoints;
    return intersections;
}

vector<shared_ptr<SceneNode>>
RayQuery::Raycast(Ray ray, bool sortListFromClosest, 
    std::vector<float> *intersectionPts)
{
    return Raycast(ray.origin, ray.direction, sortListFromClosest,
        intersectionPts);
}

Ray 
RayQuery::getRayFromMouseClick(int mouseX, int mouseY)
{
    Ray r;
    glm::vec2 winDimensions = ren->getWindowSize();
    glm::vec4 viewport = glm::vec4(0.0f, 0.0f, winDimensions.x,
        winDimensions.y);
    int dy = (int)winDimensions.y - mouseY;

    r.origin = glm::unProject(glm::vec3(float(mouseX), dy, 0.0f),
        ren->getMatrixStack().getViewMatrix(),
        ren->getMatrixStack().getProjectionMatrix(), viewport);
    vec3 t = glm::unProject(glm::vec3(float(mouseX), dy, 1.0f),
        ren->getMatrixStack().getViewMatrix(), 
        ren->getMatrixStack().getProjectionMatrix(), viewport);
    r.direction = glm::normalize(t - r.origin);
    return r;
}

bool 
RayQuery::doesRayIntersect(vec3 origin, vec3 direction, vec3 scale,
    mat4 modelMatrix, BoundingBox* bb, float *location)
{
    if (bb->getBoundingBoxType() == BOUNDING_BOX_TYPE::SPHERE)
        return raySphere(origin, direction, modelMatrix, bb, location);
    else if (bb->getBoundingBoxType() == BOUNDING_BOX_TYPE::AABB)
        return rayAABB(origin, direction, bb->aabbMins, bb->aabbMaxes,
            location);
    else // OBB
    {
        vec3 axes[3];
        axes[0] = glm::normalize(vec3(modelMatrix[0].x, modelMatrix[0].y,
            modelMatrix[0].z));
        axes[1] = glm::normalize(vec3(modelMatrix[1].x, modelMatrix[1].y,
            modelMatrix[1].z));
        axes[2] = glm::normalize(vec3(modelMatrix[2].x, modelMatrix[2].y, 
            modelMatrix[2].z));

        return rayOBB(origin, direction, bb->center, axes, scale, location);
    }
}

bool
RayQuery::doesRayIntersect(Ray ray, vec3 scale, mat4 modelMatrix,
    BoundingBox* bb, float *location)
{
    return doesRayIntersect(ray.origin, ray.direction, scale, modelMatrix,
        bb, location);
}

bool
RayQuery::raySlab(float origin, float direction, float min,
    float max, float *tfirst, float *tlast)
{
    if (fabs(direction) < 1.0E-8)
        return (origin < max && origin > min);

    float tmin = (min - origin) / direction;
    float tmax = (max - origin) / direction;

    if (tmin > tmax)
        std::swap(tmin, tmax);

    if (tmax < *tfirst || tmin > *tlast)
        return false;

    if (tmin > *tfirst) *tfirst = tmin;
    if (tmax < *tlast)  *tlast = tmax;
    return true;
}

bool
RayQuery::rayAABB(vec3 origin, vec3 direction, vec3 min, vec3 max, 
    float *location)
{
    float tfirst = 0.0f, tlast = 100000.0f;

    if (!raySlab(origin.x, direction.x, min.x, max.x, &tfirst, &tlast))
        return false;
    if (!raySlab(origin.y, direction.y, min.y, max.y, &tfirst, &tlast))
        return false;
    if (!raySlab(origin.z, direction.z, min.z, max.z, &tfirst, &tlast))
        return false;

    if (location != nullptr)
        *location = tfirst;
    return true;
}

bool
RayQuery::rayOBB(vec3 origin, vec3 dir, vec3 center, vec3 axes[3], 
    vec3 dimensions, float *location)
{
    float tfirst = 0.0f, tlast = 100000.0f;

    if (!raySlab(glm::dot(origin, axes[0]), glm::dot(dir, axes[0]), 
        glm::dot(center, axes[0]) - dimensions.x / 2.0f, 
        glm::dot(center, axes[0]) + dimensions.x / 2.0f, &tfirst, &tlast))
        return false;
    if (!raySlab(glm::dot(origin, axes[1]), glm::dot(dir, axes[1]),
        glm::dot(center, axes[1]) - dimensions.y / 2.0f,
        glm::dot(center, axes[1]) + dimensions.y / 2.0f, &tfirst, &tlast))
        return false;
    if (!raySlab(glm::dot(origin, axes[2]), glm::dot(dir, axes[2]), 
        glm::dot(center, axes[2]) - dimensions.z / 2.0f, 
        glm::dot(center, axes[2]) + dimensions.z / 2.0f, &tfirst, &tlast))
        return false;

    if (location != nullptr)
        *location = tfirst;
    return true;
}

bool 
RayQuery::raySphere(vec3 origin, vec3 direction, mat4 ModelMatrix,
    BoundingBox* bb, float *location)
{
    // TODO: Implement ray-sphere collision detection.
    return true;
}

void
RayQuery::setSceneGraph(SceneGraph &graphRoot)
{
    graph = &graphRoot;
}

void 
RayQuery::setRenderer(AbstractRenderer &renderer)
{
    ren = &renderer;
}