#pragma once

class Lighting
{
public:
	static Lighting* Get();

	static void Create();
	static void Delete();

private:
	static Lighting* instance;

private:
	Lighting();
	~Lighting();

public:
	DirectionalLight* GetDirectional() { return directional; }
	BurntLight* GetBurntLight() { return burntLight; }

public:
	void AddPointLight(const PointLightDesc& light);
	PointLight* GetPointLight(UINT index);

public:
	void AddSpotLight(const SpotLightDesc& light);
	SpotLight* GetSpotLight(UINT index);

public:
	// Perframe
	DirectionalLightDesc* DirectionalDesc() { return &directionalDesc; }
	PointLightsDesc* PointDesc() { return &pointsDesc; }
	SpotLightsDesc* SpotDesc() { return &spotsDesc; }
	BurntLightDesc* BurntDesc() { return &burntDesc; }


private:
	DirectionalLightDesc directionalDesc;
	PointLightsDesc pointsDesc;
	SpotLightsDesc spotsDesc;
	BurntLightDesc burntDesc;
	
	DirectionalLight* directional;  // 기즈모
	vector<PointLight*> pointLights;  // 기즈모
	vector<SpotLight*> spotLights;  // 기즈모
	BurntLight* burntLight;  // 기즈모
};
