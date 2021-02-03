#include "Framework.h"
#include "EffectUnitGroup.h"

EffectUnitGroup::EffectUnitGroup(float width, float height)
{
	shader = Shader::Load(URI::Shaders + L"03_Panel.fxo");
	postEffect = new PostEffect(shader, width, height);
}

EffectUnitGroup::~EffectUnitGroup()
{
	for (auto d : units)
		SafeDelete(d);

	SafeDelete(postEffect);

	SafeRelease(shader);
}

void EffectUnitGroup::Update()
{
	if (bChange)
	{
		bChange = false;
		postEffect->ClearEffects();

		for (UINT i = 0; i < units.size(); i++)
		{
			if (units[i]->IsActive)
				postEffect->AddEffects(units[i]->Effects);
		}
	}

	postEffect->Update();
}

void EffectUnitGroup::BeginPreRender()
{
	postEffect->BeginPreRender();
}

void EffectUnitGroup::EndPreRender()
{
	postEffect->EndPreRender();
}

void EffectUnitGroup::Render()
{
	postEffect->Render();
}

EffectUnit * EffectUnitGroup::AddUnit()
{
	EffectUnit* unit = new EffectUnit();
	
	units.push_back(unit);

	bChange = true;

	return unit;
}
