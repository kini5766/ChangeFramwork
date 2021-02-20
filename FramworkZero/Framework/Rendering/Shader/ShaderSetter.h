#pragma once
namespace ShaderSetterValue { class IEffectVariable; }

class ShaderSetter
{
public:
	ShaderSetter();
	ShaderSetter(wstring file);
	ShaderSetter(Shader* shader);
	ShaderSetter(const ShaderSetter& m);
	virtual ~ShaderSetter();

public:
	virtual void Render();

	virtual void SetShader(Shader* value);
	Shader* GetShader() { return shader; }

protected:
	Shader* shader = nullptr;

private:
	bool isCreated;


private:
	map<string, ShaderSetterValue::IEffectVariable*> effectValues;

public:
	void SetFloat(string name, float value);
	void SetInt(string name, int value);
	void SetVector(string name, const float* value);
	void SetVectorPointer(string name, const float* value);
	void SetMatrix(string name, const float* value);
	void SetMatrixPointer(string name, const Matrix* value);
	void SetSRV(string name, ID3D11ShaderResourceView* value);
	void SetUAV(string name, ID3D11UnorderedAccessView* value);
	void SetConstantBuffer(string name, ID3D11Buffer* value);
	void SetSampler(string name, UINT index, ID3D11SamplerState* value);
};
