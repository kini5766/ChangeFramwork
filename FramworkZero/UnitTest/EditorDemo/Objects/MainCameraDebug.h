#pragma once

#include "../IObjectDebuger.h"

class MainCameraDebug : public IObjectDebuger
{
public:
	MainCameraDebug();
	~MainCameraDebug();

public:
	// IObjectDebuger을(를) 통해 상속됨
	void Render() override;
	void SetObject(void * value) override {}
};