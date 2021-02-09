#pragma once

// PreRender용 카메라
class PreCamera : public ICamera
{
public:
	PreCamera(float width = 0, float height = 0);
	virtual ~PreCamera();

public:
	// ICamera을(를) 통해 상속됨
	virtual void GetView(Matrix * out) override;
	virtual void GetProjection(Matrix * out) override;
	virtual void GetPlanes4(Plane * planes) override;

public:
	virtual void PreRender();

public:
	RenderTarget* GetRenderTarget() { return renderTarget; }
	DepthStencil* GetDepthStencil() { return depthStencil; }

protected:
	RenderTarget* renderTarget;
	DepthStencil* depthStencil;
	Viewport* viewport;
};
