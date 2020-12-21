#pragma once

namespace ShaderEffectName
{


	// --------------------------------------------------------
	// Constant Buffer
	// --------------------------------------------------------

	// 00_Global.fx
	const string CB_PerFrame = "CB_PerFrame";
	const string CB_World = "CB_World";


	// 00_Light.fx
	const string CB_Light = "CB_Light";
	const string CB_Material = "CB_Material";

	// 02_AnimationMap.fx
	const string CB_BonesMap = "CB_BonesMap";
	const string CB_Keyframes = "CB_Keyframes";

	// 00_Terrain.fx
	const string CB_Brush = "CB_Brush";
	const string CB_TerrainLine = "CB_TerrainLine";

}
