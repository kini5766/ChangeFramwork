#pragma once
#include "Systems/IExecute.h"

// index buffer는 그리는 순서를 넣는 버퍼

// 터레인 과제 (방학 때)
// far cry map editor
// -> 브러쉬
// 1. 뾰족 -> 스무스 (주변 높이 정보를 평탄화)
// 2. 내리기
// 3. 원모양
// 4. 노이즈 (울퉁불퉁 랜덤으로)
// 5. slope (2개의 점을 누르면 비탈길 생성 너비는 지정한 arnge)
// ex. 지형이 바껴도 처음 누른 위치 그대로 적용

class TerrainDemo : public IExecute
{
public:
	~TerrainDemo();

	// IExecute을(를) 통해 상속됨
	void Initialize() override;
	void Ready() override {};
	void Destroy() override;
	void Update() override;
	void PreRender() override {};
	void Render() override;
	void PostRender() override {};
	void ResizeScreen() override {};

private:
	Shader* shader;
	class Terrain* terrain;
};

// IA -> VS -> RS -> PS -> OM
// IA : 인풋어셈블 cpu -> gpu
// 양대 자원 : Vertex Buffer, Texture

// VS : 점을 찍고, NDC공간 -> 월드공간 변환

// RS : (하는일이 많아서 느림) 3d -> 2d (NDC공간)
// RendertargetView
// DepthStencilView

// PS : 포토샵

// OM : (아웃풋머지) 랜더타겟뷰 + 뎁스스텐실 합침
// 랜더타겟뷰, 뎁스스텐실뷰 사이즈 같아야함

// 화면 안쪽으로 들어가면 왼손
// NDC 좌표 : 비치볼

// 리소스 : 버퍼
// 서브리소스다타 : 버퍼 타고 가는 놈