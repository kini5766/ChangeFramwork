#pragma once
#include "Systems/IExecute.h"

class WorldDemo : public IExecute
{
public:
	~WorldDemo();

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

	VertexColor vertices[6];
	ID3D11Buffer* vertexBuffer;

	Matrix world;
	Matrix world2;
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