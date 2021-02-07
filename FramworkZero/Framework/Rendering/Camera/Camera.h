#pragma once

// Context 클래스에서 Perframe으로 넘겨 줄 값
class ICamera
{
public:
	virtual void GetView(Matrix* out) = 0;
	virtual void GetProjection(Matrix* out) = 0;
	virtual void GetPlanes4(Plane* planes) = 0;
};

// 메인 카메라용 카메라
class Camera : public ICamera
{
public:
	Camera(unique_ptr<Projection> projection);
	virtual ~Camera();

public:
	// ICamera을(를) 통해 상속됨
	virtual void GetView(Matrix * out) override;
	virtual void GetProjection(Matrix * out) override;
	virtual void GetPlanes4(Plane * planes) override;

public:
	virtual void Update();
	virtual void ResizeScreen(float width, float height);

public:
	CameraTransform* GetTransform() { return transform; }
	Projection* GetProjection() { return projection.get(); }

protected:
	CameraTransform* transform;
	unique_ptr<Projection> projection;
	Frustum* frustum;
};
