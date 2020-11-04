#pragma once

struct SceneValue
{
	vector<Renderer*> RenderObjs;
	vector<ModelRender*> ModelObjs;
	vector<ModelAnimator*> ModelAnimObjs;

	void Clear()
	{
		RenderObjs.clear();
		ModelObjs.clear();
		ModelAnimObjs.clear();
	}
};
