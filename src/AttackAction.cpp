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
	if(m_pShip->getHealth()> 0 )
	{
		m_pShip->setHealth(m_pShip->getHealth() - 1);
	}
	m_pShip->setAnimationState(ENEMY_ATTACK_UP);
	
	
}
