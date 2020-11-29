#pragma once

#define NOMINMAX
#include <windows.h>
#undef NOMINMAX

#define FMT_HEADER_ONLY
#include "../../dependencies/fmt/format.h"
#include "../../dependencies/fmt/chrono.h"

#include <cstdint>
#include <string>
#include <stdexcept>

#include "i_base_client_dll.hpp"
#include "i_client_entity_list.hpp"
#include "iv_engine_client.hpp"
#include "i_client_state.hpp"
#include "i_panel.hpp"
#include "i_surface.hpp"
#include "c_global_vars_base.hpp"
#include "i_material_system.hpp"
#include "iv_model_info.hpp"
#include "iv_model_render.hpp"
#include "iv_debug_overlay.hpp"
#include "i_console.hpp"
#include "i_localize.hpp"
#include "i_game_event_manager.hpp"
#include "i_input.hpp"
#include "i_input_system.hpp"
#include "i_trace.hpp"
#include "i_render_view.hpp"
#include "glow_manager.hpp"
#include "i_player_movement.hpp"
#include "i_weapon_system.hpp"

using MemAllocFailHandler_t = std::size_t(__cdecl*)(std::size_t);

class IMemAlloc
{
public:
	// Release versions
	virtual void* Alloc(std::size_t nSize) = 0;
	virtual void* Realloc(void* pMemory, std::size_t nSize) = 0;
	virtual void  Free(void* pMemory) = 0;
	virtual void* Expand(void* pMemory, std::size_t nSize) = 0; // no longer supported

	// Debug versions
	virtual void* Alloc(std::size_t nSize, const char* szFileName, int nLine) = 0;
	virtual void* Realloc(void* pMemory, std::size_t nSize, const char* szFileName, int nLine) = 0;
	virtual void  Free(void* pMemory, const char* szFileName, int nLine) = 0;
	virtual void* Expand(void* pMemory, std::size_t nSize, const char* szFileName, int nLine) = 0; // no longer supported

	// Returns size of a particular allocation
	virtual std::size_t GetSize(void* pMemory) = 0;
};


using CreateInterfaceFn = void* (__cdecl*)(const char*, int*);
using InstantiateInterfaceFn = void* (__cdecl*)(void);
using DemoCustomDataCallbackFn = void(__cdecl*)(unsigned char*, std::size_t);

class CInterfaceRegister
{
public:
	InstantiateInterfaceFn	pCreateFn;
	const char* szName;
	CInterfaceRegister* pNext;
};

namespace interfaces {
	enum class interface_type { index, bruteforce };

	template <typename T>
	T* get_interface(const char*, const std::string&);

	inline i_base_client_dll* client;
	inline i_input* input;
	inline i_client_entity_list* entity_list;
	inline iv_engine_client* engine;
	inline i_client_mode* clientmode;
	inline i_client_state* clientstate;
	inline i_panel* panel;
	inline i_surface* surface;
	inline c_global_vars_base* globals;
	inline i_material_system* material_system;
	inline iv_model_info* model_info;
	inline iv_model_render* model_render;
	inline i_render_view* render_view;
	inline iv_debug_overlay* debug_overlay;
	inline i_console* console;
	inline i_localize* localize;
	inline i_game_event_manager2* event_manager;
	inline i_inputsytem* inputsystem;
	inline IDirect3DDevice9* directx;
	inline trace* trace_ray;
	inline glow_manager_t* glow_manager;
	inline player_game_movement* game_movement;
	inline player_prediction* prediction;
	inline player_move_helper* move_helper;
	inline i_weapon_system* weapon_system;
	inline i_studio_render* studio_render;
	inline IMemAlloc* mem_alloc;

	bool initialize();
}
