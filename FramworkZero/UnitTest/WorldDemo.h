#pragma once
#include "Systems/IExecute.h"

/*
���� 1) �ǳ����� ����� (����Ʈ����Ʈ, ���̶���Ʈ)
�� : �׸���
ť��� �ͳ� �����
�ͳξȿ� ����Ʈ ����Ʈ ���߱�
ĳ���Ͱ� �ͳ� �ȿ� ������ ���� ����
�ͳ� �� ĳ���Ͱ� ���̰� ī�޶� �ű��
*/

class WorldDemo : public IExecute
{
public:
	// IExecute��(��) ���� ��ӵ�
	void Initialize() override;
	void Destroy() override;
	void Update() override;
	void PreRender() override {};
	void Render() override;
	void PostRender() override {};
	void ResizeScreen() override {};

private:
	void Mesh();
	void Kachujin();
	void BurntLight();
	void PointLights();
	void SpotLights();

private:
	Shader* shader;
	MeshInstancing* plane;
	MeshInstancing* box;
	ModelSkinnedInstancing* kachujin;
	Animator* kachujinAnimator;
};
