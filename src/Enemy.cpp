#include "Enemy.h"

#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "PlayScene.h"
#include "TextureManager.h"
#include "Util.h"

Enemy::Enemy() : m_maxSpeed(1.0f), m_waypoint(0), m_pTarget(nullptr), m_state(ActionState::NO_STATE), m_ctr(0),
m_currentAnimationState(ENEMY_IDLE_DOWN)
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
	setType(AGENT);

	m_buildAnimations();

	setCurrentHeading(0.0f);// current facing angle
	setCurrentDirection(glm::vec2(1.0f, 0.0f)); // facing right
	m_turnRate = 5.0f; // 5 degrees per frame

	m_health = 40;

	// Collision
	setCOLDistance(60.0f);
	setCOLColour(glm::vec4(1, 0, 0, 1));

	// LOS
	setLOSDistance(200.0f); // 5 ppf x 80 feet
	setLOSColour(glm::vec4(1, 0, 0, 1));

	//Detection
	setDetectionDistance(300.0f);
	setDetectionColour(glm::vec4(1, 0, 0, 1));

	// Create the tree.
	m_pTree = new DecisionTree(this); // this is a pointer to the Enemy object
	// m_pTree->setAgent(this);
	//m_pTree->Display(); // Optional.
	m_patrol.reserve(m_patrol.size());

}

Enemy::~Enemy()
= default;

void Enemy::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the Enemy Animations

	switch (m_currentAnimationState)
	{
	case ENEMY_WALK_UP:
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("walkUp"),
			x, y, 0.1f, 0, 255, true);
		break;
	case ENEMY_WALK_DOWN:
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("walkDown"),
			x, y, 0.1f, 0, 255, true);
		break;
	case ENEMY_WALK_LEFT:
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("walkLeft"),
			x, y, 0.1f, 0, 255, true);
		break;
	case ENEMY_WALK_RIGHT:
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("walkRight"),
			x, y, 0.1f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	case ENEMY_IDLE_DOWN:
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("idleDown"),
			x, y, 0.02f, 0, 255, true);
		break;
	case ENEMY_ATTACK_UP:
		TextureManager::Instance().playAnimation("spritesheet", getAnimation("attackUp"),
			x, y, 0.1f, 0, 255, true);
		break;
	}

	// draw Collision Ray
	Util::DrawCircle(getTransform()->position,  getCOLDistance(), getCOLColour());

	// draw LOS
	Util::DrawLine(getTransform()->position, getTransform()->position + getCurrentDirection() * getLOSDistance(), getLOSColour());

	//draw Detection Radius
	Util::DrawCircle(getTransform()->position, getDetectionDistance(),getDetectionColour());

	// draw Health Bar
	glm::vec2 healthBarPosition = glm::vec2(getTransform()->position.x - 20.0f, getTransform()->position.y - 30.0f);
	Util::DrawFilledRect(healthBarPosition, getHealth(),10.0f, { 1,0,0,1 });
}


void Enemy::update()
{
	float angle_to_target;
	angle_to_target = Util::angle(getTransform()->position, m_pTarget->getTransform()->position);

	//std::cout << angle_to_target << std::endl;
	if (angle_to_target >=0 && angle_to_target <=45)
		setAnimationState(ENEMY_WALK_RIGHT);
	if (angle_to_target > 45 && angle_to_target <= 135)
		setAnimationState(ENEMY_WALK_DOWN);
	if (angle_to_target < -90.0f)
		setAnimationState(ENEMY_WALK_LEFT);

	getTree()->Update(); // Tell the ship what to do
	if(m_ctr++ % 60 == 0)
		getTree()->MakeDecision(); // Check for an new potential action
}

void Enemy::clean(){}

void Enemy::turnRight()
{
	setCurrentHeading(getCurrentHeading() + m_turnRate);
	if (getCurrentHeading() >= 360)
	{
		setCurrentHeading(getCurrentHeading() - 360.0f);
	}
}

void Enemy::turnLeft()
{
	setCurrentHeading(getCurrentHeading() - m_turnRate);
	if (getCurrentHeading() < 0)
	{
		setCurrentHeading(getCurrentHeading() + 360.0f);
	}
}

void Enemy::moveForward()
{
	getTransform()->position += getCurrentDirection() * m_maxSpeed;
	//getRigidBody()->velocity = getCurrentDirection() * m_maxSpeed;
}

void Enemy::moveBack()
{
	//getRigidBody()->velocity = getCurrentDirection() * -m_maxSpeed;
}

void Enemy::move()
{
	rotate();
	moveForward();
	/*getTransform()->position += getRigidBody()->velocity;
	getRigidBody()->velocity *= 0.9f;*/
}

void Enemy::wait()
{
	setAnimationState(ENEMY_IDLE_DOWN);
	
}

void Enemy::rotate()
{
	// Step 1: Find angle between two points
	// Util::angle(getTransform()->position, m_pPlayer->getTransform()->position)

	// Step 2: Plug angle into a lerp
	// Util::lerpAngle(getCurrentHeading(), <destination angle>, 0.1f)

	// Step 3: Set lerped angle into currentHeading
	// setCurrentHeading()

	setCurrentHeading(Util::lerpAngle(getCurrentHeading(), 
		Util::angle(getTransform()->position, m_pTarget->getTransform()->position), 0.1f));
}

void Enemy::rotateToTarget(DisplayObject* target)
{
	setCurrentHeading(Util::lerpAngle(getCurrentHeading(), Util::angle(getTransform()->position, target->getTransform()->position), 0.25f));
}


float Enemy::getMaxSpeed() const
{
	return m_maxSpeed;
}

int Enemy::getHealth() const
{
	return m_health;
}

void Enemy::setHealth(int newHealth)
{
	m_health = newHealth;
}

DisplayObject* Enemy::getTarget()
{
	return m_pTarget;
}

std::vector<DisplayObject*>& Enemy::getPatrol()
{
	return m_patrol;
}

DecisionTree* Enemy::getTree()
{
	return m_pTree;
}

void Enemy::setMaxSpeed(float newSpeed)
{
	m_maxSpeed = newSpeed;
}

void Enemy::setTarget(DisplayObject* target)
{
	m_pTarget = target;
}

void Enemy::setClosestLOSNode(PathNode* node)
{
	m_pLOSNode = node;
}

DisplayObject* Enemy::getCurrentWaypoint()
{
	return m_patrol[m_waypoint];
}

void Enemy::resumePatrol()
{
	setTarget(m_patrol[m_waypoint]); // m_waypoint is current PathNode to go to
	m_state = ActionState::PATROL;
}

void Enemy::setAnimationState(EnemyAnimationStates new_state)
{
	m_currentAnimationState = new_state;
}

void Enemy::m_buildAnimations()
{
	Animation walkUpAnimation = Animation();
	walkUpAnimation.name = "walkUp";
	walkUpAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-up-0"));
	walkUpAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-up-1"));
	walkUpAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-up-2"));
	walkUpAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-up-3"));
	setAnimation(walkUpAnimation);

	Animation walkDownAnimation = Animation();
	walkDownAnimation.name = "walkDown";
	walkDownAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-down-0"));
	walkDownAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-down-1"));
	walkDownAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-down-2"));
	walkDownAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-down-3"));
	setAnimation(walkDownAnimation);

	Animation walkLeftAnimation = Animation();
	walkLeftAnimation.name = "walkLeft";
	walkLeftAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-left-0"));
	walkLeftAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-left-1"));
	walkLeftAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-left-2"));
	walkLeftAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-left-3"));
	setAnimation(walkLeftAnimation);

	Animation walkRightAnimation = Animation();
	walkRightAnimation.name = "walkRight";
	walkRightAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-left-0"));
	walkRightAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-left-1"));
	walkRightAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-left-2"));
	walkRightAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-walk-left-3"));
	setAnimation(walkRightAnimation);

	Animation idleDownAnimation = Animation();
	idleDownAnimation.name = "idleDown";
	idleDownAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-idle-down-0"));
	idleDownAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-idle-down-1"));
	idleDownAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-idle-down-2"));
	idleDownAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-idle-down-3"));
	setAnimation(idleDownAnimation);

	Animation attackUpAnimation = Animation();
	attackUpAnimation.name = "attackUp";
	attackUpAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-attack-up-0"));
	attackUpAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-attack-up-1"));
	attackUpAnimation.frames.push_back(getSpriteSheet()->getFrame("blue-soldier-attack-up-2"));
	setAnimation(attackUpAnimation);
}

DisplayObject* Enemy::getNextWaypoint()
{
	m_waypoint++;
	if (m_waypoint == m_patrol.size()) m_waypoint = 0;
	setTarget(m_patrol[m_waypoint]);
	return m_pTarget;
}

PathNode* Enemy::getClosestLOSNode()
{
	return m_pLOSNode;
}

void Enemy::m_checkBounds()
{

	if (getTransform()->position.x > Config::SCREEN_WIDTH)
	{
		getTransform()->position = glm::vec2(0.0f, getTransform()->position.y);
	}

	if (getTransform()->position.x < 0)
	{
		getTransform()->position = glm::vec2(800.0f, getTransform()->position.y);
	}

	if (getTransform()->position.y > Config::SCREEN_HEIGHT)
	{
		getTransform()->position = glm::vec2(getTransform()->position.x, 0.0f);
	}

	if (getTransform()->position.y < 0)
	{
		getTransform()->position = glm::vec2(getTransform()->position.x, 600.0f);
	}

}

void Enemy::m_reset()
{
	getRigidBody()->isColliding = false;
	const int halfWidth = getWidth() * 0.5f;
	const auto xComponent = rand() % (640 - getWidth()) + halfWidth + 1;
	const auto yComponent = -getHeight();
	getTransform()->position = glm::vec2(xComponent, yComponent);
}