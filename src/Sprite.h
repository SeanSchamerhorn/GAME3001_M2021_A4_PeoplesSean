#pragma once
#ifndef __SPRITE__
#define __SPRITE__

#include "Animation.h"
#include <unordered_map>

#include "Agent.h"
#include "SpriteSheet.h"

class Sprite : public Agent
{
public:
	Sprite();
	virtual ~Sprite();
	
	// Life Cycle Functions
	virtual void draw() = 0;
	virtual void update() = 0;
	virtual void clean() = 0;

	// getters
	SpriteSheet* getSpriteSheet();
	Animation& getAnimation(const std::string& name);
	
	// setters
	void setSpriteSheet(SpriteSheet* sprite_sheet);
	void setAnimation(const Animation& animation);
private:
	// private utility functions
	bool m_animationExists(const std::string& id);

	SpriteSheet* m_pSpriteSheet;

	std::unordered_map<std::string, Animation> m_pAnimations;
};

#endif /* defined (__SPRITE__) */