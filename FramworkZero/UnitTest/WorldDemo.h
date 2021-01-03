#pragma once
#include "Systems/IExecute.h"

/*
���� 2) �� ���� �����
�׸���
1. ���� ������ �÷��̾�(Ű���� �Ǵ� ���콺)
2. �� 10���� �̻�
2-1) ���� ���� ���� -> ���� �Ǹ� �÷��̾� �������� �i�ƿ�
2-2) ���� ���� -> ���� ���� �ȿ� ������ ����
2-3) ���� ���� ���� �� -> ��Ʈ��(�͵�����)
3. ���� ����
3-1) ������ ����x
4. ���Ÿ� ����(ȭ��)
*/

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
	void Colliders();

private:
	Shader* shader;
	ModelSkinnedInstancing* kachujin;
	class KachujinInstance* character;
	class CharacterController* player;

	class SceneValue* scene;
	SendBox* sendbox1;
	SendBox* sendbox2;
	SendBox* sendbox3;
	int message;
};
