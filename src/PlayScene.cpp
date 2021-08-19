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
	//m_pTarget->setTargetPosition(m_mousePosition);
	
	// Set three conditions of decision tree here.
	m_pShip->getTree()->getLOSNode()->setLOS(m_pShip->hasLOS()); // Or use m_pShip if you want to be lazy/wrong.

	m_pShip->getTree()->getRadiusNode()->setIsWithinRadius(
		Util::distance(m_pShip->getTransform()->position, m_pTarget->getTransform()->position) <= m_pShip->getDetectionDistance());

	m_pShip->getTree()->getCloseCombatNode()->setIsWithinCombatRange(
		Util::distance(m_pShip->getTransform()->position, m_pTarget->getTransform()->position) <= 60.0f);

	updateDisplayList();
	
	m_CheckAgentLOS(m_pShip, m_pTarget);
	m_CheckPathNodeLOS(m_pTarget);

	m_findClosestPathNode(m_pShip, m_pTarget); // With LOS 

	m_CheckDetection(m_pTarget);

	// Color the patrol nodes...
	m_pGrid[81]->setLOSColour(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	m_pGrid[88]->setLOSColour(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	//m_pGrid[208]->setLOSColour(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	//m_pGrid[104]->setLOSColour(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	//if (m_pShip->hasLOS())
	//{
	//	EnemyBullet* bill = new EnemyBullet();
	//	bill->getTransform()->position = m_pShip->getTransform()->position;
	//	bill->setCurrentDirection(m_pShip->getCurrentDirection());
	//	addChild(bill);
	//	m_pEbullets.push_back(bill);
	//}
	//
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
		EnemyBullet* will = new EnemyBullet();
		will->getTransform()->position = m_pTarget->getTransform()->position;
		will->setCurrentDirection(m_pTarget->getCurrentDirection());
		SoundManager::Instance().playSound("flame", 0);
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
	SoundManager::Instance().load("../Assets/audio/flamethrower.ogg", "flame", SOUND_SFX);
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
	m_pShip = new Ship();
	m_pShip->getTransform()->position = m_pGrid[81]->getTransform()->position;
	addChild(m_pShip, 3);

	// Set the ship's patrol route.
	m_pShip->getPatrol().push_back(m_pGrid[81]);
	m_pShip->getPatrol().push_back(m_pGrid[88]);
	//m_pShip->getPatrol().push_back(m_pGrid[208]);
	//m_pShip->getPatrol().push_back(m_pGrid[104]);
	m_pShip->setTarget(m_pShip->getPatrol().front());


	//// add the Obstacle to the scene as a start point
	//m_pObstacle1 = new Obstacle();
	//m_pObstacle1->getTransform()->position = glm::vec2(380.f, 280.f);
	//addChild(m_pObstacle1);

	//// add the Obstacle to the scene as a start point
	//m_pObstacle2 = new Obstacle();
	//m_pObstacle2->getTransform()->position = glm::vec2(380.f, 80.f);
	//addChild(m_pObstacle2);

	//// add the Obstacle to the scene as a start point
	//m_pObstacle3 = new Obstacle();
	//m_pObstacle3->getTransform()->position = glm::vec2(380.f, 480.f);
	//addChild(m_pObstacle3);
	for (int i = 0; i < 6; i++)
	{
		Obstacle* obs = new Obstacle();
		obs->getTransform()->position = m_getTile(i + 1, 11)->getTransform()->position;
		obs->setGridPosition(i + 1, 11);
		m_Obstacles.push_back(obs);
		addChild(obs);
	}

	// add Blink to the scene
	

	// add the target to the scene a goal
	m_pTarget = new Blink();
	m_pTarget->getTransform()->position = glm::vec2(700.f, 300.f);
	addChild(m_pTarget);

	// Set some data onto the ship's decision tree's map. I only have the player/target for now.
	m_pShip->getTree()->setPlayer(m_pTarget);
	m_pShip->getTree()->getMap().emplace("target", m_pTarget);
	m_pShip->getTree()->MakeDecision(); // Or we won't go into patrol
	
	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function()
{
	// TODO:
	// Toggle Visibility for the StarShip and the Target
		
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
	angle = m_pShip->getCurrentHeading(); // New.
	if (ImGui::SliderInt("Ship Direction", &angle, -360, 360))
	{
		m_pShip->setCurrentHeading(angle);
	}

	ImGui::Separator();

	static int shipPosition[] = { m_pShip->getTransform()->position.x, m_pShip->getTransform()->position.y };
	if (ImGui::SliderInt2("Ship Position", shipPosition, 0, 800))
	{
		m_pShip->getTransform()->position.x = shipPosition[0];
		m_pShip->getTransform()->position.y = shipPosition[1];

		/*std::cout << "------------------------" << std::endl;
		std::cout << m_pShip->getTree()->MakeDecision() << std::endl;
		std::cout << "------------------------\n" << std::endl;*/
	}

	static int targetPosition[] = { m_pTarget->getTransform()->position.x, m_pTarget->getTransform()->position.y };
	if (ImGui::SliderInt2("Target Position", targetPosition, 0, 800))
	{
		m_pTarget->getTransform()->position.x = targetPosition[0];
		m_pTarget->getTransform()->position.y = targetPosition[1];

		/*std::cout << "------------------------" << std::endl;
		std::cout << m_pShip->getTree()->MakeDecision() << std::endl;
		std::cout << "------------------------\n" << std::endl;*/
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
	m_pShip->setClosestLOSNode(closestPathNode);
	return closestPathNode;
}

void PlayScene::m_CheckDetection(DisplayObject* target_object)
{
	m_pShip->setHasDetection(false);
	auto EnemyToTargetDistance = Util::distance(m_pShip->getTransform()->position, target_object->getTransform()->position);
	if (EnemyToTargetDistance <= m_pShip->getDetectionDistance())
	{
		m_pShip->setHasDetection(true);
	}
	
}
