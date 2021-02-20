#pragma once

class ShadowCamera : public ICamera
{
public:
	ShadowCamera(const Vector3& at, float radius, float width = 1024, float height = 1024);
	~ShadowCamera();

public:
	// ICamera을(를) 통해 상속됨
	virtual void GetView(Matrix * out) override;
	virtual void GetProjection(Matrix * out) override;
	virtual void GetPlanes_Side4(Plane * planes) override;

public:
	void Update();
	void PreRender();
	
	DepthStencil* GetDepth() { return depthStencil; }
	RenderTarget* GetRenderTarget() { return renderTarget; }
	Vector3 At() { return at; }
	void At(const Vector3& value) { at = value; }

private:
	Matrix view;
	Matrix projection;

private:
	float width, height;
	// 영역 크기 (오소그래픽)
	float radius;
	Vector3 at;

	RenderTarget* renderTarget;
	DepthStencil* depthStencil;
	Viewport* viewport;
	Frustum* frustum;
};