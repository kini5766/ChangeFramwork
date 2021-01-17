#pragma once
#include "Systems/IExecute.h"

/*
<기본과제>
달리기 혹은 크리티컬 시 후처리효과
마법진 -> 프로젝션 텍스쳐 -> 파티클 재생(1회성 파티클) -> 데미지처리
그림자 적용

<도전과제>
공격 시 검 궤적 파티클 적용
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
	CubeSky* sky;
	Billboard* billboard;

private:
	class WorldPlayer* player;
	class SceneValue* scene;
	class WorldLightGroup* lights;
	class EnemyInstancing* meleeEnemy;
	class EnemyInstancing* magicianEnemy;
};
