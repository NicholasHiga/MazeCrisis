#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "BasicTypes.h"

class SingleMesh;
class SceneNode;
class SceneGraph;

enum class BOUNDING_BOX_TYPE
{
    SPHERE, AABB, OBB
};

/// First a base bounding box needs to be initialized by calling 
/// calculateBaseBounds or calculateBoundsWithTransforms. This will create the
/// initial reference min and max of all x/y/z. Afterwards, any time the 
/// transformation matrix is update, updateBounds should be called.
class BoundingBox
{
public: 
    BoundingBox(const SingleMesh &mesh, BOUNDING_BOX_TYPE bbType,
        bool isVisible = false);

    //void updateBounds(Mat4x4 &transform); 
        // Should be called any time a new transformation matrix is used.
    void updateBounds(glm::mat4 &transform);
        // Should be called any time a new transformation matrix is used.
    BOUNDING_BOX_TYPE getBoundingBoxType() const;

    bool doesCollide(const BoundingBox &bb) const;
    static bool doesCollide(const BoundingBox &bb1, const BoundingBox &bb2);
        // Does not handle OBB
    std::vector<std::shared_ptr<SceneNode>> doesCollide(SceneGraph *graph); 
        // Check with all nodes in the graph for collisions.
    static std::vector<std::shared_ptr<SceneNode>> doesCollide(
        SceneGraph *graph, const BoundingBox &bb); 
        // Check with all nodes in the graph for collisions.
    std::vector<std::shared_ptr<SceneNode>> doesCollide(
        std::vector<std::shared_ptr<SceneNode>> &nodes,
        std::vector<unsigned int> *indices = nullptr);
        // Check with all nodes in the vector for collisions.
    static std::vector<std::shared_ptr<SceneNode>> doesCollide(
        std::vector<std::shared_ptr<SceneNode>> &nodes, const BoundingBox &bb,
        std::vector<unsigned int> *indices = nullptr);
        // Check with all nodes in the vector for collisions.

    glm::vec3 aabbMins, aabbMaxes;
        // Current minX, minY, minZ, maxX, maxY, maxZ
    glm::vec3 baseMins, baseMaxes; 
        // The base minX, minY, minZ, maxX, maxY, maxZ
    
    // Case for spherical BB.
    glm::vec3 center;
    glm::vec3 dimensions;
    float radius;
    bool isVisible;
    SingleMesh* associatedMesh;
    glm::mat4 lastTransform;

    std::vector<Vertex> openGLVerts; // Format to load bounding boxes to
                                     // OpenGL.
private:
    // Should only be used for initializing, call updateBounds after initial
    // min/maxs are calculated.
    void calculateBaseBounds(const SingleMesh &mesh, BOUNDING_BOX_TYPE bbType);

    BOUNDING_BOX_TYPE bbType;
};