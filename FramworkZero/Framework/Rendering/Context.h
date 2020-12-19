#pragma once

class Context
{
public:
	static Context* Get();

	static void Create();
	static void Delete();

private:
	static Context* instance;

public:
	Context();
	~Context();

public:
	Camera* MainCamera() { return camera; }
	void MainCamera(Camera* value);

	DirectionalLight * Light() { return light; }
	void Light(DirectionalLight * value);

public:
	D3DXMATRIX View();
	D3DXMATRIX Projection();

	Perspective* GetPerspective() { return camera->GetPerspective(); }
	Viewport* GetViewport() { return camera->GetViewport(); }

	Color & Ambient() { return light->Ambient(); }
	Color & Specular() { return light->Specular(); }
	Vector3 & Direction() { return light->Direction(); }
	Vector3 & Position() { return light->Position(); }

public:
	void ResizeScreen();

	void Update();
	void Render();

private:
	Camera* camera;
	DirectionalLight* light;
};
