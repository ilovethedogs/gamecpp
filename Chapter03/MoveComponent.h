// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Component.h"
#include "Math.h"

class MoveComponent : public Component
{
public:
	// Lower update order to update first
	MoveComponent(class Actor* owner, int updateOrder = 10);

	void Update(float deltaTime) override;
	void AddForce(Vector2 force);
	
	float GetAngularSpeed() const { return mAngularSpeed; }
	Vector2 GetNetForce() const { return mNetForce; }
	Vector2 GetVelocity() const { return mVelocity; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	void SetVelocity(Vector2 velo) { mVelocity = velo; }
private:
	// Controls rotation (radians/second)
	float mAngularSpeed;
	// Newtonian Mechanics
	float mMass;
	Vector2 mNetForce;
	Vector2 mVelocity;
};
