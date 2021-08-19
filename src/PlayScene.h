#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Blink.h"
#include "Scene.h"
#include "Button.h"
#include "EnemyBullet.h"
#include "Label.h"
#include "Obstacle.h"
#include "Enemy.h"
#include "PathNode.h"
#include "Target.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

	//void moveStarShip() const;
private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	glm::vec2 m_mousePosition;
	bool m_isGridEnabled;

	// Game Objects
	Blink* m_pPlayer;
	Enemy* m_pEnemy;

	std::vector<EnemyBullet*> m_pEbullets;

	std::vector<Obstacle*> m_Obstacles;
	Obstacle* m_pObstacle1;
	Obstacle* m_pObstacle2;
	Obstacle* m_pObstacle3;

	// Other
	std::vector<PathNode*> m_pGrid;
	void m_buildGrid();

	// LOS
	bool m_CheckAgentLOS(Agent* agent, DisplayObject* target);
	void m_CheckPathNodeLOS(DisplayObject* target);
	PathNode* m_findClosestPathNode(Agent* agent, DisplayObject* target);

	// Detection
	void m_CheckDetection(DisplayObject* target_object);

	// Collision
	bool m_CheckCollision(DisplayObject* target);
	
	// UI functions
	void m_toggleGrid(bool state);


	// Check Collision
	
	// convenience functions
	PathNode* m_getTile(int col, int row);
	PathNode* m_getTile(glm::vec2 grid_position);
};

#endif /* defined (__PLAY_SCENE__) */