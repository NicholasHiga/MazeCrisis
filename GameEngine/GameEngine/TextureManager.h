#pragma once
#include <memory>

#include "BasicTypes.h"
#include "ResourceManager.h"

class TextureManager : public ResourceManager<Texture>
{
public:
	~TextureManager();

	static TextureManager* getInstance();
	bool loadTexture(const std::string &path, const std::string &name = "");	
	// In the case of no name parameter given,																			
	// user can just load the texture without																				
	// giving a texture "name", but will use the																			
	// path as the key instead.

	bool loadCubeMap(const std::string &name,
		const std::string &negativeXTexturePath,
		const std::string &positiveXTexturePath,
		const std::string &negativeYTexturePath,
		const std::string &positiveYTexturePath,
		const std::string &negativeZTexturePath,
		const std::string &positiveZTexturePath);
private:
	TextureManager();
	static std::unique_ptr<TextureManager> myInstance;
};