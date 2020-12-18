#include "Framework.h"
#include "ImGuiInputText.h"

ImGuiInputText::ImGuiInputText(UINT maxSize)
	: maxSize(maxSize)
{
	text = new char[maxSize];
	ZeroMemory(text, sizeof(char) * maxSize);
}

ImGuiInputText::~ImGuiInputText()
{
	SafeDelete(text);
}

void ImGuiInputText::Render(const char * label)
{
	ImGui::PushID(this); // 다른 라벨의 같은 아이디의 값을 덮어쓰기 방지용
	ImGui::InputText(label, text, maxSize);
	ImGui::PopID();
}

void ImGuiInputText::Text(const char * value)
{
	ZeroMemory(text, sizeof(char) * maxSize);
	for (UINT i = 0; i < maxSize; i++)
	{
		if (value[i] == '\0')
			break;

		text[i] = value[i];
	}
}
