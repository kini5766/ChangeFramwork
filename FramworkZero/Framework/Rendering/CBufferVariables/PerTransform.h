#pragma once

class PerTransform
{
public:
	PerTransform(Shader* shader);
	~PerTransform();

public:
	void Update();
	void Render();

public:
	Transform* GetTransform() { return transform; }

private:
	PerFrameBuffer* perFrame;
	Transform* transform;

	ShaderSetter* shader;
	ConstantBuffer* worldBuffer;

	Matrix world;
};