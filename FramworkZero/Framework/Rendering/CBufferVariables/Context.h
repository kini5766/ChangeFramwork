#pragma once

class Context
{
public:
	static Context* Get();

	static void Create();
	static void Delete();

private:
	static Context* instance;

private:
	Context();
	~Context();

public:
	Camera* MainCamera() { return mainCamera.get(); }
	void MainCamera(unique_ptr<Camera> value);
	class CanvasCamera* Canvas();

public:
	D3DXMATRIX View();
	D3DXMATRIX Projection();

	Viewport* GetViewport() { return viewport; }

public:
	Plane* Culling() { return culling; }
	//Plane& Culling(UINT index) { return culling[index]; }
	Plane& Clipping() { return clipping; }

public:
	void ResizeScreen();

	void Update();
	void Render();
	void PreRenderMain();
	void PreRender(ICamera* preRenderCamera);
	void PostRender();

private:
	ICamera* currCamera = nullptr;
	Viewport* viewport;
	unique_ptr<Camera> mainCamera;
	CanvasCamera* canvasCamera;

private:
	Plane culling[4];
	Plane clipping;
};
