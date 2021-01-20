#pragma once

// input에 주로 Texture2D를 사용
class TextureBuffer : public CsResource
{
public:
	TextureBuffer(ID3D11Texture2D* src);
	~TextureBuffer();

private:
	void CreateSRV() override;

	void CreateOutput() override;
	void CreateUAV() override;

public:
	UINT Width() { return srcDesc->Desc().Width; }
	UINT Height() { return srcDesc->Desc().Height; }
	UINT ArraySize() { return srcDesc->Desc().ArraySize; }

	ID3D11ShaderResourceView* OutputSRV() { return outputSRV; }
	ID3D11Texture2D* Result() { return result; }

	void CopyToInput(ID3D11Texture2D* texture);
	ID3D11Texture2D* CopyFromOutput();

private:
	Texture2DDesc* srcDesc;
	//UINT width, height, arraySize = 1;
	//DXGI_FORMAT format;

	ID3D11ShaderResourceView* outputSRV;
	ID3D11Texture2D* result;

private:
};
