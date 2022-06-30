#pragma once
#include <iostream>
#include <functional>
#include <fstream>
#include <vector>
#include <memory>
#include "SpriteComponent.h"

class TimeMapComponent : public SpriteComponent {	

public:
	void Update(float deltaTime) override;
	void Draw(SDL_Renderer* renderer) override;
	void SetTileTextures(const std::vector<SDL_Texture*>& textures);
protected:
	std::vector<std::vector<int>> mTiles;
};
