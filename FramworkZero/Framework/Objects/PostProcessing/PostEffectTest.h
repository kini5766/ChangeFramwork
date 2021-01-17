#pragma once

class PostEffectTest
{
public:
	PostEffectTest();
	~PostEffectTest();

public:
	void Update();
	void BeginPreRender();
	void EndPreRender();
	void Render();
	void ImGuiRender();

public:
	void None();
	void OnSaturation();
	void OnVignette();
	void OnWiggle();
	void OnLensDistortion();
	void OnRaialBlur();


private:
	Shader* shaderPostEffect;
	PostEffect* postEffect;

	class Saturation* saturation;
	class Vignette* vigentte;
	class Wiggle* wiggle;
	class LensDistortion* lens;
	class RaialBlur* raialBlur;

private:
	int mode;

};