#include "Framework.h"
#include "ShaderSetter.h"

#include "IEffectVariable.h"
using namespace ShaderSetterValue;

ShaderSetter::ShaderSetter()
	: isCreated(false)
{
}

ShaderSetter::ShaderSetter(wstring file)
	: isCreated(true)
{
	SetShader(Shader::Load(file));
	isCreated = true;
}

ShaderSetter::ShaderSetter(Shader* _shader)
	: isCreated(false)
{
	SetShader(_shader);
}

ShaderSetter::ShaderSetter(const ShaderSetter & m) 
{
	if (m.shader == nullptr) shader = nullptr;
	else shader = Shader::LoadFullName(m.shader->GetFile());
	isCreated = m.isCreated;
}

ShaderSetter::~ShaderSetter()
{
	for (auto& d : effectValues)
		SafeDelete(d.second);

	if (isCreated)
		SafeRelease(shader);
}

void ShaderSetter::Render()
{
	for (auto& i : effectValues)
		i.second->Render();
}

void ShaderSetter::SetShader(Shader * value)
{
	if (isCreated) SafeRelease(shader); isCreated = false;

	shader = value;
	for (auto& i : effectValues)
		i.second->SetShader(shader);

}


#pragma region EffectValues

// EffectScalar
void ShaderSetter::SetFloat(string name, float value)
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

void ShaderSetter::SetInt(string name, int value)
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
void ShaderSetter::SetVector(string name, const float * value)
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
void ShaderSetter::SetVectorPointer(string name, const float * value)
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
void ShaderSetter::SetMatrix(string name, const float * value)
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
void ShaderSetter::SetMatrixPointer(string name, const Matrix * value)
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

// EffectSRV (Texture)
void ShaderSetter::SetTexture(string name, Texture * value)
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

// EffectSRV
void ShaderSetter::SetSRV(string name, ID3D11ShaderResourceView * value)
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

// EffectUAV
void ShaderSetter::SetUAV(string name, ID3D11UnorderedAccessView * value)
{
	if (effectValues.count(name) == 0)
	{
		IEffectVariable* effect = new EffectUAV(name, value);

		if (shader != nullptr)
			effect->SetShader(shader);

		effectValues[name] = effect;
		return;
	}

	dynamic_cast<EffectUAV*>(effectValues[name])->SetValue(value);
}

// EffectConstantBuffer
void ShaderSetter::SetConstantBuffer(string name, ID3D11Buffer * value)
{
	if (effectValues.count(name) == 0)
	{
		IEffectVariable* effect = new EffectConstantBuffer(name, value);

		if (shader != nullptr)
			effect->SetShader(shader);

		effectValues[name] = effect;
		return;
	}

	dynamic_cast<EffectConstantBuffer*>(effectValues[name])->SetValue(value);
}


#pragma endregion

