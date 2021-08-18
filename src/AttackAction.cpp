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
	m_pShip->setAnimationState(ENEMY_ATTACK_UP);
	
	
}
