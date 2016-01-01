#pragma once

#include <gengine/Window.h>
#include <gengine/GLSLProgram.h>
#include <gengine/Camera2D.h>
#include <gengine/InputManager.h>
#include <gengine/SpriteBatch.h>
#include <gengine/SpriteFont.h>

#include "Level.h"
#include "Player.h"
#include "Bullet.h"

class Zombie;

enum class GameState {
	PLAY, EXIT
};

class MainGame
{
public:
    MainGame();
    ~MainGame();

    /// Runs the game
    void run();

private:
    /// Initializes the core systems
    void initSystems();

	// Initializes the level and sets up everything
	void initLevel();

    /// Initializes the shaders
    void initShaders();

    /// Main game loop for the program
    void gameLoop();

	/// Updates all agents
	void updateAgents(float deltaTime);

	/// Update all bullets
	void updateBullets(float deltaTime);

	/// Checks the victory condition
	void checkVictory();

    /// Handles input processing
    void processInput();

    /// Renders the game
    void drawGame();

	/// Draws the HUD
	void drawHud();

    /// Member Variables
    gengine::Window m_window; ///< The game window
    
    gengine::GLSLProgram m_textureProgram; ///< The shader program

    gengine::InputManager m_inputManager; ///< Handles input

    gengine::Camera2D m_camera; ///< Main Camera
	gengine::Camera2D m_hudCamera; ///< HUD Camera

	gengine::SpriteBatch m_agentSpriteBatch; ///< Draws all agents
	gengine::SpriteBatch m_hudSpriteBatch; ///< Draws the hud

	std::vector<Level*> m_levels; /// Vector of all levels

	int m_screenWidth = 1024; /// Screen width
	int m_screenHeight = 768; /// Screen height

	float m_fps; // The Frames Per Second

	int m_currentLevel; // The current level for the game

	Player* m_player;
	std::vector<Human*> m_humans; ///< Vector of all humans
	std::vector<Zombie*> m_zombies; ///< Vector of all zombies
	std::vector<Bullet> m_bullets;

	int m_numHumansKilled;
	int m_numZombiesKilled;

	gengine::SpriteFont* m_spriteFont;

	GameState m_gameState;
};

