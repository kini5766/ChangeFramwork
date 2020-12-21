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
	Camera* MainCamera() { return camera; }
	void MainCamera(Camera* value);

public:
	D3DXMATRIX View();
	D3DXMATRIX Projection();

	Perspective* GetPerspective() { return camera->GetPerspective(); }
	Viewport* GetViewport() { return camera->GetViewport(); }

public:
	void ResizeScreen();

	void Update();
	void Render();

private:
	Camera* camera;
};
