#pragma once

struct ShadowCasterInput
{
	Shader* Shader;
	function<void()> FuncPreRender;
};

class ShadowCaster
{
public:
	ShadowCaster(const ShadowCasterInput& input);
	~ShadowCaster();

public:
	void Release() { isRelease = false; funcPreRender = []() {}; }

public:
	void PreRender(struct PreShadowValue* shadow);
	bool IsRelease() const { return isRelease; }

public:
	ShaderSetter* material;
	function<void()> funcPreRender;
	bool isRelease = false;

private:
	ConstantBuffer* buffer;
	struct ShadowDesc* shadowDesc;
};

struct PreShadowValue
{
	ShadowDesc* Shadow;
	DepthStencil* DepthStencil;
	ID3D11SamplerState* ShadowSampler;
};