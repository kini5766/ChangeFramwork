#pragma once

class ImGuiInputText
{
public:
	ImGuiInputText(UINT maxSize);
	~ImGuiInputText();

public:
	void Render(const char* label);

	void Text(const char* value);
	char* Text() { return text; }
	UINT MaxSize() { return maxSize; }

private:
	char* text;
	UINT maxSize;
};