#pragma once

struct ShadowDesc
{
	Matrix View;
	Matrix Projection;

	// ��
	Vector2 MapSize;

	//
	float Bias = -0.001f;

	// �ε��� ���
	UINT Quality = 2;
};


struct ShadowCastDesc
{
	// inputs
	ID3D11SamplerState* ShadowSampler;
	DepthStencil* DepthStencil;

	// outouts
	function<void(ShadowDesc*)> FuncPreRender;
	// �ε��찡 �ı��Ǹ� null�� �������
	ShadowCastDesc** ThisPointer;
	bool IsDestroy;
};


class Shadow
{
private:
	static Shadow* globalShadow;

public:
	static void SetGlobal(Shadow* value) { globalShadow = value; }
	static ShadowCastDesc* AddCaster_Global();


public:
	Shadow(const Vector3& at, float radius, float width = 1024, float height = 1024);
	~Shadow();

public:
	void PreRender();
	void ImGuiRender();

public:
	struct ShadowCastDesc* AddCaster();

public:
	ID3D11ShaderResourceView* SRV() { return renderTarget->SRV(); }
	void Quality(UINT value) { desc.Quality = value; }
	void Bias(float value) { desc.Bias = value; }


private:
	float width, height;
	// ���� ũ�� (���ұ׷���)
	float radius;
	Vector3 at;

	ID3D11SamplerState* shadowSampler;
	RenderTarget* renderTarget;
	DepthStencil* depthStencil;
	Viewport* viewport;
	ShadowDesc desc;

	list<ShadowCastDesc*> shadowCasters;
};


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
	void PreRender(struct ShadowDesc* desc);  // Event
	void SafeDeleteDesc();
	void SetDesc();

private:
	function<void(void)> funcPreRender;

	ShaderSetter* shader;
	ConstantBuffer* buffer;
	DepthStencil* depth = nullptr;

	ID3DX11EffectSamplerVariable* sShadowSampler;

private:
	struct ShadowCastDesc* desc = nullptr;
	ShadowDesc shadowDesc;
};
