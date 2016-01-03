#include "TextureCache.h"
#include "ImageLoader.h"

#include <iostream>

namespace gengine {

TextureCache::TextureCache()
{
}


TextureCache::~TextureCache()
{
}


GLTexture TextureCache::getTexture(std::string filePath) {

	auto mit = _textureMap.find(filePath);

	if (mit == _textureMap.end()) {
		GLTexture newTexture = ImageLoader::loadPNG(filePath);

		_textureMap.insert(make_pair(filePath, newTexture));

		std::cout << "Loaded Texture!\n" << std::endl;
		return newTexture;
	}

	//std::cout << "Used Texture Cache!\n" << std::endl;

	return mit->second;
}

}
