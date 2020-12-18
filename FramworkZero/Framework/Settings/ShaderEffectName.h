#pragma once

namespace ShaderEffectName
{
	// 20_Animation
	const string TransformsMap = "TransformsMap";


	// --------------------------------------------------------
	// Constant Buffer
	// --------------------------------------------------------

	// 00_Global.fx
	const string CB_PerFrame = "CB_PerFrame";
	const string CB_World = "CB_World";

	// 00_Light.fx
	const string CB_Light = "CB_Light";
	const string CB_Material = "CB_Material";

	// 00_Render.fx, 20_Animation
	const string CB_Bone = "CB_Bone";
	//const string CB_KeyFrames = "CB_KeyFrames";
	const string CB_AnimationFrame = "CB_AnimationFrame";
	const string CB_BlendingFrame = "CB_BlendingFrame";

	// 00_Terrain.fx
	const string CB_Brush = "CB_Brush";
	const string CB_TerrainLine = "CB_TerrainLine";

	// 24_GetBones.fx
}
