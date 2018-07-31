#pragma once
#include <string>

// TODO: Add pathing such that the shaders are found regardless of the project
// location. Looking back at this code a long time afterwards, this smells.
// A lot.

static const std::string defaultMeshName = "DefaultMesh.mesh";
static const std::string defaultMaterialName = "DefaultMaterial.mat";
static const std::string defaultShaderName = "DefaultShader.shader";
static const std::string defaultVertexShaderPath
= "../GameEngine/DefaultShaders/DefaultVertex.vert";
static const std::string defaultFragmentShaderPath
= "../GameEngine/DefaultShaders/DefaultFragment.frag";