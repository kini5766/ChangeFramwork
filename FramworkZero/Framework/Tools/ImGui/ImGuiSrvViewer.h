#pragma once

class ImGuiSrvViewer
{
public:
	static void Render(string label, void* srv);

public:
	ImGuiSrvViewer(string label);
	~ImGuiSrvViewer();

public:
	void Render();

public:
	void SetSRV(void* value) { srv = value; }

private:
	string label;
	void* srv;
};
