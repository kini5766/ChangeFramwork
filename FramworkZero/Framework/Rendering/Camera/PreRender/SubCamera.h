#pragma once

// View Projection 설정 가능한 PreRender용 카메라

class SubCamera : public PreCamera
{
public:
	// width, height는 렌더 타겟 텍스쳐 해상도(projection 해상도와는 다름)
	SubCamera(float width = 0, float height = 0, Projection* projection = nullptr);
	virtual ~SubCamera();

public:
	virtual void Update();

public:
	// ICamera을(를) 통해 상속됨
	virtual void GetView(Matrix * out) override;
	virtual void GetProjection(Matrix * out) override;
	virtual void GetPlanes_Side4(Plane * planes) override;

public:
	CameraTransform* GetTransform() { return transform; }

	void SetProjection(Projection* value) { projection = value; }
	Projection* GetProjection() { return projection; }

protected:
	CameraTransform* transform;
	Projection* projection = nullptr;
	Frustum* frustum;
};
