#pragma once

// Ʈ������ ����, �ִϸ��̼� ����
class ModelRenderer
{
public:
	ModelRenderer(const ModelDesc& desc);
	~ModelRenderer();

public:
	void Update();
	void Render();
	void RenderInstance(UINT drawCount);

public:
	void Pass(UINT value) { renderer->Pass(value); }
	ModelData* GetModel() { return data; }
	SkinnedMeshRenderer* GetMeshRenderer() { return renderer; }
	void SetShader(Shader* shader, wstring name);
	void SetShader(Shader* shader, UINT index);
	Shader* GetDefaultShader() { return defaultShader; }

private: // render ����
	Shader* defaultShader;
	PerFrame* perframe;
	ModelData* data;
	SkinnedMeshRenderer* renderer;
};