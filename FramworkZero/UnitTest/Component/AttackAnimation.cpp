#include "stdafx.h"
#include "AttackAnimation.h"

AttackAnimation::AttackAnimation()
{
}

AttackAnimation::~AttackAnimation()
{
	for (auto d : units)
		SafeDelete(d);
}

void AttackAnimation::Update()
{
	list<UINT>::iterator iter = playList.begin();
	while (iter != playList.end())
	{
		UINT curr = *iter;
		if (units[curr]->Update(units[curr]->RunningTime))
		{
			units[curr]->RunningTime = 0.0f;
			junkUnits.push_back(curr);
			playList.erase(iter++);
		}
		else
		{
			units[curr]->RunningTime += Time::Delta();
			++iter;
		}
	}
}

bool AttackAnimation::IsAttackAble()
{
	bool result = true;
	result &= junkUnits.size() != 0;
	result &= funcAttackAble();
	return result;
}

void AttackAnimation::Play()
{
	playList.push_back(junkUnits.back());
	junkUnits.pop_back();
}

void AttackAnimation::Stop()
{
	funcStop();
}

void AttackAnimation::Reset()
{
	playList.clear();
	for (UINT i = 0; i < units.size(); i++)
	{
		units[i]->RunningTime = 0.0f;
		junkUnits.push_back(i);
	}
}

void AttackAnimation::AddUnit(function<bool(float)> funcUpdate)
{
	UINT i = units.size();
	Unit* unit = new Unit();
	unit->Update = funcUpdate;
	units.push_back(unit);
	junkUnits.push_back(i);
}
