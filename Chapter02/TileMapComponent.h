#pragma once
#include <iostream>
#include <functional>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <array>
#include "Actor.h"
#include "SpriteComponent.h"

constexpr size_t TILE{ 32 };
constexpr size_t TILE_WIDTH{ 32 };
constexpr size_t TILE_HEIGHT{ 24 };
// include the lf
constexpr size_t TILE_SIZE{ (TILE_WIDTH + 1) * TILE_HEIGHT };

class TileMapComponent : public SpriteComponent {	
public:
	TileMapComponent(class Actor* owner, int drawOrder = 10);

	void Draw(SDL_Renderer* renderer) override;
	void readTiles(char const* const filename);
protected:
	std::vector<std::array<int, TILE_SIZE>> mTiles;
};
