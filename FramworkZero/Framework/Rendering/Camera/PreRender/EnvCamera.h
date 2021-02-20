#pragma once

class EnvCamera : public ICamera
{
public:
	EnvCamera(float width, float height);
	~EnvCamera();

public:
	// ICamera을(를) 통해 상속됨
	virtual void GetView(Matrix * out) override;
	virtual void GetProjection(Matrix * out) override;
	virtual void GetPlanes_Side4(Plane * planes) override;

public:
	void PreRender();
	void GetEnvProjection(Matrix * out);
	void GetEnvViews_Side6(Matrix * out);
	Transform* GetTransform() { return transform; }
	Projection* GetProjection() { return projection; }
	void SetProjection(float width, float height, float zNear, float zFar, float fov);
	
	ID3D11ShaderResourceView* GetSRV() { return srv; }

private:
	Transform* transform;
	float width, height;

	ID3D11Texture2D* rtvTexture;
	ID3D11RenderTargetView* rtv;
	ID3D11ShaderResourceView* srv;

	ID3D11Texture2D* dsvTexture;
	ID3D11DepthStencilView* dsv;

	Viewport* viewport;
	Projection* projection;
	Matrix forword;
};