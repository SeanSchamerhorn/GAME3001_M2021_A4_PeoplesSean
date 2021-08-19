#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "Transition.h"
#include "Attack.h"
#include "MoveToLOS.h"
#include "MoveToPlayer.h"
#include "Patrol.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	TextureManager::Instance().draw("background", 0, 0);
	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 255, 255, 255);
	
}

void PlayScene::update()
{
	
	m_mousePosition = EventManager::Instance().getMousePosition();
	float angle_to_target;
	angle_to_target = Util::angle(m_pPlayer->getTransform()->position, m_mousePosition);
	
	// Set three conditions of decision tree here.
	m_pEnemy->getTree()->getLOSNode()->setLOS(m_pEnemy->hasLOS()); // Or use m_pEnemy if you want to be lazy/wrong.

	m_pEnemy->getTree()->getRadiusNode()->setIsWithinRadius(
		Util::distance(m_pEnemy->getTransform()->position, m_pPlayer->getTransform()->position) <= m_pEnemy->getDetectionDistance());

	m_pEnemy->getTree()->getCloseCombatNode()->setIsWithinCombatRange(
		Util::distance(m_pEnemy->getTransform()->position, m_pPlayer->getTransform()->position) <= 50.0f);

	updateDisplayList();
	
	m_CheckAgentLOS(m_pEnemy, m_pPlayer);
	m_CheckPathNodeLOS(m_pPlayer);

	m_findClosestPathNode(m_pEnemy, m_pPlayer); // With LOS 

	m_CheckDetection(m_pPlayer);

	if(m_CheckCollision(m_pObstacle1))
	{
		m_pEnemy->setCurrentHeading(m_pEnemy->getCurrentHeading() + 4.0f);
	}
	if (m_pPlayer->getHealth() < 0) { Game::Instance().changeSceneState(END_SCENE); }
	// Color the patrol nodes...
	m_pGrid[81]->setLOSColour(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	m_pGrid[88]->setLOSColour(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	//m_pGrid[208]->setLOSColour(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	//m_pGrid[104]->setLOSColour(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance().quit();
	}
	if (EventManager::Instance().getMouseButton(LEFT))
	{
		m_mousePosition = EventManager::Instance().getMousePosition();
		float angle_to_target;
		angle_to_target = Util::angle(m_pPlayer->getTransform()->position, m_mousePosition);
		std::cout << "Bullet Direction ("<< m_mousePosition.x<<", "<<m_mousePosition.y<<")" << std::endl;
		EnemyBullet* will = new EnemyBullet();
		will->getTransform()->position = m_pPlayer->getTransform()->position;
		//will->setCurrentDirection(m_pPlayer->getCurrentDirection());
		will->setCurrentHeading(angle_to_target);
		SoundManager::Instance().setSoundVolume(5);
		SoundManager::Instance().playSound("flame", 0);
		std::cout << "BULLET DIRECTION (" <<will->getCurrentDirection().x << ", "<< will->getCurrentDirection().y<<")" << std::endl;
		addChild(will);
		m_pEbullets.push_back(will);
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	// Load in sound files
	SoundManager::Instance().allocateChannels(128);
	SoundManager::Instance().load("../Assets/audio/walking.wav", "footsteps", SOUND_SFX);
	SoundManager::Instance().setSoundVolume(20);
	SoundManager::Instance().load("../Assets/audio/flame.ogg", "flame", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/enemy_strike.ogg", "strike", SOUND_SFX);

	SoundManager::Instance().load("../Assets/audio/forest_main.ogg", "forest", SOUND_MUSIC);
	SoundManager::Instance().setMusicVolume(35);
	SoundManager::Instance().playMusic("forest");
	TextureManager::Instance().load("../Assets/textures/Forest_Tile_Map.png", "background");
	auto size = TextureManager::Instance().getTextureSize("background");
	setWidth(size.x);
	setHeight(size.y);

	m_buildGrid();

	// add the enemy to the scene as a start point
	m_pEnemy = new Enemy();
	m_pEnemy->getTransform()->position = m_pGrid[81]->getTransform()->position;
	addChild(m_pEnemy, 3);

	// Set the ship's patrol route.
	m_pEnemy->getPatrol().push_back(m_pGrid[81]);
	m_pEnemy->getPatrol().push_back(m_pGrid[88]);
	//m_pEnemy->getPatrol().push_back(m_pGrid[208]);
	//m_pEnemy->getPatrol().push_back(m_pGrid[104]);
	m_pEnemy->setTarget(m_pEnemy->getPatrol().front());


	// add the Obstacle to the scene as a start point
	m_pObstacle1 = new Obstacle();
	m_pObstacle1->getTransform()->position = m_pGrid[85]->getTransform()->position;
	addChild(m_pObstacle1);

	for (int i = 0; i < 6; i++)
	{
		Obstacle* obs = new Obstacle();
		obs->getTransform()->position = m_getTile(i + 1, 11)->getTransform()->position;
		obs->setGridPosition(i + 1, 11);
		m_Obstacles.push_back(obs);
		addChild(obs);
	}

	// add Blink to the scene
	m_pPlayer = new Blink();
	m_pPlayer->getTransform()->position = glm::vec2(700.f, 300.f);
	addChild(m_pPlayer);

	// Set some data onto the ship's decision tree's map. I only have the player/target for now.
	m_pEnemy->getTree()->setPlayer(m_pPlayer);
	m_pEnemy->getTree()->getMap().emplace("target", m_pPlayer);
	m_pEnemy->getTree()->MakeDecision(); // Or we won't go into patrol
	
	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function()
{
		
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("GAME3001 - M2021 - Lab 9", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	static bool gridVisible = true;
	if (ImGui::Checkbox("Toggle Grid", &gridVisible))
	{
		m_toggleGrid(gridVisible);
	}

	ImGui::Separator();

	// allow ship rotation
	static int angle;
	angle = m_pEnemy->getCurrentHeading(); // New.
	if (ImGui::SliderInt("Enemy Direction", &angle, -360, 360))
	{
		m_pEnemy->setCurrentHeading(angle);
	}

	ImGui::Separator();

	static int shipPosition[] = { m_pEnemy->getTransform()->position.x, m_pEnemy->getTransform()->position.y };
	if (ImGui::SliderInt2("Enemy Position", shipPosition, 0, 800))
	{
		m_pEnemy->getTransform()->position.x = shipPosition[0];
		m_pEnemy->getTransform()->position.y = shipPosition[1];
	}

	static int targetPosition[] = { m_pPlayer->getTransform()->position.x, m_pPlayer->getTransform()->position.y };
	if (ImGui::SliderInt2("Target Position", targetPosition, 0, 800))
	{
		m_pPlayer->getTransform()->position.x = targetPosition[0];
		m_pPlayer->getTransform()->position.y = targetPosition[1];
	}

	ImGui::Separator();
	
	ImGui::End();
}

void PlayScene::m_buildGrid()
{
	auto tileSize = Config::TILE_SIZE;

	// add path_nodes to the Grid
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			PathNode* path_node = new PathNode();
			path_node->getTransform()->position = glm::vec2(
				(col * tileSize) + tileSize * 0.5f, (row * tileSize) + tileSize * 0.5f);
			addChild(path_node);
			m_pGrid.push_back(path_node);
		}
	}
}

void PlayScene::m_toggleGrid(bool state)
{
	for (auto path_node : m_pGrid)
	{
		path_node->setVisible(state);
	}
}

PathNode* PlayScene::m_getTile(int col, int row)
{
	return m_pGrid[(row * Config::COL_NUM) + col];
}

PathNode* PlayScene::m_getTile(glm::vec2 grid_position)
{
	const auto col = grid_position.x;
	const auto row = grid_position.y;

	return m_pGrid[(row * Config::COL_NUM) + col];
}

bool PlayScene::m_CheckAgentLOS(Agent* agent, DisplayObject* target)
{
	bool hasLOS = false;
	agent->setHasLOS(hasLOS);
	// if ship to target distance is less than or equal to LOS Distance
	auto AgentToTargetDistance = Util::getClosestEdge(agent->getTransform()->position, target );
	if (AgentToTargetDistance <= agent->getLOSDistance())
	{
		std::vector<DisplayObject*> contactList;
		for (auto object : getDisplayList())
		{
			if (object->getType() == GameObjectType::OBSTACLE) // Only putting obstacles in contactList now.
			{
				// check if object is farther than than the target
				auto AgentToObjectDistance = Util::getClosestEdge(agent->getTransform()->position, object);
				if (AgentToObjectDistance <= AgentToTargetDistance)
					contactList.push_back(object);
			}
		}
		auto agentTarget = agent->getTransform()->position + agent->getCurrentDirection() * agent->getLOSDistance();
		auto hasLOS = CollisionManager::LOSCheck(agent, agentTarget, contactList, target);
		agent->setHasLOS(hasLOS);
	}
	return hasLOS;
}



void PlayScene::m_CheckPathNodeLOS(DisplayObject* target)
{
	for (auto path_node : m_pGrid)
	{
		auto targetDirection = target->getTransform()->position - path_node->getTransform()->position;
		auto normalizeDirection = Util::normalize(targetDirection);
		path_node->setCurrentDirection(normalizeDirection);
		m_CheckAgentLOS(path_node, target);
	}
}

PathNode* PlayScene::m_findClosestPathNode(Agent* agent, DisplayObject* target)
{
	auto min = FLT_MAX;
	PathNode* closestPathNode = nullptr;
	for (auto path_node : m_pGrid)
	{
		const auto distance = Util::distance(agent->getTransform()->position, path_node->getTransform()->position);
		if (distance < min && path_node->hasLOS() 
			&& (Util::distance(target->getTransform()->position, path_node->getTransform()->position) <= agent->getLOSDistance()))
		{
			min = distance;
			closestPathNode = path_node;
		}
	}
	closestPathNode->setLOSColour(glm::vec4(0.0f,0.0f,1.0f,1.0f));
	m_pEnemy->setClosestLOSNode(closestPathNode);
	return closestPathNode;
}

void PlayScene::m_CheckDetection(DisplayObject* target_object)
{
	m_pEnemy->setHasDetection(false);
	auto EnemyToTargetDistance = Util::distance(m_pEnemy->getTransform()->position, target_object->getTransform()->position);
	if (EnemyToTargetDistance <= m_pEnemy->getDetectionDistance())
	{
		m_pEnemy->setHasDetection(true);
	}
	
}

bool PlayScene::m_CheckCollision(DisplayObject* target)
{
	bool hasCOL = false;
	m_pEnemy->setHasCOL(hasCOL);
	auto EnemyToTargetDistance = Util::distance(m_pEnemy->getTransform()->position, target->getTransform()->position);
	if (EnemyToTargetDistance <= m_pEnemy->getCOLDistance())
	{
		hasCOL = true;
		m_pEnemy->setHasCOL(hasCOL);
	}
	return hasCOL;
}



