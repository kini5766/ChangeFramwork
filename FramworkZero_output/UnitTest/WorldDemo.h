#pragma once
#include "Systems/IExecute.h"

/*
과제 2) 적 전투 만들기
그리드
1. 조작 가능한 플레이어(키보드 또는 마우스)
2. 적 10마리 이상
2-1) 적의 감지 범위 -> 감지 되면 플레이어 방향으로 쫒아옴
2-2) 공격 범위 -> 공격 범위 안에 들어오면 공격
2-3) 감지 범위 밖일 때 -> 패트롤(와따갔다)
3. 서로 공격
3-1) 죽으면 렌더x
4. 원거리 공격(화살)
*/

class WorldDemo : public IExecute
{
public:
	// IExecute을(를) 통해 상속됨
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
