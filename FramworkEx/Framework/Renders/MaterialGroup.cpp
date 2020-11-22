#include "Framework.h"
#include "MaterialGroup.h"

#include "IEffectVariable.h"
using namespace MaterialGroupValue;

MaterialGroup::MaterialGroup()
{
}

MaterialGroup::~MaterialGroup()
{
	for (auto& d : effectsValues)
		SafeDelete(d.second);

	for (auto d : shaders)
		SafeDelete(d);
}

void MaterialGroup::Render()
{
	for (auto i : effectsValues)
		i.second->Render();
}

UINT MaterialGroup::AddMaterial(Material * value)
{
	AddShader(value->GetShader());

	materials.push_back(value);
	return materials.size() - 1;
}


#pragma region EffectValues

void MaterialGroup::SetConstantBuffer(string name, ConstantBuffer * buffer)
{
	if (effectsValues.count(name) == 0)
	{
		IVariableEffects* effect;
		if (buffer == nullptr)
			effect = new ConstantBufferEffects(name, nullptr);
		else
			effect = new ConstantBufferEffects(name, buffer->Buffer());

		for (ShaderRef* ref : shaders)
			effect->AddShader(ref->Shader);

		effectsValues[name] = effect;
		return;
	}

	dynamic_cast<ConstantBufferEffects*>(effectsValues[name])->SetValue(buffer->Buffer());
}

void MaterialGroup::SetSRV(string name, ID3D11ShaderResourceView * value)
{
	if (effectsValues.count(name) == 0)
	{
		IVariableEffects* effect = new SRVEffects(name, value);

		for (ShaderRef* ref : shaders)
			effect->AddShader(ref->Shader);

		effectsValues[name] = effect;
		return;
	}

	dynamic_cast<SRVEffects*>(effectsValues[name])->SetValue(value);
}

#pragma endregion


void MaterialGroup::AddShader(Shader * value)
{
	ShaderRef* curr = nullptr;

	for (ShaderRef* ref : shaders)
	{
		if (ref->Shader == value)
		{
			curr = ref;
			curr->RefCount++;
			break;
		}
	}

	if (curr == nullptr)
	{
		curr = new ShaderRef();
		curr->Shader = value;
		curr->RefCount = 1;
		shaders.push_back(curr);

		for (auto i : effectsValues)
			i.second->AddShader(value);
	}
}