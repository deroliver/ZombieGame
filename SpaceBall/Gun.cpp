#include "Gun.h"

#include <glm/gtx/rotate_vector.hpp>

#include <ctime>
#include <random>

#include <gengine/ResourceManager.h>


Gun::Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletSpeed, float bulletDamage, gengine::SoundEffect fireEffect) :
	m_name(name), 
	m_fireRate(fireRate), 
	m_bulletsPerShot(bulletsPerShot), 
	m_spread(spread),
	m_bulletSpeed(bulletSpeed),
	m_bulletDamage(bulletDamage),
	m_frameCounter(0),
	m_fireEffect(fireEffect) {


	m_smokeParticleBatch = new gengine::ParticleBatch2D();
	m_smokeParticleBatch->init(1000, 0.05f, gengine::ResourceManager::getTexture("Textures/particle.png"),
		[](gengine::Particle2D& particle, float deltaTime) {
		particle.position += particle.velocity * deltaTime;
		particle.color.a = (GLubyte)(particle.life * 255.0f);

	});
	m_particleEngine.addParticleBatch(m_smokeParticleBatch);
}

Gun::~Gun() {
	 
}

void Gun::update(bool isMouseDown, const glm::vec2& pos, const glm::vec2& direction, std::vector<Bullet>& bullets, float deltaTime) {
	m_particleEngine.update(deltaTime);
	m_frameCounter += 1.0f * deltaTime;
	if (m_frameCounter >= m_fireRate && isMouseDown) {
		addGunSmoke(pos, direction, 15);
		fire(direction, pos, bullets);
		m_frameCounter = 0;
	}
}


void Gun::fire(const glm::vec2& direction, const glm::vec2& pos, std::vector<Bullet>& bullets) {

	static std::mt19937 randomEngine(time(nullptr));
	std::uniform_real_distribution<float> randRotate(-m_spread, m_spread);

	m_fireEffect.play();

	for (int i = 0; i < m_bulletsPerShot; i++) {
		bullets.emplace_back(pos, glm::rotate(direction, randRotate(randomEngine)), m_bulletDamage, m_bulletSpeed);
	}
}


void Gun::addGunSmoke(const glm::vec2& position, const glm::vec2 direction, int numParticles) {

	glm::vec2 normVec = glm::normalize(direction);

	float angle = std::atan2(normVec.y, normVec.x);
	float deg = (angle / M_PI) * 180.0f;

	static std::mt19937 randEngine(time(nullptr));
	static std::uniform_real_distribution<float> randAngle(deg - 20.0f, deg + 20.0f);

	glm::vec2 vel(2.0f, 0.0f);
	gengine::ColorRGBA8 col(170, 165, 165, 255);

	for (int i = 0; i < numParticles; i++) {
		m_smokeParticleBatch->addParticle(position, glm::rotate(vel, randAngle(randEngine)), col, 25.0f);
	}
}