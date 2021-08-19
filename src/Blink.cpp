#include "Blink.h"

#include "EventManager.h"
#include "Game.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include "Util.h"

Blink::Blink():m_maxSpeed(2.0f),m_health(40),m_currentAnimationState(BLINK_IDLE_DOWN)
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
	setType(PLAYER);

	m_buildAnimations();

	setCurrentHeading(0.0f);// current facing angle
	setCurrentDirection(glm::vec2(1.0f, 0.0f)); // facing right
	m_turnRate = 5.0f; // 5 degrees per frame

	// LOS
	setLOSDistance(400.0f); // 5 ppf x 80 feet
	setLOSColour(glm::vec4(1, 0, 0, 1));

	//Detection
	setDetectionDistance(300.0f);
	setDetectionColour(glm::vec4(1, 0, 0, 1));
}

Blink::~Blink()
= default;

void Blink::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the player according to animation state
	switch (m_currentAnimationState)
	{
	case BLINK_IDLE_DOWN:
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("idle"),
			x, y, 0.03f, 0, 255, true);
		break;
	case BLINK_CLOSE_ATTACK:
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("swing"),
			x, y, 1.0f, 0, 255, true);
		break;
	case BLINK_RUN_RIGHT:
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("run"),
			x, y, 0.5f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	case BLINK_RUN_LEFT:
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("run"),
			x, y, 0.5f, 0, 255, true);
		break;
	case BLINK_RUN_DOWN:
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("runDown"),
			x, y, 0.5f, 0, 255, true);
		break;
	case BLINK_RUN_UP:
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("runUp"),
			x, y, 0.5f, 0, 255, true);
		break;
	default:
		break;
	}

	// draw LOS
	Util::DrawLine(getTransform()->position, getTransform()->position + getCurrentDirection() * getLOSDistance(), getLOSColour());


	// draw Detection Radius
	Util::DrawCircle(getTransform()->position, getDetectionDistance(), getDetectionColour());

	// draw Health Bar
	glm::vec2 healthBarPosition = glm::vec2(getTransform()->position.x - 20.0f, getTransform()->position.y - 30.0f);
	Util::DrawFilledRect(healthBarPosition, getHealth(), 10.0f, { 1,0,0,1 });
	
}

void Blink::update()
{
	bool is_playing = false;
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_W))
	{
		if (Game::Instance().getFrames() % 17 == 0)
		{
			SoundManager::Instance().playSound("footsteps", 0);
			is_playing = true;
		}
			moveForward();
	}
	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
	{
		setAnimationState(BLINK_RUN_LEFT);
		turnLeft();
	}
	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
	{
		setAnimationState(BLINK_RUN_RIGHT);
		turnRight();
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_S))
	{
		moveBack();
	}
	if(EventManager::Instance().getMouseButton(RIGHT))
	{
		setAnimationState(BLINK_CLOSE_ATTACK);
	}
	
}

void Blink::clean()
{
}

void Blink::turnRight()
{
	setCurrentHeading(getCurrentHeading() + m_turnRate);
	if (getCurrentHeading() >= 360)
	{
		setCurrentHeading(getCurrentHeading() - 360.0f);
	}
}

void Blink::turnLeft()
{
	setCurrentHeading(getCurrentHeading() - m_turnRate);
	if (getCurrentHeading() < 0)
	{
		setCurrentHeading(getCurrentHeading() + 360.0f);
	}
}

void Blink::moveForward()
{
	getTransform()->position += getCurrentDirection() * m_maxSpeed;	
}

void Blink::moveBack()
{
	getTransform()->position += getCurrentDirection() * -m_maxSpeed;
}

void Blink::move()
{
	rotate();
	moveForward();
}

void Blink::rotate()
{

	// Step 1: Find angle between two points
	// Util::angle(getTransform()->position, m_pTarget->getTransform()->position)

	// Step 2: Plug angle into a lerp
	// Util::lerpAngle(getCurrentHeading(), <destination angle>, 0.1f)

	// Step 3: Set lerped angle into currentHeading
	// setCurrentHeading()

	setCurrentHeading(Util::lerpAngle(getCurrentHeading(),
		Util::angle(getTransform()->position, getTargetPosition()), 0.1f));
}

float Blink::getMaxSpeed() const
{
	return m_maxSpeed;
}

int Blink::getHealth() const
{
	return m_health;
}

void Blink::setMaxSpeed(float newSpeed)
{
	m_maxSpeed = newSpeed;
}

void Blink::setHealth(int newHealth)
{
	m_health = newHealth;
}

void Blink::setAnimationState(BlinkAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void Blink::m_buildAnimations()
{
	Animation runDownAnimation = Animation();

	runDownAnimation.name = "runDown";
	runDownAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-down-0"));
	runDownAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-down-1"));
	runDownAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-down-2"));
	runDownAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-down-3"));
	runDownAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-down-4"));
	runDownAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-down-5"));

	setAnimation(runDownAnimation);


	Animation runUpAnimation = Animation();

	runUpAnimation.name = "runUp";
	runUpAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-up-0"));
	runUpAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-up-1"));
	runUpAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-up-2"));
	runUpAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-up-3"));
	runUpAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-up-4"));
	runUpAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-up-5"));

	setAnimation(runUpAnimation);

	Animation idleAnimation = Animation();

	idleAnimation.name = "idle";
	//idleAnimation.frames.push_back(getSpriteSheet()->getFrame("link-idle-0"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("link-idle-1"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("link-idle-2"));


	setAnimation(idleAnimation);

	Animation runAnimation = Animation();

	runAnimation.name = "run";
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-0"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-1"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-2"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-3"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-4"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("link-run-5"));

	setAnimation(runAnimation);

	Animation swingAnimation = Animation();

	swingAnimation.name = "swing";
	swingAnimation.frames.push_back(getSpriteSheet()->getFrame("link-swing-0"));
	swingAnimation.frames.push_back(getSpriteSheet()->getFrame("link-swing-1"));
	swingAnimation.frames.push_back(getSpriteSheet()->getFrame("link-swing-2"));
	swingAnimation.frames.push_back(getSpriteSheet()->getFrame("link-swing-3"));
	swingAnimation.frames.push_back(getSpriteSheet()->getFrame("link-swing-4"));
	swingAnimation.frames.push_back(getSpriteSheet()->getFrame("link-swing-5"));
	swingAnimation.frames.push_back(getSpriteSheet()->getFrame("link-swing-6"));
	swingAnimation.frames.push_back(getSpriteSheet()->getFrame("link-swing-7"));

	setAnimation(swingAnimation);
}
