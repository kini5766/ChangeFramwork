#pragma once

class Viewport
{
public:
	Viewport(float width, float height, float x = 0, float y = 0, float minDepth = 0, float maxDepth = 1);
	~Viewport();

	void RSSetViewport();
	void Set(float width, float height, float x = 0, float y = 0, float minDepth = 0, float maxDepth = 1);

public:
	void Resize(float width, float height);
	float GetWidth() { return width; }
	float GetHeight() { return height; }

public:
	Vector3 Project(const Vector3& source, const Matrix& W, const Matrix& V, const Matrix& P);
	Vector3 Unproject(const Vector3& source, const Matrix& W, const Matrix& V, const Matrix& P);
	void GetMouseRay(Vector3* outPosition, Vector3* outDirection, Matrix* world = nullptr);

private:
	float x, y;
	float width, height;
	float minDepth, maxDepth;

	D3D11_VIEWPORT viewport;
};