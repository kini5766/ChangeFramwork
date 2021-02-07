#pragma once

class SubCamera : public PreCamera
{
public:
	// width, height는 렌더 타겟 텍스쳐 해상도(projection 해상도와는 다름)
	SubCamera(unique_ptr<Projection> projection, float width = 0, float height = 0);
	virtual ~SubCamera();

public:
	virtual void Update();

public:
	// ICamera을(를) 통해 상속됨
	virtual void GetView(Matrix * out) override;
	virtual void GetProjection(Matrix * out) override;
	virtual void GetPlanes4(Plane * planes) override;

public:
	CameraTransform* GetTransform() { return transform; }
	Projection* GetProjection() { return projection.get(); }

protected:
	CameraTransform* transform;
	unique_ptr<Projection> projection;
	Frustum* frustum;
};
