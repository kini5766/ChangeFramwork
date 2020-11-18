#include "Framework.h"
#include "Material.h"

Material::Material()
{
}

Material::Material(Shader* _shader)
{
	SetShader(_shader);
}

Material::~Material()
{
	for (auto& d : effectValues)
		SafeDelete(d.second);
}

void Material::Render()
{
	for (auto& i : effectValues)
		i.second->Render();
}

void Material::SetShader(Shader * value)
{
	shader = value;
	for (auto& i : effectValues)
		i.second->SetShader(shader);

}

#pragma region EffectValues


// EffectScalar
void Material::SetScalar(string name, float value)
{
	if (effectValues.count(name) == 0)
	{
		IEffectVariable* effect = new EffectScalar(name, value);
		if (shader != nullptr)
			effect->SetShader(shader);

		effectValues[name] = effect;
		return;
	}

	dynamic_cast<EffectScalar*>(effectValues[name])->SetValue(value);
}

// EffectVector
void Material::SetVector(string name, const float * value)
{
	if (effectValues.count(name) == 0)
	{
		IEffectVariable* effect = new EffectVector(name, value);
		if (shader != nullptr)
			effect->SetShader(shader);

		effectValues[name] = effect;
		return;
	}

	dynamic_cast<EffectVector*>(effectValues[name])->SetValue(value);
}

// EffectMatrix
void Material::SetMatrix(string name, Matrix * value)
{
	if (effectValues.count(name) == 0)
	{
		IEffectVariable* effect = new EffectMatrix(name, value);
		if (shader != nullptr)
			effect->SetShader(shader);

		effectValues[name] = effect;
		return;
	}

	dynamic_cast<EffectMatrix*>(effectValues[name])->SetValue(value);
}

// EffectTexture
void Material::SetTexture(string name, wstring file)
{
	SetTexture(name, new Texture(file));
}

void Material::SetTexture(string name, Texture * value)
{
	if (effectValues.count(name) == 0)
	{
		IEffectVariable* effect = new EffectTexture(name, value);
		if (shader != nullptr)
			effect->SetShader(shader);

		effectValues[name] = effect;
		return;
	}

	dynamic_cast<EffectTexture*>(effectValues[name])->SetValue(value);
}

Texture * Material::GetTexture(string name)
{
	if (effectValues.count(name) == 0)
		return nullptr;

	return dynamic_cast<EffectTexture*>(effectValues[name])->GetValue();
}


#pragma endregion

