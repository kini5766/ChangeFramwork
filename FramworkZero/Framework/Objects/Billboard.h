#pragma once

constexpr auto BILLBOARD_MAX_COUNT = 10000;

class Billboard
{
public:
	Billboard();
	~Billboard();

public:
	void SetTextures(vector<wstring> textureFiles);
	void Add(const Vector3& position, const Vector2& scale, UINT mapIndex);

public:
	void Update();
	void Render();

private:
	ShaderSetter* shader;
	PerFrame* perFrame;
	VertexBuffer* vertexBuffer;
	UINT vertexCount = 0;

	// w, h, format 동일해야함
	TextureArray* textureArray = nullptr;
	vector<wstring> textureFiles;

private:
	struct VertexBillboard
	{
		Vector3 Position;  // 월드좌표
		Vector2 Scale;  // 월드 크기
		UINT MapIndex;
	};
	vector<VertexBillboard> vertices;
};