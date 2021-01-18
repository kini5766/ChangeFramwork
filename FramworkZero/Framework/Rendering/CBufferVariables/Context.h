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

	Viewport* GetViewport() { return mainCamera->GetViewport(); }

public:
	void ResizeScreen();

	void Update();
	void Render();

private:
	unique_ptr<Camera> mainCamera;
	class CanvasCamera* canvasCamera;
};
