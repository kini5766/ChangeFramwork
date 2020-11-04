#pragma once
#include "Systems/IExecute.h"

// index buffer는 그리는 순서를 넣는 버퍼

class WratherDemo : public IExecute
{
public:
	~WratherDemo();

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
	class CubeSky* sky;

	Shader* bbShader;
	class Billboard* bb[1000];

	class Rain* rain;
	class Snow* snow;

private:
	enum class Weather
	{
		None = 0, Rain, Snow
	} weather;
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




// 터레인
// 각진 지형 스무딩 (평균)
// 지형 울퉁불퉁 노이즈 (랜덤)
// 원형 지형 상승 하강
// 경사면 나름대로 정의해서 (선택)
// 경사면 (높이가 다른 두 지점을 마우스를 누르면 생김) 
// -> 경사의 폭 (임구이)

// 공부
//지오메트리쉐이더,
//헐쉐이더,
//테셀레이션,
//도메인쉐이더,
//컴퓨트쉐이더