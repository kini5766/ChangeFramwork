#pragma once

// I_ 이름 중복 땜시
class I_Context
{
public:
	virtual D3DXMATRIX View() = 0;
	virtual D3DXMATRIX Projection() = 0;

	virtual Perspective* GetPerspective() = 0;
	virtual Viewport* GetViewport() = 0;

	virtual Camera* MainCamera() = 0;
	virtual void MainCamera(Camera* value) = 0;

	virtual Color& Ambient() = 0;
	virtual Color& Specular() = 0;
	virtual Vector3& Direction() = 0;
	virtual Vector3& Position() = 0;

	virtual DirectionalLight* Light() = 0;
	virtual void Light(DirectionalLight* value) = 0;
};


class Context : public I_Context
{
public:
	static I_Context* Get();
	static void Set(I_Context* value);

private:
	static I_Context* instance;

public:
	Context();
	~Context();

public:
	// IContext을(를) 통해 상속됨
	D3DXMATRIX View() override;
	D3DXMATRIX Projection() override;

	Perspective* GetPerspective() override { return camera->GetPerspective(); }
	Viewport* GetViewport() override { return camera->GetViewport(); }

	Camera* MainCamera() override { return camera; }
	void MainCamera(Camera* value) override;

	Color & Ambient() override { return light->Ambient(); }
	Color & Specular() override { return light->Specular(); }
	Vector3 & Direction() override { return light->Direction(); }
	Vector3 & Position() override { return light->Position(); }

	DirectionalLight * Light() override { return light; }
	void Light(DirectionalLight * value) override;

public:
	void ResizeScreen();

	void Update();
	void Render();

private:
	Camera* camera;
	DirectionalLight* light;
};
