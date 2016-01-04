#include "MainGame.h"

#include <glm/gtx/rotate_vector.hpp>

#include <gengine/gengine.h>
#include <gengine/Timing.h>
#include <gengine/GengineErrors.h>
#include <gengine/ResourceManager.h>
#include <gengine/Vertex.h>

#include <SDL/SDL.h>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <random>

#include "Zombie.h"
#include "Gun.h"

const float HUMANm_SPEED = 1.0f;
const float ZOMBIEm_SPEED = 1.3f;
const float PLAYERm_SPEED = 6.0f;

MainGame::MainGame() : m_screenWidth(1024), m_screenHeight(768), m_gameState(GameState::PLAY), m_fps(0), m_player(nullptr), m_numHumansKilled(0), m_numZombiesKilled(0) {
    // Empty
}

MainGame::~MainGame() {
	for (int i = 0; i < m_levels.size(); i++) {
		delete m_levels[i];
	}
}

void MainGame::run() {

	initSystems();

	initLevel();
		
	gengine::Music music =  m_audioEngine.loadMusic("Sound/scary.mp3");
	music.play(-1);

	gameLoop();
	
}

void MainGame::initSystems() {

	// Initialize our game engine
	gengine::init();

	// Initialize sound 
	m_audioEngine.init();

	// Create the window
	m_window.create("ZombieGame", m_screenWidth, m_screenHeight, 0);

	// Set the clear color -- The background color
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

	// Initialize the shaders
	initShaders();

	// Initialize the sprite batches
	m_agentSpriteBatch.init();
	m_hudSpriteBatch.init();

	// Initialize sprite font -- Font file path and font size
	m_spriteFont = new gengine::SpriteFont("Fonts/Horrorfind.ttf", 64);

	// Initialize the cameras
	m_camera.init(m_screenWidth, m_screenHeight);
	m_hudCamera.init(m_screenWidth, m_screenHeight);
	m_hudCamera.setPosition(glm::vec2(m_screenWidth / 2, m_screenHeight /2 ));

	// Initialize particles
	m_bloodParticleBatch = new gengine::ParticleBatch2D();
	m_bloodParticleBatch->init(1000, 0.05f, gengine::ResourceManager::getTexture("Textures/particle.png"), 
							   [](gengine::Particle2D& particle, float deltaTime) {	
			particle.position += particle.velocity * deltaTime;
			particle.color.a = (GLubyte)(particle.life * 255.0f);
		
	});

	m_sparksParticleBatch = new gengine::ParticleBatch2D();
	m_sparksParticleBatch->init(1000, 0.03f, gengine::ResourceManager::getTexture("Textures/particle.png"),
								[](gengine::Particle2D& particle, float deltaTime) {

		//static std::mt19937 randomEngine(time(nullptr));
		//static std::uniform_real_distribution<float> randRotate(360.0f, 360.0f);

		//particle.velocity = glm::rotate(particle.velocity, randRotate(randomEngine));

		particle.position += particle.velocity * deltaTime;
		particle.color.a = (GLubyte)(particle.life * 255.0f);

	});
	m_particleEngine.addParticleBatch(m_bloodParticleBatch);
	m_particleEngine.addParticleBatch(m_sparksParticleBatch);
}


void MainGame::initLevel() {
	// Level 1
	m_levels.push_back(new Level("Levels/level1.txt"));
	m_currentLevel = 0;

	m_player = new Player();
	m_player->init(PLAYERm_SPEED, m_levels[m_currentLevel]->getStartPlayerPosition(), &m_inputManager, &m_camera, &m_bullets);

	std::cout << "X position: " << m_levels[m_currentLevel]->getStartPlayerPosition().x << "   Y position: " << m_levels[m_currentLevel]->getStartPlayerPosition().y << std::endl;

	m_humans.push_back(m_player);

	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));

	std::uniform_int_distribution<int> randPosX(1, m_levels[m_currentLevel]->getWidth() - 1);
	std::uniform_int_distribution<int> randPosY(1, m_levels[m_currentLevel]->getHeight() - 1);

	for (int i = 0; i < m_levels[m_currentLevel]->getNumHumans(); i++) {
		m_humans.push_back(new Human);
		glm::vec2 pos(randPosX(randomEngine) * TILE_WIDTH, randPosY(randomEngine) * TILE_WIDTH);
		m_humans.back()->init(HUMANm_SPEED, pos);
	}

	// Add the zombies
	const std::vector<glm::vec2>& zombiePositions = m_levels[m_currentLevel]->getStartZombiePositions();
	for (int i = 0; i < zombiePositions.size(); i++) {
		m_zombies.push_back(new Zombie);
		m_zombies.back()->init(ZOMBIEm_SPEED, zombiePositions[i]);
	}

	// Set up the player's guns
	const float BULLETm_SPEED = 30;
	m_player->addGun(new Gun("Magnum", 30, 1, .1f, BULLETm_SPEED, 30, m_audioEngine.loadSoundEffect("Sound/shots/pistol.wav")));
	m_player->addGun(new Gun("Shotgun", 40, 20, .6f, BULLETm_SPEED, 4, m_audioEngine.loadSoundEffect("Sound/shots/shotgun.wav")));
	m_player->addGun(new Gun("MP5", 4, 1, .3f, BULLETm_SPEED, 15, m_audioEngine.loadSoundEffect("Sound/shots/cg1.wav")));
}

void MainGame::initShaders() {
    // Compile our color shader
    m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    m_textureProgram.addAttribute("vertexPosition");
    m_textureProgram.addAttribute("vertexColor");
    m_textureProgram.addAttribute("vertexUV");
    m_textureProgram.linkShaders();
}

void MainGame::gameLoop() {

	const float DESIREDm_FPS = 60.0f;
	const int MAXm_PHYSICSm_STEPS = 6;

	gengine::FpsLimiter fpsLimiter;
	fpsLimiter.setMaxFPS(60.0f);

	const float CAMERAm_SCALE = 1.0f / 3.0f;
	m_camera.setScale(CAMERAm_SCALE);

	const float MSm_PERm_SECOND = 1000;
	const float DESIREDm_FRAMETIME = MSm_PERm_SECOND / DESIREDm_FPS;
	const float MAXm_DELTAm_TIME = 1.0f;

	float previousTicks = SDL_GetTicks();

	// Main loop
	while (m_gameState == GameState::PLAY) {
		fpsLimiter.beginFrame();

		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - previousTicks;
		previousTicks = newTicks;
		float totalDeltaTime = frameTime / DESIREDm_FRAMETIME;

		checkVictory();

		m_inputManager.update();

		processInput();

		int i = 0;
		while (totalDeltaTime > 0.0f && i < MAXm_PHYSICSm_STEPS) {
			float deltaTime = std::min(totalDeltaTime, MAXm_DELTAm_TIME);
			updateAgents(deltaTime);
			updateBullets(deltaTime);
			m_particleEngine.update(totalDeltaTime);
			totalDeltaTime -= deltaTime;
			i++;
		}
	

		m_camera.setPosition(m_player->getPosition());
		m_camera.update();
		m_hudCamera.update();

		drawGame();

		m_fps = fpsLimiter.end();
		//std::cout << m_fps << std::endl;

	}
}

void MainGame::updateAgents(float deltaTime) {
	// Update all humans
	for (int i = 0; i < m_humans.size(); i++) {
		m_humans[i]->update(m_levels[m_currentLevel]->getLevelData(), m_humans, m_zombies, deltaTime);
	}

	// Don't forget to update zombies
	for (int i = 0; i < m_zombies.size(); i++) {
		m_zombies[i]->update(m_levels[m_currentLevel]->getLevelData(), m_humans, m_zombies, deltaTime);
	}


	// Update Zombie collisions
	for (int i = 0; i < m_zombies.size(); i++) {
		for (int j = i + 1; j < m_zombies.size(); j++) {
			m_zombies[i]->collideWithAgent(m_zombies[j]);
		}

		for (int j = 1; j < m_humans.size(); j++) {
			if (m_zombies[i]->collideWithAgent(m_humans[j])) {
				m_zombies.push_back(new Zombie);
				m_zombies.back()->init(ZOMBIEm_SPEED, m_humans[j]->getPosition());

				delete m_humans[j];
				m_humans[j] = m_humans.back();
				m_humans.pop_back();
			}
		}

		if (m_zombies[i]->collideWithAgent(m_player)) {
			gengine::fatalError("YOU LOSE");
		}
	}


	for (int i = 0; i < m_humans.size(); i++) {
		for (int j = i + 1; j < m_humans.size(); j++) {
			m_humans[i]->collideWithAgent(m_humans[j]);
		}
	}


	// Update Human collisions
	for (int i = 0; i < m_humans.size(); i++) {
		for (int j = i + 1; j < m_humans.size(); j++) {
			m_humans[i]->collideWithAgent(m_humans[j]);
		}
	}
}


void MainGame::updateBullets(float deltaTime) {
	for (int i = 0; i < m_bullets.size();) {
		if (m_bullets[i].update(m_levels[m_currentLevel]->getLevelData(), deltaTime)) {
			// Add sparks
			addSparks(m_bullets[i].getPosition(), m_bullets[i].getDirection(), 5);
			m_bullets[i] = m_bullets.back();
			m_bullets.pop_back();
		}
		else {
			i++;
		}
	}

	bool wasBulletRemoved;

	for (int i = 0; i < m_bullets.size(); i++) {
		wasBulletRemoved = false;
		for (int j = 0; j < m_zombies.size();) {
			if (m_bullets[i].collideWithAgent(m_zombies[j])) {
				// Add blood
				addBlood(m_bullets[i].getPosition(), 5);

				if (m_zombies[j]->applyDamage(m_bullets[i].getDamage())) {
					delete m_zombies[j];
					m_zombies[j] = m_zombies.back();
					m_zombies.pop_back();
					m_numZombiesKilled++;
				}
				else {
					j++;
				}

				m_bullets[i] = m_bullets.back();
				m_bullets.pop_back();
				wasBulletRemoved = true;
				i--;
				break;
			}
			else {
				j++;
			}
		}

		if (!wasBulletRemoved) {
			for (int j = 1; j < m_humans.size();) {
				if (m_bullets[i].collideWithAgent(m_humans[j])) {
					// Add blood
					addBlood(m_bullets[i].getPosition(), 5);

					if (m_humans[j]->applyDamage(m_bullets[i].getDamage())) {
						delete m_humans[j];
						m_humans[j] = m_humans.back();
						m_humans.pop_back();
						m_numHumansKilled++;
					}
					else {
						j++;
					}

					m_bullets[i] = m_bullets.back();
					m_bullets.pop_back();
					wasBulletRemoved = true;
					i--;
					break;
				}
				else {
					j++;
				}
			}
		}
	}
}


void MainGame::checkVictory() {
	if (m_zombies.empty()) {
		std::printf("**** You Win! \n You killed %d civilians and %d zombies. There are %d/%d civilians remaining.", m_numHumansKilled, m_numZombiesKilled, m_humans.size() -1, m_levels[m_currentLevel]->getNumHumans());
		gengine::fatalError("");
	}
}

void MainGame::processInput() {
    SDL_Event evnt;
    //Will keep looping until there are no more events to process
    while (SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
            case SDL_QUIT:
                // Exit the game here!
                break;
            case SDL_MOUSEMOTION:
                m_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
                break;
            case SDL_KEYDOWN:
                m_inputManager.pressKey(evnt.key.keysym.sym);
                break;
            case SDL_KEYUP:
                m_inputManager.releaseKey(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                m_inputManager.pressKey(evnt.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                m_inputManager.releaseKey(evnt.button.button);
                break;
        }
    }
}

void MainGame::drawGame() {
    // Set the base depth to 1.0
    glClearDepth(1.0);
    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	m_textureProgram.use();

	// Make sure the shader uses texture 0
	glActiveTexture(GL_TEXTURE0);

	// Make sure the shader uses texture 0
	GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
   
	// Grab the camera matrix
	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	// Draw the level
	m_levels[m_currentLevel]->draw();

	// Begin drawing agents
	m_agentSpriteBatch.begin();

	const glm::vec2 agentDims(AGENT_RADIUS * 2);

	// Draw the humans
	for (int i = 0; i < m_humans.size(); i++) {
		if (m_camera.isBoxInView(m_humans[i]->getPosition(), agentDims)) {
			m_humans[i]->draw(m_agentSpriteBatch);
		}		
	}

	// Draw the zombies
	for (int i = 0; i < m_zombies.size(); i++) {
		if (m_camera.isBoxInView(m_zombies[i]->getPosition(), agentDims)) {
			m_zombies[i]->draw(m_agentSpriteBatch);
		}
	}

	// Draw the bullets
	for (int i = 0; i < m_bullets.size(); i++) {
		m_bullets[i].draw(m_agentSpriteBatch);
	}

	m_agentSpriteBatch.end();

	m_agentSpriteBatch.renderBatch();

	// Render the particles
	m_particleEngine.draw(&m_agentSpriteBatch);

	// Draw the Heads Up Display
	drawHud();

	m_textureProgram.unuse();

    // Swap our buffer and draw everything to the screen!
    m_window.swapBuffer();
}


void MainGame::drawHud() {

	char buffer[256];

	glm::mat4 projectionMatrix = m_hudCamera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	m_hudSpriteBatch.begin();

	sprintf_s(buffer, "Num Humans: %d", m_humans.size());
	m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(0.0f, 0.0f),
					  glm::vec2(0.5f), 0.0f, gengine::ColorRGBA8(255, 255, 255, 255));
	

	sprintf_s(buffer, "Num Zombies: %d", m_zombies.size());
	m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(0.0, 36.0f),
		glm::vec2(0.5f), 0.0f, gengine::ColorRGBA8(0, 0, 0, 255));

	m_hudSpriteBatch.end();
	m_hudSpriteBatch.renderBatch();

}


void MainGame::addBlood(const glm::vec2& position, int numParticles) {

	static std::mt19937 randEngine(time(nullptr));
	static std::uniform_real_distribution<float> randAngle(0.0f, 360.0f);

	glm::vec2 vel(2.0f, 0.0f);
	gengine::ColorRGBA8 col(255, 0, 0, 255);

	for (int i = 0; i < numParticles; i++) {
		m_bloodParticleBatch->addParticle(position, glm::rotate(vel, randAngle(randEngine)), col, 35.0f);
	}
}


void MainGame::addSparks(const glm::vec2& position, const glm::vec2& direction, int numParticles) {

	float radAngle = std::atan2(direction.y, direction.x);
	float degAngle = (radAngle / M_PI) * 180.0f;

	
	float reverseAngle = degAngle + 180.0f;
	
	std::cout << "Reverse Angle: " << reverseAngle << std::endl;

	float first = (int)(reverseAngle - 20.0f) % 360;
	float second = (int)(reverseAngle + 20.0f) % 360;

	std::cout << "Reverse Angle - 20: " << first << std::endl;
	std::cout << "Reverse Angle + 20: " << second << std::endl;

	static std::mt19937 randEngine(time(nullptr));
	static std::uniform_real_distribution<float> randAngle(first, second);

	glm::vec2 vel(2.0f, 0.0f);
	gengine::ColorRGBA8 col(253, 200, 6, 255);

	for (int i = 0; i < numParticles; i++) {
		float randRotate = randAngle(randEngine);

		std::cout << randRotate << std::endl;

		m_sparksParticleBatch->addParticle(position, glm::rotate(vel, randRotate), col, 40.0f);
	}
}