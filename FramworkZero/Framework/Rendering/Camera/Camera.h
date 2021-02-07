#pragma once

// Context Ŭ�������� Perframe���� �Ѱ� �� ��
class ICamera
{
public:
	virtual void GetView(Matrix* out) = 0;
	virtual void GetProjection(Matrix* out) = 0;
	virtual void GetPlanes4(Plane* planes) = 0;
};

// ���� ī�޶�� ī�޶�
class Camera : public ICamera
{
public:
	Camera(unique_ptr<Projection> projection);
	virtual ~Camera();

public:
	// ICamera��(��) ���� ��ӵ�
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
