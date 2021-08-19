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
	if (m_pEnemy->m_state != ActionState::PATROL) //"enter/initialize" of a state
	{
		m_pTarget = m_pEnemy->getCurrentWaypoint();
		m_pEnemy->resumePatrol(); // Also sets state to PATROL
	}
	if (Util::distance(m_pEnemy->getTransform()->position, m_pTarget->getTransform()->position) <= 10.0f)
	{
		m_pTarget = m_pEnemy->getNextWaypoint();
		
	}
	m_pEnemy->move();
}
