#pragma once

// input : 주로 배열로 사용
class RawBuffer : public CsResource
{
public:
	RawBuffer(void* inputData, UINT inputByte, UINT outputByte);
	~RawBuffer();

	void CreateInput() override;
	void CreateSRV() override;
	void CreateOutput() override;
	void CreateUAV() override;

public:
	void CopyToInput(void* data);  // vRam으로 data를 복사해주는 함수
	void CopyFromOutput(void* data);  // vRam으로 부터 복사하는 함수

private:
	void* inputData;
	UINT inputByte;
	UINT outputByte;
};
