#pragma once
#include "Systems/IExecute.h"

class DrowAnimationDemo : public IExecute
{
public:
	// IExecute을(를) 통해 상속됨
	void Initialize() override;
	void Ready() override {}
	void Destroy() override;
	void Update() override;
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}
	void ResizeScreen() override {}

private:
	void Kachujin();
	void Paladin();

private:
	Shader* shader;

	ModelAnimator* kachujin = nullptr;
	ModelAnimator* paladin = nullptr;
	//Model* weapon = nullptr;
};

// 과제 완
// 1. orbit
// 2. model editor
// 3. animation run

// LU Decompose atan2() 조사

// 과제
// 4. 무기 교체
// 힌트
// 최대한 인터럭트 발생안하게
// 미리 무기별로 srv를 제작

// 기능
// 1 unarmed (칼넣기)
// 2 get (칼뽑기)
// 3 choice (칼선택)
