#pragma once

#include "Agent.h"

class Zombie : public Agent {	
public:

	void init(float speed, glm::vec2 position);

	virtual void update(const std::vector<std::string>& levelData, std::vector<Human*>& humans, std::vector<Zombie*>& zombies, float deltatime) override;

	Zombie();
	~Zombie();

private:
	Human* getNearestHuman(std::vector<Human*>& humans);
};

