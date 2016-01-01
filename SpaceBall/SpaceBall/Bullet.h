#pragma once

#include <gengine/SpriteBatch.h>

#include <glm/glm.hpp>

#include<vector>
#include <string>

class Human;
class Zombie;
class Agent;

const int BULLET_RADIUS = 5;

class Bullet {

public:
	Bullet(glm::vec2 pos, glm::vec2 dir, float damage, float speed);
	~Bullet();

	// Update returns true, delete bullet
	bool update(const std::vector<std::string>& levelData, float deltatime);

	void draw(gengine::SpriteBatch& spriteBatch);

	bool collideWithAgent(Agent* agent);

	float getDamage() { return m_damage; }

private:

	bool collideWithWorld(const std::vector<std::string>& levelData);

	float m_damage;
	glm::vec2 m_position;
	glm::vec2 m_direction;
	float m_speed;
	
};

