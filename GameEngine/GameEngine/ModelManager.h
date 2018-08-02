#pragma once
#include <vector>
#include <string>
#include <memory>

#include "ResourceManager.h"
#include "BasicTypes.h"
#include "EffectedModel.h"

class ModelManager : public ResourceManager<EffectedModel>
{
public:
    static ModelManager* getInstance();

    // Used for loading any model from a file.
    bool loadModel(const std::string &modelName, const std::string &filepath);

    bool loadModel(const std::string &modelName,
        std::shared_ptr<EffectedModel> model);

private:
    static std::unique_ptr<ModelManager> myInstance; // Singleton

    ModelManager() {};
};