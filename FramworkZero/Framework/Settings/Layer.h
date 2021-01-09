#pragma once

constexpr unsigned int COLLIDER_LAYER_NONE = 0x00000000u;
constexpr unsigned int COLLIDER_LAYER_ALL = 0xFFFFFFFFu;
constexpr unsigned int COLLIDER_LAYER_DEFAULT = 0xFFFFFF0Fu;
constexpr unsigned int COLLIDER_LAYER_CAMERA = 0x00000002u;  // 1 << 1
constexpr unsigned int COLLIDER_LAYER_HITBOX = 0x00000020u;  // 1 << 5
constexpr unsigned int COLLIDER_LAYER_PLAYER = 0x00000040u;  // 1 << 6
constexpr unsigned int COLLIDER_LAYER_ENEMY = 0x00000080u;  // 1 << 7


constexpr auto COLLIDER_TAG_SENDBOX = L"SendBox";