#pragma once

#include <string>
#include <vector>

#include <gengine/SpriteBatch.h>

const int TILE_WIDTH = 64;


class Level {
public:
	// Load the level
	Level(const std::string fileName);
	~Level();

	void draw();

	// Getters
	int getWidth() const { return m_levelData[0].size(); }
	int getHeight() const { return m_levelData.size(); }
	int getNumHumans() const { return m_numHumans; }
	const std::vector<std::string>& getLevelData() const { return m_levelData; }
	glm::vec2 getStartPlayerPosition() const { return m_startPlayerPositions; };
	const std::vector<glm::vec2>& getStartZombiePositions() const { return m_zombieStartPositions; };

private:
	std::vector<std::string> m_levelData;
	int m_numHumans;
	gengine::SpriteBatch m_spriteBatch;

	glm::vec2 m_startPlayerPositions;
	std::vector<glm::vec2> m_zombieStartPositions;
};

