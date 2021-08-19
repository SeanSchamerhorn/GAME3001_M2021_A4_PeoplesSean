#include "MoveToLOSAction.h"
#include "PlayScene.h"
#include "Enemy.h"
#include "DisplayObject.h"
#include <iostream>

MoveToLOSAction::MoveToLOSAction()
{
	name = "Move To LOS Action";
}

MoveToLOSAction::~MoveToLOSAction()
= default;

void MoveToLOSAction::Action()
{
	std::cout << "Performing Move To LOS Action" << std::endl;
	if (m_pEnemy->m_state != ActionState::MOVE_TO_LOS)
	{
		m_pEnemy->m_state = ActionState::MOVE_TO_LOS;		
	}
	m_pTarget = m_pEnemy->getClosestLOSNode();
	m_pEnemy->setTarget(m_pTarget);
	if (Util::distance(m_pEnemy->getTransform()->position, m_pTarget->getTransform()->position) <= 5.0f)
	{
		m_pEnemy->rotateToTarget(m_pEnemy->getTree()->getMap()["target"]);
	
	}
	else
		m_pEnemy->move();
	// If agent is at node, rotate towards target so it actually gets LOS. Add this. 
}