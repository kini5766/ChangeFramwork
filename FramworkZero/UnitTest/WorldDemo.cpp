#include "stdafx.h"
#include "WorldDemo.h"

WorldDemo::~WorldDemo()
{
}

void WorldDemo::Initialize()
{
}

void WorldDemo::Destroy()
{
}

static float test = 0;
void WorldDemo::Update()
{
	ImGui::InputFloat("Test", &test);
}

void WorldDemo::Render()
{
}
