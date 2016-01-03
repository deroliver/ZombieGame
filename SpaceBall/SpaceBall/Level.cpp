#include "Level.h"

#include <fstream>
#include <iostream>

#include <gengine/GengineErrors.h>
#include <gengine/ResourceManager.h>


Level::Level(const std::string fileName){

	std::ifstream file;

	file.open(fileName);

	// Error checking
	if (file.fail()) {
		gengine::fatalError("Failed to open " + fileName);
	}

	// Throw away the first string in tmp
	std::string tmp;
	file >> tmp >> m_numHumans;

	std::getline(file, tmp);


	while (std::getline(file, tmp)) {
		m_levelData.push_back(tmp);
	}

	m_spriteBatch.init();
	m_spriteBatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	gengine::ColorRGBA8 whiteColor;

	whiteColor.r = 255;
	whiteColor.g = 255;
	whiteColor.b = 255;
	whiteColor.a = 255;

	// Render all the tiles
	for (int y = 0; y < m_levelData.size(); y++) {
		for (int x = 0; x < m_levelData[y].size(); x++) {

			char tile = m_levelData[y][x];

			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);
			
			switch (tile) {
				case 'B':
					m_spriteBatch.draw(destRect, uvRect, gengine::ResourceManager::getTexture("Textures/red_bricks.png").id, 0.0f, whiteColor);
					break;

				case 'R':
					m_spriteBatch.draw(destRect, uvRect, gengine::ResourceManager::getTexture("Textures/red_bricks.png").id, 0.0f, whiteColor);
					break;

				case 'G':
					m_spriteBatch.draw(destRect, uvRect, gengine::ResourceManager::getTexture("Textures/glass.png").id, 0.0f, whiteColor);
					break;

				case 'L':
					m_spriteBatch.draw(destRect, uvRect, gengine::ResourceManager::getTexture("Textures/light_bricks.png").id, 0.0f, whiteColor);
					break;

				case '@':
					m_levelData[y][x] = '.';
					m_startPlayerPositions.x = x * TILE_WIDTH;
					m_startPlayerPositions.y = y * TILE_WIDTH;
					break;

				case 'Z':
					m_levelData[y][x] = '.';
					m_zombieStartPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
					break;

				case '.':
					break;

				default:
					std::printf("Unexpected symbol %c at (%d, %d)", tile, x, y);
					break;

			}
		}
	}


	m_spriteBatch.end();
}


Level::~Level() {

}

void Level::draw() {
	m_spriteBatch.renderBatch();
}
