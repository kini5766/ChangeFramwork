#pragma once

#include "../IObjectDebuger.h"

class MainCameraDebug : public IObjectDebuger
{
public:
	MainCameraDebug();
	~MainCameraDebug();

public:
	// IObjectDebuger��(��) ���� ��ӵ�
	void Render() override;
	void SetObject(void * value) override {}
};