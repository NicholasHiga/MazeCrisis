#include "TextureManager.h"
#include "Image.h"

using std::string;
using std::unique_ptr;
using std::shared_ptr;

unique_ptr<TextureManager> TextureManager::myInstance = nullptr;

TextureManager::TextureManager()
{
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);

	// call this ONLY when linking with FreeImage as a static library
	#ifdef FREEIMAGE_LIB
		FreeImage_Initialise();
	#endif
}


TextureManager::~TextureManager()
{
	// call this ONLY when linking with FreeImage as a static library
	#ifdef FREEIMAGE_LIB
		FreeImage_DeInitialise();
	#endif
}

TextureManager*
TextureManager::getInstance()
{
	if (myInstance == nullptr)
		myInstance = unique_ptr<TextureManager>(new TextureManager());

	return myInstance.get();
}

bool 
TextureManager::loadTexture(const string &path, const string &name)
{
	try
	{
		if (get(name) == nullptr)
		{
			shared_ptr<Texture> texture = std::make_shared<Texture>();
			//GLenum error = GL_NO_ERROR;
			texture->paths.push_back(path);
			texture->images.push_back(std::make_shared<Image>(path));
			texture->textureType = TextureType::SINGLE;
			glGenTextures(1, &texture->textureID);

			glBindTexture(GL_TEXTURE_2D, texture->textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture->images[0]->getWidth(), texture->images[0]->getHeight(),
				0, GL_BGRA, GL_UNSIGNED_BYTE, texture->images[0]->getImageData());
			glGenerateMipmap(GL_TEXTURE_2D);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);

			//error = glGetError();

			//if (error != GL_NO_ERROR) // For some reason GL_INVALID_ENUM keeps being returned
										// even when there is no instance of this occuring.
										// Hence this was commented out.
			//	throw std::runtime_error("Image failed to load.");

			if (name == "")
				return store(path, texture);
			else
				return store(name, texture);
		}
	}
	catch (std::exception &e)
	{
		throw e;
	}
	return false;
}

bool 
TextureManager::loadCubeMap(const std::string &name,
	const std::string &negativeXTexturePath,
	const std::string &positiveXTexturePath,
	const std::string &negativeYTexturePath,
	const std::string &positiveYTexturePath,
	const std::string &negativeZTexturePath,
	const std::string &positiveZTexturePath)
{
	try
	{
		if (get(name) == nullptr)
		{
			shared_ptr<Texture> texture = std::make_shared<Texture>();
			texture->paths.push_back(negativeXTexturePath);
			texture->images.push_back(std::make_shared<Image>(negativeXTexturePath));
			texture->paths.push_back(positiveXTexturePath);
			texture->images.push_back(std::make_shared<Image>(positiveXTexturePath));
			texture->paths.push_back(negativeYTexturePath);
			texture->images.push_back(std::make_shared<Image>(negativeYTexturePath));
			texture->paths.push_back(positiveYTexturePath);
			texture->images.push_back(std::make_shared<Image>(positiveYTexturePath));
			texture->paths.push_back(negativeZTexturePath);
			texture->images.push_back(std::make_shared<Image>(negativeZTexturePath));
			texture->paths.push_back(positiveZTexturePath);
			texture->images.push_back(std::make_shared<Image>(positiveZTexturePath));

			texture->textureType = TextureType::CUBEMAP;
			glGenTextures(1, &texture->textureID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, texture->textureID);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			for (int i = 0; i < 6; ++i)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, texture->images[i]->getWidth(), texture->images[i]->getHeight(),
					0, GL_BGRA, GL_UNSIGNED_BYTE, texture->images[i]->getImageData());

			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

			return store(name, texture);
		}
	}
	catch (std::exception &e)
	{
		throw e;
	}
	return false;
}