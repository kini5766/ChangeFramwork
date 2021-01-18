#pragma once

class CanvasCamera : public Camera
{
public:
	CanvasCamera();
	~CanvasCamera();

	void Update() override {}
	void ResizeScreen(float width, float height) override;

private:
	class UIProjection* uiProjection;
};