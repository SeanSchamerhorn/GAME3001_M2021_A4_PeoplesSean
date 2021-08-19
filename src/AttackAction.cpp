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
	std::cout << "Performing Attack Action" << std::endl;
	if (m_pShip->getTree()->getPlayer()->getHealth() != 0 && Game::Instance().getFrames() % 60 == 0)
	{
		m_pShip->getTree()->getPlayer()->setHealth(m_pShip->getTree()->getPlayer()->getHealth() - 10);
	}
	if (m_pShip->getTree()->getPlayer()->getTransform()->position.x <= m_pShip->getTransform()->position.x)
	{
		std::cout << "ATTACK LEFT" << std::endl;
		
	}
	if (Game::Instance().getFrames() % 32 == 0)
		SoundManager::Instance().playSound("strike", 0);
	if (m_pShip->getTree()->getPlayer()->getTransform()->position.x >= m_pShip->getTransform()->position.x)
		std::cout << "ATTACK RIGHT" << std::endl;
	m_pShip->setAnimationState(ENEMY_ATTACK_UP);
	
	
}
