#pragma once

class PointLight
{
public:
	PointLight(PointLightDesc* desc);
	~PointLight();

	PointLightDesc* GetDesc() { return desc; }
	Transform* GetTransform() { return transform; }

	void ApplyLight();
	void ApplyTransform();

private:
	PointLightDesc* desc;
	Transform* transform;
};