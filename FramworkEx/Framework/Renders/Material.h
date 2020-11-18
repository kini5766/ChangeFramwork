#pragma once

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
	class IEffectVariable;
	map<string, IEffectVariable*> effectValues;

public:
	void SetScalar(string name, float value);
	void SetVector(string name, const float* value);
	void SetMatrix(string name, Matrix* value);
	void SetTexture(string name, wstring file);
	void SetTexture(string name, Texture* value);
	Texture* GetTexture(string name);

private:
	class IEffectVariable
	{
	public:
		virtual ~IEffectVariable() {}
		virtual void Render() = 0;
		virtual void SetShader(Shader* value) = 0;
	};


	class EffectScalar : public IEffectVariable
	{
	public:
		EffectScalar(string name, float value) : value(value), name(name) { }
		~EffectScalar() {}
		void Render() override { sEffect->SetFloat(value); }
		void SetShader(Shader* value) override { sEffect = value->AsScalar(name); }
		void SetValue(float value) { this->value = value; }

	private:
		string name;
		float value;
		ID3DX11EffectScalarVariable* sEffect;
	};

	class EffectVector : public IEffectVariable
	{
	public:
		EffectVector(string name, const float* value) : value(value), name(name) { }
		~EffectVector() {}
		void Render() override { sEffect->SetFloatVector(value); }
		void SetShader(Shader* value) override { sEffect = value->AsVector(name); }
		void SetValue(const float* value) { this->value = value; }

	private:
		Vector4 value;
		string name;
		ID3DX11EffectVectorVariable* sEffect;
	};

	class EffectMatrix : public IEffectVariable
	{
	public:
		EffectMatrix(string name, Matrix* value) : name(name) { SetValue(value); }
		~EffectMatrix() {}
		void Render() override { sEffect->SetMatrix(value); }
		void SetShader(Shader* value) override { sEffect = value->AsMatrix(name); }
		void SetValue(const Matrix* value) { memcpy(&this->value, value, sizeof(Matrix)); }

	private:
		string name;
		Matrix value;
		ID3DX11EffectMatrixVariable* sEffect;
	};

	class EffectTexture : public IEffectVariable
	{
	public:
		EffectTexture(string name, Texture* value) : name(name) { SetValue(value); }
		~EffectTexture() { SafeDelete(value); }
		void Render() override { sEffect->SetResource(srv); }
		void SetShader(Shader* value) override { sEffect = value->AsSRV(name); }
		void SetValue(Texture* value) { SafeDelete(this->value); this->value = value; if (value != nullptr) srv = value->SRV(); }
		Texture* GetValue() { return value; }

	private:
		string name;
		Texture* value;
		ID3D11ShaderResourceView* srv = nullptr;
		ID3DX11EffectShaderResourceVariable* sEffect;
	};
};
