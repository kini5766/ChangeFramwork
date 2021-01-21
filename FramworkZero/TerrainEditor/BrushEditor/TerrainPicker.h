#pragma once

class TerrainPicker
{
public:
	TerrainPicker();
	~TerrainPicker();

public:
	bool RenderImGui();
	void AddPicker(const Vector3& position);

public:
	bool IsPickMode() { return bPickMode; }
	Transform* Pick1() { return picks[0]; }
	Transform* Pick2() { return picks[1]; }

private:
	bool bPickMode = false;
	Transform* picks[2];
};