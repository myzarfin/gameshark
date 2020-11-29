#include "interfaces.hpp"
#include "../utilities/csgo.hpp"

bool interfaces::initialize() {
	client = get_interface<i_base_client_dll>("client.dll", "VClient");
	entity_list = get_interface<i_client_entity_list>("client.dll", "VClientEntityList");
	engine = get_interface<iv_engine_client>("engine.dll", "VEngineClient");
	panel = get_interface<i_panel>("vgui2.dll", "VGUI_Panel");
	surface = get_interface<i_surface>("vguimatsurface.dll", "VGUI_Surface");
	material_system = get_interface<i_material_system>("materialsystem.dll", "VMaterialSystem");
	model_info = get_interface<iv_model_info>("engine.dll", "VModelInfoClient");
	model_render = get_interface<iv_model_render>("engine.dll", "VEngineModel");
	render_view = get_interface<i_render_view>("engine.dll", "VEngineRenderView");
	console = get_interface<i_console>("vstdlib.dll", "VEngineCvar");
	localize = get_interface<i_localize>("localize.dll", "Localize_");
	event_manager = get_interface<i_game_event_manager2>("engine.dll", "GAMEEVENTSMANAGER002");
	debug_overlay = get_interface<iv_debug_overlay>("engine.dll", "VDebugOverlay");
	inputsystem = get_interface<i_inputsytem>("inputsystem.dll", "InputSystemVersion");
	trace_ray = get_interface<trace>("engine.dll", "EngineTraceClient");
	game_movement = get_interface<player_game_movement>("client.dll", "GameMovement");
	prediction = get_interface<player_prediction>("client.dll", "VClientPrediction");
	studio_render = get_interface<i_studio_render>("studiorender.dll", "VStudioRender");

	mem_alloc = *(IMemAlloc**)GetProcAddress(GetModuleHandle("tier0.dll"), ("g_pMemAlloc"));;

	clientmode = **reinterpret_cast<i_client_mode * **>((*reinterpret_cast<uintptr_t * *>(client))[10] + 0x5);
	globals = **reinterpret_cast<c_global_vars_base***>((*reinterpret_cast<uintptr_t**>(client))[0] + 0x1F);

	clientstate = **reinterpret_cast<i_client_state***>(utilities::pattern_scan("engine.dll", sig_client_state) + 1);
	directx = **reinterpret_cast<IDirect3DDevice9***>(utilities::pattern_scan("shaderapidx9.dll", sig_directx) + 1);
	input = *reinterpret_cast<i_input**>(utilities::pattern_scan("client.dll", sig_input) + 1);
	glow_manager = reinterpret_cast<glow_manager_t*>(*reinterpret_cast<uintptr_t*>(utilities::pattern_scan("client.dll", sig_glow_manager) + 3));
	move_helper = **reinterpret_cast<player_move_helper***>(utilities::pattern_scan("client.dll", sig_player_move_helper) + 2);
	weapon_system = *reinterpret_cast<i_weapon_system**>(utilities::pattern_scan("client.dll", sig_weapon_data) + 2);

	console::log("[setup] interfaces initialized!\n");

	return true;
}


template <typename T>
T* interfaces::get_interface(const char* module_name, const std::string& szInterface) {
	static auto GetRegisterList = [module_name]() -> CInterfaceRegister*
	{
		FARPROC oCreateInterface = nullptr;

		if (const auto hModule = GetModuleHandle(module_name); hModule != nullptr)
			oCreateInterface = GetProcAddress(hModule, ("CreateInterface"));

		if (oCreateInterface == nullptr)
			throw std::runtime_error(("Failed get createinterface address"));

		const std::uintptr_t	uCreateInterfaceJmp = (std::uintptr_t)oCreateInterface + 0x4;
		const std::int32_t		iJmpDisp = *(std::int32_t*)(uCreateInterfaceJmp + 0x1);
		const std::uintptr_t	uCreateInterface = uCreateInterfaceJmp + 0x5 + iJmpDisp;
		return **(CInterfaceRegister***)(uCreateInterface + 0x6);
	};

	for (auto pRegister = GetRegisterList(); pRegister != nullptr; pRegister = pRegister->pNext)
	{
		if ((!std::string(pRegister->szName).compare(0U, szInterface.length(), szInterface) &&
			atoi(pRegister->szName + szInterface.length()) > 0) ||
			!szInterface.compare(pRegister->szName))
			return (T*)pRegister->pCreateFn();
	}

	return nullptr;
}