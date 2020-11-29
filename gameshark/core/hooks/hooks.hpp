#pragma once

#include "../../dependencies/utilities/detourhook.h"

namespace detour
{
	inline CDetourHook frame_stage_notify;
	inline CDetourHook create_move;
	inline CDetourHook paint_traverse;
	inline CDetourHook lock_cursor;
	inline CDetourHook post_entity;
	inline CDetourHook shutdown;
	inline CDetourHook post_screen_effects;
	inline CDetourHook draw_model;
	inline CDetourHook scene_end;
	inline CDetourHook override_config;
}

namespace hooks {
	inline constexpr void* GetVFunc(void* thisptr, std::size_t nIndex)
	{
		return (void*)((*(std::uintptr_t**)thisptr)[nIndex]);
	}

	bool initialize();

	bool	__fastcall	hkCreateMove(void*, int, float, c_usercmd*);
	void	__fastcall	hkPaintTraverse(void*, int, unsigned int, bool, bool);
	void	__fastcall	hkLockCursor(void*, int);
	void	__fastcall	hkFrameStageNotify(void*, int, int);
	void	__fastcall	hkLevelInitPostEntity(void*, void*);
	void	__fastcall	hkLevelShutdown(void*, void*);
	int		__fastcall	hkDoPostScreenEffects(void*, int, void*);
	void	__fastcall	hkSceneEnd(void*);
	bool    __fastcall  hkOverrideConfig(void*, void*, MaterialSystemConfig_t*, bool);
	void	__fastcall	hkDrawModel(void*, int, DrawModelResults_t*, const DrawModelInfo_t&, matrix3x4_t*, float*, float*, const vec3_t&, int);

}
