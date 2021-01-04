#pragma once

class CanvasCamera : public Camera
{
public:
	CanvasCamera();
	~CanvasCamera();

	void Update() override {}
	void ResizeScreen(float width, float height) override;

public:
	D3DXMATRIX View();
	D3DXMATRIX Projection();

private:
	D3DXMATRIX view;
	D3DXMATRIX projection;
};