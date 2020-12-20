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

void DebugLog::Render()
{
	if (logs.size() == 0) return;

	ImGui::Begin("Debug Logs");
	for (string log : logs)
	{
		ImGui::Text(log.c_str());
	}
	ImGui::End();
}
