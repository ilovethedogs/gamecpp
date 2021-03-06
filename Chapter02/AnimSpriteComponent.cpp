// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "AnimSpriteComponent.h"
#include "Math.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mAnimFPS(24.0f)
	, mCurAnim(0)
{
}

void AnimSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	if (mAnimTextures.size() > 0)
	{
		// Update the current frame based on frame rate
		// and delta time
		auto change = mAnimFPS * deltaTime;
		mCurrFrame += change;
		
		// Wrap current frame if needed
		size_t l_bound{ mAnimTextureList[mCurAnim].range.first };
		size_t r_bound{ mAnimTextureList[mCurAnim].range.second };
		if (mAnimTextureList[mCurAnim].circulation) {
			if (mCurrFrame >= r_bound) mCurrFrame = l_bound;
		}
		else {
			mCurrFrame -= change * (mCurrFrame >= r_bound);
		}

		// Set the current texture
		SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures, const std::vector<struct AnimTexture>& textureRanges)
{
	mAnimTextures = textures;
	mAnimTextureList = textureRanges;
	if (mAnimTextures.size() > 0)
	{
		// Set the active texture to first frame
		mCurrFrame = 0.0f;
		SetTexture(mAnimTextures[0]);
	}
}

void AnimSpriteComponent::SelectAnim(size_t n) {
	if (n >= 0 && n < mAnimTextureList.size())
		mCurAnim = n;
}
