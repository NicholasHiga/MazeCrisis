#include "ShaderProgramManager.h"
#include "ShaderVariable.h"

using std::string;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;

unique_ptr<ShaderProgramManager> ShaderProgramManager::myInstance = nullptr;

ShaderProgramManager*
ShaderProgramManager::getInstance()
{
	if (myInstance == nullptr)
		myInstance = unique_ptr<ShaderProgramManager>(new ShaderProgramManager());

	return myInstance.get();
}

ShaderManagerLoadStatus
ShaderProgramManager::loadShader(const string &shaderName, const string &vertexFilePath, const string &fragmentFilePath,
	const vector<ShaderVariable> &shaderVars)
{
	ShaderManagerLoadStatus status;
	status.shaderLoadStatus = OK;
	status.message = "\"" + shaderName + "\" shader successfully loaded.";

	if (get(shaderName) != nullptr)
	{
		status.shaderLoadStatus = SHADER_NAME_TAKEN;
		status.message = "Shader name \"" + shaderName + "\" already taken, use a different name.";
		return status;
	}

	for (ResourceIterator i = myInstance->begin(); i != myInstance->end(); ++i)
	{
		// Checking to see if the shader already exists.
		if (i->second->getVertexShaderFilePath() == vertexFilePath
			&& i->second->getFragmentShaderFilePath() == fragmentFilePath)
		{
			status.shaderLoadStatus = DUPLICATE_SHADER;
			status.message = i->first;
			return status;
		}
	}

	try
	{
		shared_ptr<ShaderProgram> prog = std::make_shared<ShaderProgram>(vertexFilePath, fragmentFilePath, shaderVars);
		if (!store(shaderName, prog))
		{
			status.shaderLoadStatus = LOAD_FAILED;
			status.message = "Shader failed to load.";
		}

		return status;
	}
	catch (std::exception &e)
	{
		throw e;
	}
}