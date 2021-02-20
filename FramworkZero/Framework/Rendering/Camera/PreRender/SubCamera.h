#pragma once

// View Projection ���� ������ PreRender�� ī�޶�

class SubCamera : public PreCamera
{
public:
	// width, height�� ���� Ÿ�� �ؽ��� �ػ�(projection �ػ󵵿ʹ� �ٸ�)
	SubCamera(float width = 0, float height = 0, Projection* projection = nullptr);
	virtual ~SubCamera();

public:
	virtual void Update();

public:
	// ICamera��(��) ���� ��ӵ�
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
