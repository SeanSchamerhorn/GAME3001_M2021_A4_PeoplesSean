#pragma once
#ifndef __ENEMY_BULLET__
#define __ENEMY_BULLET__

#include "Sprite.h"
#include "BulletAnimationState.h"
class EnemyBullet : public Sprite
{
public:
	// Constructor
	EnemyBullet();

	// Destructor
	~EnemyBullet();

	// Life Cycle functions Inherited from DisplayObject
	void draw() override;
	void update() override;
	void clean() override;

	void setAnimationState(BulletAnimationState new_state);


private:
	void m_buildAnimations();

	float m_maxSpeed;

	BulletAnimationState m_currentAnimationState;
};
#endif