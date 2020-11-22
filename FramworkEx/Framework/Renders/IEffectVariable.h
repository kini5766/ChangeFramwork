#pragma once

#include "Framework.h"


namespace MaterialValue
{
	class IEffectVariable
	{
	public:
		virtual ~IEffectVariable() {}
		virtual void Render() = 0;
		virtual void SetShader(Shader* value) = 0;
	};


	class EffectScalarFloat : public IEffectVariable
	{
	public:
		EffectScalarFloat(string name, float value) : value(value), name(name) { }
		~EffectScalarFloat() {}
		void Render() override { sEffect->SetFloat(value); }
		void SetShader(Shader* value) override { sEffect = value->AsScalar(name); }
		void SetValue(float value) { this->value = value; }

	private:
		string name;
		float value;
		ID3DX11EffectScalarVariable* sEffect;
	};

	class EffectScalarInt : public IEffectVariable
	{
	public:
		EffectScalarInt(string name, int value) : value(value), name(name) { }
		~EffectScalarInt() {}
		void Render() override { sEffect->SetInt(value); }
		void SetShader(Shader* value) override { sEffect = value->AsScalar(name); }
		void SetValue(int value) { this->value = value; }

	private:
		string name;
		int value;
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
		string name;
		Vector4 value;
		ID3DX11EffectVectorVariable* sEffect;
	};

	class EffectVectorPointer : public IEffectVariable
	{
	public:
		EffectVectorPointer(string name, const float* value) : value(value), name(name) { }
		~EffectVectorPointer() {}
		void Render() override { sEffect->SetFloatVector(value); }
		void SetShader(Shader* value) override { sEffect = value->AsVector(name); }
		void SetValue(const float* value) { this->value = value; }

	private:
		string name;
		const float* value;
		ID3DX11EffectVectorVariable* sEffect;
	};

	class EffectMatrix : public IEffectVariable
	{
	public:
		EffectMatrix(string name, const float* value) : name(name) { SetValue(value); }
		~EffectMatrix() {}
		void Render() override { sEffect->SetMatrix(value); }
		void SetShader(Shader* value) override { sEffect = value->AsMatrix(name); }
		void SetValue(const float* value) { this->value = value; }

	private:
		string name;
		Matrix value;
		ID3DX11EffectMatrixVariable* sEffect;
	};

	class EffectMatrixPointer : public IEffectVariable
	{
	public:
		EffectMatrixPointer(string name, const Matrix* value) : value(value), name(name) { }
		~EffectMatrixPointer() {}
		void Render() override { sEffect->SetMatrix(*value); }
		void SetShader(Shader* value) override { sEffect = value->AsMatrix(name); }
		void SetValue(const Matrix* value) { this->value = value; }

	private:
		string name;
		const Matrix* value;
		ID3DX11EffectMatrixVariable* sEffect;
	};

	class EffectSRV : public IEffectVariable
	{
	public:
		EffectSRV(string name, ID3D11ShaderResourceView* value) : name(name) { SetValue(value); }
		~EffectSRV() { }
		void Render() override { sEffect->SetResource(value); }
		void SetShader(Shader* value) override { sEffect = value->AsSRV(name); }
		void SetValue(ID3D11ShaderResourceView* value) { this->value = value; }

	private:
		string name;
		ID3D11ShaderResourceView* value;
		ID3DX11EffectShaderResourceVariable* sEffect;
	};
}

namespace MaterialGroupValue
{
	class IVariableEffects
	{
	public:
		virtual ~IVariableEffects() {}
		virtual void Render() = 0;
		virtual void AddShader(Shader* value) = 0;
	};

	class ConstantBufferEffects : public IVariableEffects
	{
	public:
		ConstantBufferEffects(string name, ID3D11Buffer* value)
			: name(name), value(value) { }
		~ConstantBufferEffects() { }

		void Render() override {
			for (auto& sEffect : sEffects)sEffect->SetConstantBuffer(value);
		}
		void AddShader(Shader* value) override { sEffects.push_back(value->AsConstantBuffer(name)); }
		void SetValue(ID3D11Buffer* value) { this->value = value; }

	private:
		string name;
		ID3D11Buffer* value;
		vector<ID3DX11EffectConstantBuffer*> sEffects;
	};

	class SRVEffects : public IVariableEffects
	{
	public:
		SRVEffects(string name, ID3D11ShaderResourceView* value)
			: name(name), value(value) { }
		~SRVEffects() { }

		void Render() override {
			for (auto& sEffect : sEffects)sEffect->SetResource(value);
		}
		void AddShader(Shader* value) override { sEffects.push_back(value->AsSRV(name)); }
		void SetValue(ID3D11ShaderResourceView* value) { this->value = value; }

	private:
		string name;
		ID3D11ShaderResourceView* value;
		vector<ID3DX11EffectShaderResourceVariable*> sEffects;
	};
}
