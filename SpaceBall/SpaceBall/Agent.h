#pragma once

#include <gengine/SpriteBatch.h>
#include <glm/glm.hpp>

const float AGENT_WIDTH = 60;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;

class Zombie;
class Human;

class Agent {
public:
	Agent();
	virtual ~Agent();

	virtual void update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies, float deltaTime) = 0;

	bool collideWithLevel(const std::vector<std::string>& levelData);

	void draw(gengine::SpriteBatch& spriteBatch);

	bool applyDamage(float damage);

	glm::vec2 getPosition() const { return m_position; }

	bool Agent::collideWithAgent(Agent* agent);

protected:

	void checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePositions, float x, float y);

	void collideWithTile(glm::vec2 tilePosition);

	glm::vec2 m_position;
	gengine::ColorRGBA8 m_color;
	float m_speed;
	float m_health;

};

