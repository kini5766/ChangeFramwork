#include "Framework.h"
#include "ImGuiSrvViewer.h"

void ImGuiSrvViewer::Render(string label, void * srv)
{
	ImGui::Begin(label.c_str());
	{
		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();
		ImVec2 v;
		v.x = vMax.x - vMin.x;
		v.y = vMax.y - vMin.y;

		ImGui::Image((void*)srv, v);
	}
	ImGui::End();
}



ImGuiSrvViewer::ImGuiSrvViewer(string label)
	: label(label)
{
}

ImGuiSrvViewer::~ImGuiSrvViewer()
{
}

void ImGuiSrvViewer::Render()
{
	Render(label, srv);
}
