#include "Framework.h"
#include "DebugLog.h"

DebugLog::DebugLog()
{
}

DebugLog::~DebugLog()
{
}

void DebugLog::Print(string log)
{
	logs.insert(logs.begin(), log);
}

void DebugLog::Show(string log)
{
	views.push_back(log);
}

void DebugLog::Render()
{
	if (views.size() == 0) return;

	ImGui::Begin("Debug Logs");
	for (string& view : views)
	{
		ImGui::Text(view.c_str());
	}
	views.clear();

	if (logs.size() == 0) { ImGui::End(); return; }
	ImGui::Separator();
	for (string& log : logs)
	{
		ImGui::Text(log.c_str());
	}
	ImGui::End();
}
