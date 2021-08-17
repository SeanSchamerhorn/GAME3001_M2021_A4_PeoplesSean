#pragma once
#ifndef __BLINK__
#define __BLINK__

#include "TextureManager.h"
#include <glm/vec4.hpp>
#include "BlinkAnimationState.h"
#include "Sprite.h"

class Blink final : public Sprite
{
public:
	Blink();
	~Blink();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void turnRight();
	void turnLeft();
	void moveForward();
	void moveBack();

	void move();
	void rotate();

	//getters
	float getMaxSpeed() const;
	int getHealth() const;

	// setters
	void setMaxSpeed(float newSpeed);
	void setHealth(int newHealth);
	void setAnimationState(BlinkAnimationState new_state);

private:
	void m_buildAnimations();

	int m_health;
	float m_maxSpeed;
	float m_turnRate;

	BlinkAnimationState m_currentAnimationState;
};

#endif /* defined (__BLINK__) */
