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
	ImGui::PushID(this); // �ٸ� ���� ���� ���̵��� ���� ����� ������
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
