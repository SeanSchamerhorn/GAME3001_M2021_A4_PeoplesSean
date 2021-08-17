#include "EnemyBullet.h"

#include "TextureManager.h"

EnemyBullet::EnemyBullet():m_currentAnimationState(BULLET_UP),m_maxSpeed(10.0f)
{
	TextureManager::Instance().loadSpriteSheet(
		"../Assets/sprites/atlas.txt",
		"../Assets/sprites/BlueLinkAtlas.png",
		"spritesheet");

	setSpriteSheet(TextureManager::Instance().getSpriteSheet("spritesheet"));

	// set frame width
	setWidth(40);

	// set frame height
	setHeight(40);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(ENEMY_BULLET);

	m_buildAnimations();
}

EnemyBullet::~EnemyBullet()
= default;


void EnemyBullet::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	switch (m_currentAnimationState)
	{
	case BULLET_UP:
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("bulletUp"),
			x, y, 0.5f, 0, 255, true);
		break;
	}
}

void EnemyBullet::update()
{
	getTransform()->position += getCurrentDirection() * m_maxSpeed;
}

void EnemyBullet::clean()
{
}

void EnemyBullet::setAnimationState(BulletAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void EnemyBullet::m_buildAnimations()
{
	Animation bulletAnimation = Animation();
	bulletAnimation.name = "bulletUp";
	bulletAnimation.frames.push_back(getSpriteSheet()->getFrame("link-bullet-0"));
	bulletAnimation.frames.push_back(getSpriteSheet()->getFrame("link-bullet-1"));
	bulletAnimation.frames.push_back(getSpriteSheet()->getFrame("link-bullet-2"));
	bulletAnimation.frames.push_back(getSpriteSheet()->getFrame("link-bullet-3"));

	setAnimation(bulletAnimation);
}
