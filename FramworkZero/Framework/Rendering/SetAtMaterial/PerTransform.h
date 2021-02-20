#pragma once

class PerTransform
{
public:
	PerTransform();
	~PerTransform();

public:
	void Update();
	void Render();

public:
	void SetAtMaterial(ShaderSetter* target);
	Transform* GetTransform() { return transform; }

private:
	PerFrame* perFrame;
	Transform* transform;
	ConstantBuffer* worldBuffer;

	Matrix world;
};