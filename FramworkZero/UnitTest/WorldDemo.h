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

class WorldDemo : public IExecute
{
public:
	// IExecute��(��) ���� ��ӵ�
	void Initialize() override;
	void Destroy() override;
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override {};
	void ResizeScreen() override {};

private:
	void LoadScene();
	void Billboards();

private:
	Shader* shader;
	CubeSky* sky;
	Billboard* billboard;

private:
	class WorldPlayer* player;
	class SceneValue* scene;
	class WorldLightGroup* lights;
	class EnemyInstancing* meleeEnemy;
	class EnemyInstancing* magicianEnemy;
};
