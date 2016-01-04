#pragma once

#include <gengine/AudioEngine.h>
#include <gengine/ParticleEngine2D.h>
#include <gengine/ParticleBatch2D.h>

#include <glm/glm.hpp>

#include "Bullet.h"

#include <string>
#include <vector>

class Gun {
public:
	Gun(std::string name, int fireRate, int bulletsPerShot, 
		float spread, float bulletSpeed, float bulletDamage, gengine::SoundEffect fireEffect);
	~Gun();

	void update(bool isMouseDown, const glm::vec2& pos, const glm::vec2& direction, std::vector<Bullet>& bullets, float deltaTime);

	void addGunSmoke(const glm::vec2& position, const glm::vec2 direction, int numParticles);

	gengine::ParticleEngine2D m_particleEngine;
	gengine::ParticleBatch2D* m_smokeParticleBatch;

private:

	void fire(const glm::vec2& direction, const glm::vec2& pos, std::vector<Bullet>& bullets);

	gengine::SoundEffect m_fireEffect;

	std::string m_name;

	int m_fireRate; ///< Fire rate in terms of frames

	int m_bulletsPerShot; ///< How many bullets fired at a time

	float m_spread; ///< Accuracy

	float m_bulletSpeed;

	float m_bulletDamage;

	float m_frameCounter;

};

