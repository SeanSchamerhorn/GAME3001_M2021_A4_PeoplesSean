#include "PatrolAction.h"
#include <iostream>

#include "Game.h"

PatrolAction::PatrolAction()
{
	name = "Patrol Action";
}

PatrolAction::~PatrolAction()
= default;

void PatrolAction::Action()
{
	std::cout << "Performing Patrol Action" << std::endl;
	if (m_pShip->m_state != ActionState::PATROL) //"enter/initialize" of a state
	{
		m_pTarget = m_pShip->getCurrentWaypoint();
		m_pShip->resumePatrol(); // Also sets state to PATROL
	}
	if (Util::distance(m_pShip->getTransform()->position, m_pTarget->getTransform()->position) <= 10.0f)
	{
		m_pTarget = m_pShip->getNextWaypoint();
		/*if (Game::Instance().getFrames()%60 == 0)
		{
			m_pShip->wait();
			m_pTarget = m_pShip->getNextWaypoint();
		}
		else
		{
			m_pShip->setMaxSpeed(1.0f);
			m_pTarget = m_pShip->getNextWaypoint();
			m_pShip->resumePatrol();
		}*/
	}
	m_pShip->move();
}
