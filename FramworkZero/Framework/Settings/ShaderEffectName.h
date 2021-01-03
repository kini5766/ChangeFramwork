#pragma once


namespace ShaderEffectName
{
	// 00_Global.fx
	constexpr auto CB_PERFRAME = "CB_PerFrame";
	constexpr auto CB_WORLD = "CB_World";

	// 00_Light.fx
	constexpr auto CB_MATERIAL = "CB_Material";
	constexpr auto DIFFUSEMAP = "DiffuseMap";
	constexpr auto SPECULARMAP = "SpecularMap";
	constexpr auto NORMALMAP = "NormalMap";

	constexpr auto CB_LIGHT = "CB_Light";
	constexpr auto CB_LIGHT_SPOT = "CB_SpotLights";
	constexpr auto CB_LIGHT_POINT = "CB_PointLights";
	constexpr auto CB_LIGHT_BURNT = "CB_BurntLight";

	// 00_Render.fx
	constexpr auto INV_BINDPOSE = "InvBindPose";
	constexpr auto BONESMAP = "BonesMap";

	// --------------------------------------------------------

	// 02_AnimationInstance.fx
	constexpr auto CB_BLENDING_FRAME = "CB_BlendingFrame";
	constexpr auto OUTPUT = "Output";

	// 02_GetBonesInstance.fx
	constexpr auto OUTPUT_SKINNED = "OutputSkinned";

	// 02_SetAnimation.fx
	constexpr auto INPUT_CLIP_MAP = "InputClipMap";
	constexpr auto INPUT_KEYFRAME_COUNTS = "InputKeyframeCount";

	// 02_SetBones.fx
	constexpr auto INPUT_LOCAL_BONES = "InputLocalBones";
	constexpr auto INPUT_BONE_DESC = "InputboneDesc";

}


