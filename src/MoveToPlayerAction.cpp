#include "MoveToPlayerAction.h"
#include <iostream>

MoveToPlayerAction::MoveToPlayerAction()
{
	name = "Move To Player Action";
}

MoveToPlayerAction::~MoveToPlayerAction()
= default;

void MoveToPlayerAction::Action()
{
	std::cout << "Performing Move To Player Action" << std::endl;
	if (m_pEnemy->m_state != ActionState::MOVE_TO_PLAYER) // "enter/init"
	{
		m_pTarget = m_pEnemy->getTree()->getMap()["target"]; // Again just target/player in map.
		m_pEnemy->setTarget(m_pTarget);
		m_pEnemy->m_state = ActionState::MOVE_TO_PLAYER;
	}
	m_pEnemy->move();
}
