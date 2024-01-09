#pragma once
#include "Color.h"
#include "Environment.h"
#include "MathUtils.h"
#include <SDL.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

class GameOfLife : public Environment
{
public:
	GameOfLife(int width, int height, std::shared_ptr<class Texture> texture) : Environment(width, height, texture) {}

	bool Initialize() override;
	void Step() override;

	// input
	void KeyDown(SDL_Keycode keycode) override;
	
protected:
	//change
	int frame{ 0 };
	std::vector<uint8_t> bufferA;
	std::vector<uint8_t> bufferB;
};