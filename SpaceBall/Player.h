#pragma once

#include "Human.h"
#include "Bullet.h"

#include <gengine/InputManager.h>
#include <gengine/Camera2D.h>


class Gun;

class Player : public Human { 
public:
	Player();
	~Player();

	void init(float speed, glm::vec2 position, gengine::InputManager* inputManager, gengine::Camera2D* camera, std::vector<Bullet>* bullets);

	void addGun(Gun* gun);

	void update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies, float deltaTime) override;

private:
	gengine::InputManager* m_inputManager;

	std::vector<Gun*> m_guns;
	int m_currentGunIndex;

	gengine::Camera2D* m_camera;
	std::vector<Bullet>* m_bullets;
};

