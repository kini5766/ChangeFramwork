#pragma once

class IEnemySkill
{
public:
	virtual bool IsValid() = 0;
	virtual float GetWeight() = 0;
	virtual class FlowTesk* GetTesk() = 0;
};
