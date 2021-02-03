#pragma once

struct EffectUnit
{
	vector<FuncComposite> Effects;
	bool IsActive = false;
};

class EffectUnitGroup
{
public:
	EffectUnitGroup(float width = 0, float height = 0);
	~EffectUnitGroup();

public:
	void Update();
	void BeginPreRender();
	void EndPreRender();
	void Render();

public:
	EffectUnit* AddUnit();
	void OnChanged() { bChange = true; }

private:
	Shader* shader;
	PostEffect* postEffect;
	vector<EffectUnit*> units;

private:
	bool bChange = false;
};