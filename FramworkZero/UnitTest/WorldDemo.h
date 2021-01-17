#pragma once
#include "Systems/IExecute.h"

/*
<�⺻����>
�޸��� Ȥ�� ũ��Ƽ�� �� ��ó��ȿ��
������ -> �������� �ؽ��� -> ��ƼŬ ���(1ȸ�� ��ƼŬ) -> ������ó��
�׸��� ����

<��������>
���� �� �� ���� ��ƼŬ ����
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
	CubeSky* sky;
	Billboard* billboard;

private:
	class WorldPlayer* player;
	class SceneValue* scene;
	class WorldLightGroup* lights;
	class EnemyInstancing* meleeEnemy;
	class EnemyInstancing* magicianEnemy;
};
