// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
	:mWindow(nullptr)
	, mRenderer(nullptr)
	, mTicksCount(0)
	, mIsRunning(true)
	, mPaddleDir0(0)
	, mPaddleDir1(0)
{

}

bool Game::Initialize(std::mt19937 rd, std::uniform_real_distribution<> ud)
{
	// Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Create an SDL Window
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)", // Window title
		100,	// Top left x-coordinate of window
		100,	// Top left y-coordinate of window
		1024,	// Width of window
		768,	// Height of window
		0		// Flags (0 for no flags set)
	);

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	//// Create SDL renderer
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		-1,		 // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	//
	mPaddlePos0.x = 10.0f;
	mPaddlePos0.y = 768.0f / 2.0f;
	mPaddlePos1.x = 1024.0f - 24.0f;
	mPaddlePos1.y = mPaddlePos0.y;

	for (int i = 0; i < 2; ++i) {
		float x_vel = ud(rd);
		float y_vel = ud(rd);
		x_vel < 0.0f ? x_vel -= 0.5f : x_vel += 0.5f;
		y_vel < 0.0f ? y_vel -= 0.5f : y_vel += 0.5f;
		Ball b{ 1024.0f / 2.0f, 768.f / 2.0f, 200.0f * x_vel, 235.0f * y_vel };
		mBalls.emplace_back(b);
	}
	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// If we get an SDL_QUIT event, end loop
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	// Get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	// Update paddle direction based on W/S keys
	mPaddleDir0 = 0;
	if (state[SDL_SCANCODE_W])
	{
		mPaddleDir0 -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddleDir0 += 1;
	}

	mPaddleDir1 = 0;
	if (state[SDL_SCANCODE_I])
	{
		mPaddleDir1 -= 1;
	}
	if (state[SDL_SCANCODE_K])
	{
		mPaddleDir1 += 1;
	}
}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// Delta time is the difference in ticks from last frame
	// (converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();

	// Update paddle position based on direction
	if (mPaddleDir0 != 0)
	{
		mPaddlePos0.y += mPaddleDir0 * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen!
		if (mPaddlePos0.y < (paddleH / 2.0f + thickness))
		{
			mPaddlePos0.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddlePos0.y > (768.0f - paddleH / 2.0f - thickness))
		{
			mPaddlePos0.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	if (mPaddleDir1 != 0)
	{
		mPaddlePos1.y += mPaddleDir1 * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen!
		if (mPaddlePos1.y < (paddleH / 2.0f + thickness))
		{
			mPaddlePos1.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddlePos1.y > (768.0f - paddleH / 2.0f - thickness))
		{
			mPaddlePos1.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	// Update ball position based on ball velocity
	for (auto& b : mBalls) {
		b.mBallPos.x += b.mBallVel.x * deltaTime;
		b.mBallPos.y += b.mBallVel.y * deltaTime;
	}

	for (auto& b : mBalls) {
		// Bounce if needed
		// Did we intersect with the paddle?
		float diff = mPaddlePos0.y - b.mBallPos.y;
		// Take absolute value of difference
		diff = (diff > 0.0f) ? diff : -diff;

		float diff1 = mPaddlePos1.y - b.mBallPos.y;
		diff1 = (diff1 > 0.0f) ? diff1 : -diff1;
		if (
			// Our y-difference is small enough
			(diff <= paddleH / 2.0f &&
				// We are in the correct x-position
				b.mBallPos.x <= 25.0f && b.mBallPos.x >= 20.0f &&
				// The ball is moving to the left
				b.mBallVel.x < 0.0f)
			||
			(diff1 <= paddleH / 2.0f &&
				// We are in the correct x-position
				b.mBallPos.x >= 990.0f && b.mBallPos.x <= 1000.0f &&
				// The ball is moving to the left
				b.mBallVel.x > 0.0f)
			)
		{
			b.mBallVel.x *= -1.0f;
		}
		// Did the ball go off the screen? (if so, end game)
		else if (b.mBallPos.x <= 0.0f || b.mBallPos.x >= 1024.0f)
		{
			mIsRunning = false;
		}
		// Did the ball collide with the right wall?
		/*
		else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f)
		{
			mBallVel.x *= -1.0f;
		}
		*/

		// Did the ball collide with the top wall?
		if (b.mBallPos.y <= thickness && b.mBallVel.y < 0.0f)
		{
			b.mBallVel.y *= -1;
		}
		// Did the ball collide with the bottom wall?
		else if (b.mBallPos.y >= (768 - thickness) &&
			b.mBallVel.y > 0.0f)
		{
			b.mBallVel.y *= -1;
		}
	}
}

void Game::GenerateOutput()
{
	// Set draw color to blue
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G 
		255,	// B
		255		// A
	);

	// Clear back buffer
	SDL_RenderClear(mRenderer);

	// Draw walls
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	// Draw top wall
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		1024,		// Width
		thickness	// Height
	};
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw bottom wall
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw right wall
	/*
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 1024;
	SDL_RenderFillRect(mRenderer, &wall);
	*/

	// Draw paddle
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos0.x),
		static_cast<int>(mPaddlePos0.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	paddle.x = static_cast<int>(mPaddlePos1.x);
	paddle.y = static_cast<int>(mPaddlePos1.y - paddleH / 2);
	SDL_RenderFillRect(mRenderer, &paddle);

	// Draw ball
	for (auto& b : mBalls) {
		SDL_Rect ball{
			static_cast<int>(b.mBallPos.x - thickness / 2),
			static_cast<int>(b.mBallPos.y - thickness / 2),
			thickness,
			thickness
		};
		SDL_RenderFillRect(mRenderer, &ball);
	}

	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}
