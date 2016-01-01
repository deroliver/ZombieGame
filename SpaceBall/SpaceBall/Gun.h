#pragma once

#include <glm/glm.hpp>

#include "Bullet.h"

#include <string>
#include <vector>

class Gun {
public:
	Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletSpeed, float bulletDamage);
	~Gun();

	void update(bool isMouseDown, const glm::vec2& pos, const glm::vec2& direction, std::vector<Bullet>& bullets, float deltaTime);


private:

	void fire(const glm::vec2& direction, const glm::vec2& pos, std::vector<Bullet>& bullets);

	std::string m_name;

	int m_fireRate; ///< Fire rate in terms of frames

	int m_bulletsPerShot; ///< How many bullets fired at a time

	float m_spread; ///< Accuracy

	float m_bulletSpeed;

	float m_bulletDamage;

	float m_frameCounter;
};

