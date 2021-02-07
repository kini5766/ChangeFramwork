#pragma once

class SubCamera : public PreCamera
{
public:
	// width, height�� ���� Ÿ�� �ؽ��� �ػ�(projection �ػ󵵿ʹ� �ٸ�)
	SubCamera(unique_ptr<Projection> projection, float width = 0, float height = 0);
	virtual ~SubCamera();

public:
	virtual void Update();

public:
	// ICamera��(��) ���� ��ӵ�
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
