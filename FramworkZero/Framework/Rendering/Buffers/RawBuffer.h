#pragma once

// input : �ַ� �迭�� ���
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
	void CopyToInput(void* data);  // vRam���� data�� �������ִ� �Լ�
	void CopyFromOutput(void* data);  // vRam���� ���� �����ϴ� �Լ�

private:
	void* inputData;
	UINT inputByte;
	UINT outputByte;
};
