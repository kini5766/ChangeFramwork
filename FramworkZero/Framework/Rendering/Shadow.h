#pragma once

class ShadowCaster
{
public:
	ShadowCaster(Shader* s);
	~ShadowCaster();

public:
	void SetShadow_Global();
	void SetShadow(class Shadow* value);
	void SetFuncPreRender(const function<void(void)>& value) { funcPreRender = value; }

private:
	void PreRender();  // Event

private:
	function<void(void)> funcPreRender;

	ShaderSetter* shader;
	ConstantBuffer* buffer;

	ID3DX11EffectSamplerVariable* sShadowSampler;

private:
	struct ShadowCastDesc* desc;

};


class Shadow
{
public:
	struct ShadowDesc
	{
		Matrix View;
		Matrix Projection;

		// 블러
		Vector2 MapSize;

		//
		float Bias = -0.001f;

		// 셰도우 방법
		UINT Quality = 2;
	};

private:
	static Shadow* globalShadow;

public:
	static void SetGlobal(Shadow* value) { globalShadow = value; }
	static void AddCaster_Global(ShadowCastDesc** shadowcast);


public:
	Shadow(const Vector3& at, float radius, float width = 1024, float height = 1024);
	~Shadow();

public:
	void PreRender();
	void ImGuiRender();

public:
	void AddCaster(ShadowCastDesc** outDesc);

public:
	ID3D11ShaderResourceView* SRV() { return renderTarget->SRV(); }
	void Quality(UINT value) { desc.Quality = value; }
	void Bias(float value) { desc.Bias = value; }


private:
	float width, height;
	// 영역 크기 (오소그래픽)
	float radius;
	Vector3 at;

	ID3D11SamplerState* shadowSampler;
	RenderTarget* renderTarget;
	DepthStencil* depthStencil;
	Viewport* viewport;
	ShadowDesc desc;

	list<ShadowCastDesc*> shadowCasters;
};


struct ShadowCastDesc
{
	function<void(void)> FuncPreRender;

	const struct Shadow::ShadowDesc* ShadowDesc;
	ID3D11SamplerState* ShadowSampler;
	DepthStencil* DepthStencil;
	bool IsDestroy;

	ShadowCastDesc** ThisPointer;
};


class ShadowTest
{
public:
	ShadowTest(Shader* s, const Vector3& at, float radius, float width = 1024, float height = 1024);
	~ShadowTest();

	void PreRender();
	void RenderImGui();
	ID3D11ShaderResourceView* SRV() { return renderTarget->SRV(); }

	void Quality(UINT value) { desc.Quality = value; }
	void Bias(float value) { desc.Bias = value; }

private:
	ShaderSetter* shader;

	// 각진 그림자
	float width, height;
	// 영역 크기 (오소그래픽)
	float radius;
	Vector3 at;

	RenderTarget* renderTarget;
	DepthStencil* depthStencil;
	Viewport* viewport;

	ConstantBuffer* buffer;

	ID3D11SamplerState* shadowSampler;
	ID3DX11EffectSamplerVariable* sShadowSampler;

private:
	struct Desc
	{
		Matrix View;
		Matrix Projection;

		// 블러
		Vector2 MapSize;

		//
		float Bias = -0.001f;

		// 셰도우 방법
		UINT Quality = 2;
	} desc;
};