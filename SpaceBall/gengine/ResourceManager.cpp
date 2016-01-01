#include "ResourceManager.h"

namespace gengine {

TextureCache ResourceManager::_textureCache;

GLTexture ResourceManager::getTexture(std::string filePath) {
	return _textureCache.getTexture(filePath);
}

}