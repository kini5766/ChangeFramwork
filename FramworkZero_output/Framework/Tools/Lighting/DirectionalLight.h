#pragma once


class DirectionalLight
{
public:
	DirectionalLight(DirectionalLightDesc* desc);
	~DirectionalLight();

	DirectionalLightDesc* GetDesc() { return desc; }
	Transform* GetTransform() { return transform; }

	void ApplyLight();
	void ApplyTransform();

private:
	DirectionalLightDesc* desc;
	Transform* transform;
};