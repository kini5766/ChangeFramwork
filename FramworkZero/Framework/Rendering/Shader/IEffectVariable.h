#pragma once

#include "Framework.h"


namespace ShaderSetterValue
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
		EffectSRV(string name, ID3D11ShaderResourceView* value) : name(name), value(value) { }
		~EffectSRV() { }
		void Render() override { sEffect->SetResource(value); }
		void SetShader(Shader* value) override { sEffect = value->AsSRV(name); }
		void SetValue(ID3D11ShaderResourceView* value) { this->value = value; }

	private:
		string name;
		ID3D11ShaderResourceView* value;
		ID3DX11EffectShaderResourceVariable* sEffect;
	};

	class EffectUAV : public IEffectVariable
	{
	public:
		EffectUAV(string name, ID3D11UnorderedAccessView* value) : name(name), value(value) { }
		~EffectUAV() { }
		void Render() override { sEffect->SetUnorderedAccessView(value); }
		void SetShader(Shader* value) override { sEffect = value->AsUAV(name); }
		void SetValue(ID3D11UnorderedAccessView* value) { this->value = value; }

	private:
		string name;
		ID3D11UnorderedAccessView* value;
		ID3DX11EffectUnorderedAccessViewVariable* sEffect;
	};

	class EffectConstantBuffer : public IEffectVariable
	{
	public:
		EffectConstantBuffer(string name, ID3D11Buffer* value) : name(name), value(value) { }
		~EffectConstantBuffer() { }
		void Render() override { sEffect->SetConstantBuffer(value); }
		void SetShader(Shader* value) override { sEffect = value->AsConstantBuffer(name); }
		void SetValue(ID3D11Buffer* value) { this->value = value; }

	private:
		string name;
		ID3D11Buffer* value;
		ID3DX11EffectConstantBuffer* sEffect;
	};

	class EffectSampler : public IEffectVariable
	{
	public:
		EffectSampler(string name, UINT index, ID3D11SamplerState* value)
			: name(name), value(value) { }
		~EffectSampler() { }
		void Render() override { sEffect->SetSampler(index, value); }
		void SetShader(Shader* value) override { sEffect = value->AsSampler(name); }
		void SetValue(UINT index, ID3D11SamplerState* value)
		{ this->index = index; this->value = value; }

	private:
		string name;
		UINT index;
		ID3D11SamplerState* value;
		ID3DX11EffectSamplerVariable* sEffect;
	};
}

namespace ShadersValueSetterValue
{
	class IVariableEffects
	{
	public:
		virtual ~IVariableEffects() {}
		virtual void Render() = 0;
		virtual void AddShader(Shader* value) = 0;
		virtual void RemoveShader(Shader* target) = 0;
		virtual void ClearShaders() = 0;
	};

	class ConstantBufferEffects : public IVariableEffects
	{
	public:
		ConstantBufferEffects(string name, ID3D11Buffer* value)
			: name(name), value(value) { }
		~ConstantBufferEffects() { }

		void Render() override {
			for (auto& e : sEffects)e.Effect->SetConstantBuffer(value);
		}
		void AddShader(Shader* value) override { 
			sEffects.push_back({ value, value->AsConstantBuffer(name) });
		}
		void RemoveShader(Shader* value) override { 
			vector<ShaderEffectPair>::iterator iter = sEffects.begin();
			for (; iter != sEffects.end(); iter++) 
				if ((*iter).Shader == value) { sEffects.erase(iter); break; }
		}
		void ClearShaders() override { sEffects.clear(); }
		void SetValue(ID3D11Buffer* value) { this->value = value; }

	private:
		string name;
		ID3D11Buffer* value;

	private:
		struct ShaderEffectPair
		{
			Shader* Shader;
			ID3DX11EffectConstantBuffer* Effect;
		};
		vector<ShaderEffectPair> sEffects;
	};

	class SRVEffects : public IVariableEffects
	{
	public:
		SRVEffects(string name, ID3D11ShaderResourceView* value)
			: name(name), value(value) { }
		~SRVEffects() { }

		void Render() override {
			for (auto& e : sEffects)e.Effect->SetResource(value);
		}
		void AddShader(Shader* value) override {
			sEffects.push_back({ value, value->AsSRV(name) });
		}
		void RemoveShader(Shader* value) override {
			vector<ShaderEffectPair>::iterator iter = sEffects.begin();
			for (; iter != sEffects.end(); iter++)
				if ((*iter).Shader == value) { sEffects.erase(iter); break; }
		}
		void ClearShaders() override { sEffects.clear(); }
		void SetValue(ID3D11ShaderResourceView* value) { this->value = value; }

	private:
		string name;
		ID3D11ShaderResourceView* value;

	private:
		struct ShaderEffectPair
		{
			Shader* Shader;
			ID3DX11EffectShaderResourceVariable* Effect;
		};
		vector<ShaderEffectPair> sEffects;
	};
}
