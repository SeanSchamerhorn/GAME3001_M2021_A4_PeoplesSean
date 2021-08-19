#pragma once
#ifndef __SHIP__
#define __SHIP__

#include "TextureManager.h"
#include "DecisionTree.h"
#include "ActionState.h"
#include "PathNode.h"
#include <glm/vec4.hpp>
#include "EnemyAnimationStates.h"
#include "Sprite.h"

class Ship final : public Sprite
{
public:
	Ship();
	~Ship();

	// Inherited via GameObject
	void draw() override;
	void update() override;
	void clean() override;

	void turnRight();
	void turnLeft();
	void moveForward();
	void moveBack();

	void move();
	void wait();
	void rotate();
	void rotateToTarget(DisplayObject* target);

	// getters
	float getMaxSpeed() const;
	int getHealth() const;

	DisplayObject* getTarget();

	std::vector<DisplayObject*>& getPatrol();
	DecisionTree* getTree();
	DisplayObject* getCurrentWaypoint();
	DisplayObject* getNextWaypoint();
	PathNode* getClosestLOSNode();

	// setters
	void setMaxSpeed(float newSpeed);
	void setHealth(int newHealth);
	void setTarget(DisplayObject* target);
	void setClosestLOSNode(PathNode* node);
	void resumePatrol();

	void setAnimationState(EnemyAnimationStates new_state);

	ActionState m_state;


private:
	void m_checkBounds();
	void m_reset();

	// Animation stuff
	void m_buildAnimations();
	EnemyAnimationStates m_currentAnimationState;
	
	int m_health;
	float m_maxSpeed;
	float m_turnRate;

	

	// Decision Tree
	DecisionTree* m_pTree;

	std::vector<DisplayObject*> m_patrol;
	int m_waypoint;
	DisplayObject* m_pTarget;
	PathNode* m_pLOSNode;

	int m_ctr = 0;
};



#endif /* defined (__SHIP__) */

