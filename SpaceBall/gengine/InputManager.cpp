#include "InputManager.h"


namespace gengine {

InputManager::InputManager() : _mouseCoords(0.0f) {

}



InputManager::~InputManager() {

}


void InputManager::update() {
	// Loop through key may using for each and copy to previous key map
	for (auto& it : _keyMap) {
		_previousKeyMap[it.first] = it.second;
	}
}

void InputManager::pressKey(unsigned int keyID) {
	_keyMap[keyID] = true;
}


void InputManager::releaseKey(unsigned int keyID) {
	_keyMap[keyID] = false;
}


bool InputManager::isKeyDown(unsigned int keyID) {
	auto it = _keyMap.find(keyID);

	if (it != _keyMap.end()) {
		return it->second;
	}
	else {
		return false;
	}
}


bool InputManager::isKeyPressed(unsigned int keyID) {
	if (isKeyDown(keyID) && !wasKeyDown(keyID)) {
		return true;
	}

	return false;
}

void InputManager::setMouseCoords(float x, float y) {
	_mouseCoords.x = x;
	_mouseCoords.y = y;
}


bool InputManager::wasKeyDown(unsigned int keyID) {
	auto it = _previousKeyMap.find(keyID);

	if (it != _previousKeyMap.end()) {
		return it->second;
	}
	else {
		return false;
	}
}

}