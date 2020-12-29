#pragma once
#include "Systems/IExecute.h"

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

private:
	Shader* shader;
	MeshInstancing* plane;
	MeshInstancing* box;
	ModelSkinnedInstancing* kachujin;
	Animator* kachujinAnimator;
};
