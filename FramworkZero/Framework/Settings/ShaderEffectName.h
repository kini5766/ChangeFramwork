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
	const string CB_SpotLights = "CB_SpotLights";
	const string CB_PointLights = "CB_PointLights";
	const string CB_BurntLight = "CB_BurntLight";

	// 02_AnimationMap.fx
	const string CB_BonesMap = "CB_BonesMap";
	const string CB_BlendingFrame = "CB_BlendingFrame";

}
