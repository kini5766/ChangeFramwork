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

/*
과제 1) 실내조명 만들기 (포인트라이트, 스팟라이트)
땅 : 그리드
큐브로 터널 만들기
터널안에 포인트 라이트 비추기
캐릭터가 터널 안에 들어오면 조명 삭제
터널 안 캐릭터가 보이게 카메라 옮기기
*/

class WorldDemo : public IExecute
{
public:
	// IExecute을(를) 통해 상속됨
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
