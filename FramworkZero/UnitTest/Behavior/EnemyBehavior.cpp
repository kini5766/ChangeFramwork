#include "stdafx.h"
#include "EnemyBehavior.h"

#include "BehaviorSelecter.h"

EnemyBehavior::EnemyBehavior()
{
	PatrolChecker patrol;
	patrol.InBattle = &inBattle;
	patrol.InRecognition = &inRecognition;
	selecter->GetBehaviors()->push_back(
		[patrol]() { return patrol.IsAble(); }
	);
}

EnemyBehavior::~EnemyBehavior()
{
}
