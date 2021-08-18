#pragma once
#ifndef __ACTION_NODE__
#define __ACTION_NODE__
#include "TreeNode.h"
#include "ActionState.h"
#include "Blink.h"
#include "Util.h"

#include "ship.h" // New

// Interface
class ActionNode : public TreeNode
{
protected: // New.
	Ship* m_pShip;
	DisplayObject* m_pTarget;

	Blink* m_pPlayer;

public:
	ActionNode(): m_pShip(nullptr), m_pTarget(nullptr), m_pPlayer(nullptr) { isLeaf = true; }
	virtual ~ActionNode() = default;
	virtual void Action() = 0;

	void SetShip(Ship* ship) { m_pShip = ship; }
	void SetTarget(DisplayObject* t) { m_pTarget = t; }

	void SetPlayer(Blink* p) { m_pPlayer = p; }
};

#endif /* defined (__ACTION_NODE__) */