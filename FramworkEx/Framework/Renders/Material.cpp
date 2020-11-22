#include "Framework.h"
#include "Material.h"

#include "IEffectVariable.h"
using namespace MaterialValue;

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
void Material::SetFloat(string name, float value)
{
	if (effectValues.count(name) == 0)
	{
		IEffectVariable* effect = new EffectScalarFloat(name, value);
		if (shader != nullptr)
			effect->SetShader(shader);

		effectValues[name] = effect;
		return;
	}

	dynamic_cast<EffectScalarFloat*>(effectValues[name])->SetValue(value);
}

void Material::SetInt(string name, int value)
{
	if (effectValues.count(name) == 0)
	{
		IEffectVariable* effect = new EffectScalarInt(name, value);
		if (shader != nullptr)
			effect->SetShader(shader);

		effectValues[name] = effect;
		return;
	}

	dynamic_cast<EffectScalarInt*>(effectValues[name])->SetValue(value);
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

// EffectVectorPointer
void Material::SetVectorPointer(string name, const float * value)
{
	if (effectValues.count(name) == 0)
	{
		IEffectVariable* effect = new EffectVectorPointer(name, value);
		if (shader != nullptr)
			effect->SetShader(shader);

		effectValues[name] = effect;
		return;
	}

	dynamic_cast<EffectVectorPointer*>(effectValues[name])->SetValue(value);
}

// EffectMatrix
void Material::SetMatrix(string name, const float * value)
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

// EffectMatrixPointer
void Material::SetMatrixPointer(string name, const Matrix * value)
{
	if (effectValues.count(name) == 0)
	{
		IEffectVariable* effect = new EffectMatrixPointer(name, value);
		if (shader != nullptr)
			effect->SetShader(shader);

		effectValues[name] = effect;
		return;
	}

	dynamic_cast<EffectMatrixPointer*>(effectValues[name])->SetValue(value);
}

// EffectTexture
void Material::SetTexture(string name, Texture * value)
{
	if (effectValues.count(name) == 0)
	{
		IEffectVariable* effect;
		if (value == nullptr)
			effect = new EffectSRV(name, nullptr);
		else
			effect = new EffectSRV(name, value->SRV());

		if (shader != nullptr)
			effect->SetShader(shader);

		effectValues[name] = effect;
		return;
	}

	if (value == nullptr)
		dynamic_cast<EffectSRV*>(effectValues[name])->SetValue(nullptr);
	else
		dynamic_cast<EffectSRV*>(effectValues[name])->SetValue(value->SRV());
	
}

void Material::SetSRV(string name, ID3D11ShaderResourceView * value)
{
	if (effectValues.count(name) == 0)
	{
		IEffectVariable* effect = new EffectSRV(name, value);

		if (shader != nullptr)
			effect->SetShader(shader);

		effectValues[name] = effect;
		return;
	}

	dynamic_cast<EffectSRV*>(effectValues[name])->SetValue(value);
}


#pragma endregion

