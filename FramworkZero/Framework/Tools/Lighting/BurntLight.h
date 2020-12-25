#pragma once

class BurntLight
{
public:
	BurntLight(BurntLightDesc* desc);
	~BurntLight();

	BurntLightDesc* GetDesc() { return desc; }
	Transform* GetTransform() { return transform; }

	void ApplyLight();
	void ApplyTransform();

private:
	BurntLightDesc* desc;
	Transform* transform;
};