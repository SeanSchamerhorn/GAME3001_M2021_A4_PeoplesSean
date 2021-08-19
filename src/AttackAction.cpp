#include "AttackAction.h"
#include <iostream>


#include "Game.h"
#include "PlayScene.h"


AttackAction::AttackAction()
{
	name = "Attack Action";
}

AttackAction::~AttackAction()
= default;

void AttackAction::Action()
{
	if (/*m_pEnemy->getTree()->getPlayer()->getHealth() != 0 && */Game::Instance().getFrames() % 60 == 0)
	{
		m_pEnemy->getTree()->getPlayer()->setHealth(m_pEnemy->getTree()->getPlayer()->getHealth() - 10);
	}
	if (m_pEnemy->getTree()->getPlayer()->getTransform()->position.x <= m_pEnemy->getTransform()->position.x)
	{
		std::cout << "ATTACK LEFT" << std::endl;
		
	}
	if (Game::Instance().getFrames() % 32 == 0)
		SoundManager::Instance().playSound("strike", 0);

	if (m_pEnemy->getTree()->getPlayer()->getTransform()->position.x >= m_pEnemy->getTransform()->position.x)
		std::cout << "ATTACK RIGHT" << std::endl;
	m_pEnemy->setAnimationState(ENEMY_ATTACK_UP);
	
	
}
