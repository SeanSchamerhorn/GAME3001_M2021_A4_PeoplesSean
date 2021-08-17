#include "Obstacle.h"


#include "SoundManager.h"
#include "TextureManager.h"
#include "Util.h"

Obstacle::Obstacle()
{
	//TextureManager::Instance().load("../Assets/textures/obstacle.png", "obstacle");

	//auto size = TextureManager::Instance().getTextureSize("obstacle");
	setWidth(40.0f);
	setHeight(40.0f);

	getTransform()->position = glm::vec2(300.0f, 300.0f);

	setType(OBSTACLE);
	getRigidBody()->isColliding = false;

	//SoundManager::Instance().load("../Assets/audio/thunder.ogg", "thunder", SOUND_SFX);
}

Obstacle::~Obstacle()
= default;

void Obstacle::draw()
{
	/*TextureManager::Instance().draw("obstacle", getTransform()->position.x, getTransform()->position.y, 0, 255, true);*/
	Util::DrawCircle(getTransform()->position, 20.0f,glm::vec4(1,0,1,1));
}

void Obstacle::update()
{
}

void Obstacle::clean()
{
}
