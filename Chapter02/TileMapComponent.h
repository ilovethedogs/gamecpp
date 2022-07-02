#pragma once
#include <iostream>
#include <functional>
#include <fstream>
#include <vector>
#include <memory>
#include "SpriteComponent.h"

class TileMapComponent : public SpriteComponent {	
public:
	void Draw(SDL_Renderer* renderer) override;
	void SetTileMapTexture(SDL_Texture* texture);
protected:
	std::vector<std::vector<int>> mTiles;
};
