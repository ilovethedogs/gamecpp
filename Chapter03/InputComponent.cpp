// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "InputComponent.h"
#include "Actor.h"

InputComponent::InputComponent(class Actor* owner)
:MoveComponent(owner)
,mForwardKey(0)
,mBackKey(0)
,mClockwiseKey(0)
,mCounterClockwiseKey(0)
{
	
}

void InputComponent::ProcessInput(const uint8_t* keyState)
{
	// Calculate forward speed for MoveComponent
	if (keyState[mForwardKey])
	{
		Vector2 force{GetVelocity()};
		force += Vector2(0.0f, -1.0f) * Math::NearZero(force.Length());
		force.Normalize();
		AddForce(force);
	}
	if (keyState[mBackKey])
	{
		Vector2 force{ -1 * GetVelocity()};
		force += Vector2(0.0f, 1.0f) * Math::NearZero(force.Length());
		force.Normalize();
		AddForce(force);
	}

	// Calculate angular speed for MoveComponent
	if (keyState[mClockwiseKey])
	{
		Vector2 temp{ GetVelocity() };
		float len{ temp.Length() };
		temp += Vector2(1.0f, 0.0f) * Math::NearZero(len);
		Vector2 force{ temp.y, -temp.x };
		force.Normalize();
		force *= 7.5f;
		AddForce(force);
	}
	if (keyState[mCounterClockwiseKey])
	{
		Vector2 temp{ GetVelocity() };
		float len{ temp.Length() };
		temp += Vector2(1.0f, 0.0f) * Math::NearZero(len);
		Vector2 force{ -temp.y, temp.x };
		force.Normalize();
		force *= 7.5f;
		AddForce(force);
	}
}
