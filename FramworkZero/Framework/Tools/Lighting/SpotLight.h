#pragma once

class SpotLight
{
public:
	SpotLight(SpotLightDesc* desc);
	~SpotLight();

	SpotLightDesc* GetDesc() { return desc; }
	Transform* GetTransform() { return transform; }

	void ApplyLight();
	void ApplyTransform();

private:
	SpotLightDesc* desc;
	Transform* transform;
};