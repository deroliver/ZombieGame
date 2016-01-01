#include "Bullet.h"
#include <gengine/ResourceManager.h>

#include "Agent.h"
#include "Human.h"
#include "Zombie.h"
#include "Level.h"

Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float damage, float speed) : 
	m_position(pos), 
	m_direction(dir), 
	m_damage(damage), 
	m_speed(speed) {
	// Empty
}


Bullet::~Bullet() {

}


bool Bullet::update(const std::vector<std::string>& levelData, float deltatime) {

	m_position += m_direction * m_speed * deltatime;

	return collideWithWorld(levelData);
}

void Bullet::draw(gengine::SpriteBatch& spriteBatch) {
	glm::vec4 destRect(m_position.x + BULLET_RADIUS, m_position.y + BULLET_RADIUS, BULLET_RADIUS * 2, BULLET_RADIUS * 2);

	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	gengine::ColorRGBA8 color;

	color.r = 75;
	color.g = 75;
	color.b = 75;
	color.a = 255;

	spriteBatch.draw(destRect, uvRect, gengine::ResourceManager::getTexture("Textures/circle.png").id, 0.0f, color);
}


bool Bullet::collideWithAgent(Agent* agent) {
	// Minimum distance before there is a collision
	const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;

	// Center position of this agent
	glm::vec2 centerPosA = m_position;
	// Center position of the parameter agent
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(BULLET_RADIUS);

	// Distance vector between the two agents
	glm::vec2 distVec = centerPosA - centerPosB;

	// Length of the distance vector
	float distance = glm::length(distVec);

	// Depth of the collision
	float collisionDepth = MIN_DISTANCE - distance;

	// If collision depth > 0 then we did collide
	if (collisionDepth > 0) {

		return true;
	}
	return false;
}


bool Bullet::collideWithWorld(const std::vector<std::string>& levelData) {
	glm::ivec2 gridPosition;
	gridPosition.x = floor(m_position.x / (float)TILE_WIDTH);
	gridPosition.y = floor(m_position.y / (float)TILE_WIDTH);

	// If we are outside the world, just return
	if (gridPosition.x < 0 || gridPosition.x >= levelData[0].size() ||
		gridPosition.y < 0 || gridPosition.y >= levelData.size()) {
		return true;
	}

	return (levelData[gridPosition.y][gridPosition.x] != '.');
}