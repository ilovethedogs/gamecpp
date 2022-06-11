// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"

int main(int argc, char** argv)
{
	std::random_device rd{};
	auto mtgen = std::mt19937{ rd() };
	auto ud = std::uniform_real_distribution<>{ -0.5, 0.5 };
	Game game;
	bool success = game.Initialize(mtgen, ud);
	if (success)
	{
		game.RunLoop();
	}
	game.Shutdown();
	return 0;
}
