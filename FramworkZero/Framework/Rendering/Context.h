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

	// camera
public:
	Camera* MainCamera() { return mainCamera.get(); }
	void MainCamera(unique_ptr<Camera> value);
	class CanvasCamera* Canvas();

	D3DXMATRIX View();
	D3DXMATRIX Projection();

	Viewport* GetViewport() { return viewport; }
	Plane* Culling() { return culling; }
	Plane& Clipping() { return clipping; }

private:
	ICamera* currCamera = nullptr;
	unique_ptr<Camera> mainCamera;
	CanvasCamera* canvasCamera;
	Viewport* viewport;

	Plane culling[4];
	Plane clipping;


	// win update
public:
	void ResizeScreen();
	void Update();
	void Render();
	void PreRenderMain();
	void PreRender(ICamera* preRenderCamera);
	void PostRender();


	// Shadow
public:
	class ShadowCaster* AddShadowCaster(const struct ShadowCasterInput& input);
	void PreRenderShadow(struct PreShadowValue* input);

private:
	list<class ShadowCaster*> shadowCasters;


	// EnvCubeMap
public:
	class EnvCubeCaster* AddEnvCubeCaster(const struct EnvCubeCasterInput& input);
	void PreRenderEnvCubeMap(struct PreEnvCubeDesc* input);

private:
	list<class EnvCubeCaster*> envCubeCasters;
};
