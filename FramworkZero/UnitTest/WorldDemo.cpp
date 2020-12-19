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

static int test = 0;
void WorldDemo::Update()
{
	ImGui::InputInt("Test", &test);
}

void WorldDemo::Render()
{
}
