#pragma once

// ----------------------------------------------------------------------------
// Transform
// ----------------------------------------------------------------------------

class Transform
{
public:
	Transform();
	Transform(Matrix* pMatrixGetter);
	~Transform();


#pragma region Transform

public:
	void Set(Transform* value);  // ����

	void Position(float x, float y, float z);
	void Position(const Vector3& value);
	void Position(Vector3* out);

	void Scale(float x, float y, float z);
	void Scale(const Vector3& value);
	void Scale(Vector3* out);

	void RotationDegree(float x, float y, float z);
	void RotationDegree(const Vector3& value);
	void RotationDegree(Vector3* out);  // ���ʹϿ¿��� ��ȯ�� �Ŷ� ���� ������ ��� �뵵�θ� ���

	void Rotation(float x, float y, float z);
	void Rotation(const Vector3& value);

	void Rotation(const Quaternion& value);
	void Rotation(Quaternion* out);

	void RotateYawDegree(float deg);
	void RotateYaw(float rad);

	void RotatePitchDegree(float deg);
	void RotatePitch(float rad);

	void RotateRollDegree(float deg);
	void RotateRoll(float rad);

	Vector3 Forward();
	Vector3 Up();
	Vector3 Right();

	void LocalWorld(const Matrix& set);
	void LocalWorld(Matrix* out);

private:
	Vector3 position;
	Vector3 scale;
	Quaternion rotation;

#pragma endregion


#pragma region World Matrix

public:
	void LossyWorld(const Matrix& set);
	void LossyWorld(Matrix* out);
	void UpdateWorld();
	void ReplaceMatrixGetter(Matrix* pMatrixGetter);

private:
	void ChangedWorld();

private:
	Matrix* lossyWorld;
	bool changed = false;
	bool bMyWorld;

#pragma endregion


#pragma region TransformFamily

public:
	void SetParent(Transform* value);
	void UnLink();  // �θ� �ڽ� ���� ����
	Transform* GetParent() { return parent; }
	Transform** GetChilds() { return childs.data(); }
	UINT ChildCount() const { return childs.size(); }

private:
	void AddThis(Transform* oldParent, Transform* newparent);  // �θ� �ڽ� �ֱ�
	void RemoveThis(Transform* oldParent);  // �θ� �ڽ� ����

private:
	Transform* parent = nullptr;
	vector<Transform*> childs;

#pragma endregion

};
