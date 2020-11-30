#pragma once
namespace MaterialValue { class IEffectVariable; }

class Material
{
public:
	Material();
	Material(Shader* shader);
	virtual ~Material();

public:
	virtual void Render();

	virtual void SetShader(Shader* value);
	Shader* GetShader() { return shader; }

protected:
	Shader* shader = nullptr;


private:
	map<string, MaterialValue::IEffectVariable*> effectValues;

public:
	void SetFloat(string name, float value);
	void SetInt(string name, int value);
	void SetVector(string name, const float* value);
	void SetVectorPointer(string name, const float* value);
	void SetMatrix(string name, const float* value);
	void SetMatrixPointer(string name, const Matrix* value);
	void SetTexture(string name, Texture* value);
	void SetSRV(string name, ID3D11ShaderResourceView* value);
};
