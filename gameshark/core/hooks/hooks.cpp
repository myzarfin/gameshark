#include "../../dependencies/utilities/csgo.hpp"
#include "../features/features.hpp"
#include "../features/misc/engine_prediction.hpp"
#include "../features/visuals/player_esp.h"
#include "../features/visuals/visuals.h"

bool hooks::initialize() {
	if (MH_Initialize() != MH_OK)
		throw std::runtime_error("failed to initialize MH_Initialize.");

	if (!detour::create_move.Create(GetVFunc(interfaces::clientmode, 24), &hkCreateMove))
		throw std::runtime_error("failed to initialize hkCreateMove");

	if (!detour::paint_traverse.Create(GetVFunc(interfaces::panel, 41), &hkPaintTraverse))
		throw std::runtime_error("failed to initialize hkPaintTraverse");

	if (!detour::frame_stage_notify.Create(GetVFunc(interfaces::client, 37), &hkFrameStageNotify))
		throw std::runtime_error("failed to initialize hkFrameStageNotify");

	if (!detour::lock_cursor.Create(GetVFunc(interfaces::surface, 67), &hkLockCursor))
		throw std::runtime_error("failed to initialize hkLockCursor");

	if (!detour::post_entity.Create(GetVFunc(interfaces::client, 6), &hkLevelInitPostEntity))
		throw std::runtime_error("failed to initialize hkLevelInitPostEntity");

	if (!detour::shutdown.Create(GetVFunc(interfaces::client, 7), &hkLevelShutdown))
		throw std::runtime_error("failed to initialize hkLevelShutdown");

	if (!detour::post_screen_effects.Create(GetVFunc(interfaces::clientmode, 44), &hkDoPostScreenEffects))
		throw std::runtime_error("failed to initialize hkDoPostScreenEffects");

	if (!detour::scene_end.Create(GetVFunc(interfaces::render_view, 9), &hkSceneEnd))
		throw std::runtime_error("failed to initialize hkSceneEnd");

	if (!detour::draw_model.Create(GetVFunc(interfaces::studio_render, 29), &hkDrawModel))
		throw std::runtime_error("failed to initialize hkDrawModel");

	if (!detour::override_config.Create(GetVFunc(interfaces::material_system, 21), &hkOverrideConfig))
		throw std::runtime_error("failed to initialize hkOverrideConfig");

	console::log("[setup] hooks initialized!\n");
	return true;
}

bool __fastcall hooks::hkCreateMove(void* thisptr, int edx, float flInputSampleTime, c_usercmd* pCmd) {
	static auto oCreateMove = detour::create_move.GetOriginal<decltype(&hkCreateMove)>();
	oCreateMove(thisptr, edx, flInputSampleTime, pCmd);

	if (!pCmd || !pCmd->command_number)
		return false;

	g::local_player = interfaces::entity_list->get<c_base_entity>(interfaces::engine->get_local_player());
	if (g::local_player)
	{
		g::local_weapon = g::local_player->active_weapon();
		if (g::local_weapon)
			g::local_weapon_data = interfaces::weapon_system->get_weapon_data(g::local_weapon->item_definition_index());
	}

	g::cmd = pCmd;

	if (GUI::m_bOpened)
	{
		if (pCmd->buttons & IN_ATTACK)
			pCmd->buttons &= ~IN_ATTACK;

		if (pCmd->buttons & IN_ATTACK2)
			pCmd->buttons &= ~IN_ATTACK2;
	}

	uintptr_t* frame_pointer;
	__asm mov frame_pointer, ebp;
	bool& send_packet = *reinterpret_cast<bool*>(*frame_pointer - 0x1C);

	const auto vecViewangles = pCmd->viewangles;
	const auto flForwardmove = pCmd->forwardmove;
	const auto flSidemove = pCmd->sidemove;

	const int iFlags = g::local_player->flags();

	misc::Bunnyhop();
	misc::InfiniteDuck();

	prediction::Start(); {

	} prediction::End();

	misc::EdgeJump(iFlags);
	misc::Jumpbug(iFlags);
	misc::Edgebug(iFlags);

	math::correct_movement(vecViewangles, pCmd, flForwardmove, flSidemove);

	return false;
}

void __fastcall hooks::hkPaintTraverse(void* thisptr, int edx, unsigned int uPanel, bool bForceRepaint, bool bForce){
	static auto oPaintTraverse = detour::paint_traverse.GetOriginal<decltype(&hkPaintTraverse)>();
	oPaintTraverse(thisptr, edx, uPanel, bForceRepaint, bForce);

	int iCurrentWidth, iCurrentHeight = 0;
	interfaces::engine->get_screen_size(iCurrentWidth, iCurrentHeight);
	if (g::screen_size.x != iCurrentWidth || g::screen_size.y != iCurrentHeight)
		g::screen_size.x = iCurrentWidth, g::screen_size.y = iCurrentHeight;

	switch (FNV1A::HashConst(interfaces::panel->get_panel_name(uPanel))) {
		case FNV1A::HashConst("MatSystemTopPanel"):
		{
			visuals::players::run();

			GUI::UTILS::InitializeInput("Counter-Strike: Global Offensive");
			GUI::Run();

			visuals::run_flashlight();
		} break;
		default:
			break;
	}
}

void __fastcall hooks::hkFrameStageNotify(void* thisptr, int edx, int stage) {
	static auto oFrameStageNotify = detour::frame_stage_notify.GetOriginal<decltype(&hkFrameStageNotify)>();

	if (!interfaces::engine->is_in_game())
		oFrameStageNotify(thisptr, edx, stage);

	switch (stage)
	{
		case FRAME_START:
			break;
		case FRAME_NET_UPDATE_START:
			entity_cache::clear();
			break;
		case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
			break;
		case FRAME_NET_UPDATE_POSTDATAUPDATE_END:
			break;
		case FRAME_NET_UPDATE_END:
			entity_cache::fill();
			break;
		case FRAME_RENDER_START:
			break;
		case FRAME_RENDER_END:
			break;
		default:
			break;
	}

	oFrameStageNotify(thisptr, edx, stage);
}

void __fastcall hooks::hkLockCursor(void* thisptr, int edx)
{
	static auto oLockCursor = detour::lock_cursor.GetOriginal<decltype(&hkLockCursor)>();

	if (g::local_player && g::local_player->is_alive() && interfaces::engine->is_in_game())
		interfaces::inputsystem->enable_input(true);

	GUI::m_bOpened ? interfaces::surface->unlock_cursor() : oLockCursor(thisptr, edx);
}

void __fastcall hooks::hkLevelInitPostEntity(void* pThis, void* edx)
{
	static auto oLevelInitPostEntity = detour::post_entity.GetOriginal<decltype(&hkLevelInitPostEntity)>();
	
	if (C::Get<bool>(Vars.nightmode))
		visuals::modulate_world();
	
	oLevelInitPostEntity(pThis, edx);
}

void __fastcall hooks::hkLevelShutdown(void* pThis, void* edx)
{
	static auto oLevelShutdown = detour::shutdown.GetOriginal<decltype(&hkLevelShutdown)>();
	
	entity_cache::clear();
	
	oLevelShutdown(pThis, edx);
}

int	__fastcall	hooks::hkDoPostScreenEffects(void* thisptr, int edx, void* pSetup)
{
	static auto oDoPostScreenEffects = detour::post_screen_effects.GetOriginal<decltype(&hkDoPostScreenEffects)>();

	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
		return oDoPostScreenEffects(thisptr, edx, pSetup);

	if (g::local_player && interfaces::glow_manager && C::Get<bool>(Vars.glow))
		visuals::glow();

	return oDoPostScreenEffects(thisptr, edx, pSetup);
}

void __fastcall hooks::hkDrawModel(void* thisptr, int edx, DrawModelResults_t* pResults, const DrawModelInfo_t& info, matrix3x4_t* pBoneToWorld, float* flFlexWeights, float* flFlexDelayedWeights, const vec3_t& vecModelOrigin, int nFlags)
{
	static auto oDrawModel = detour::draw_model.GetOriginal<decltype(&hkDrawModel)>();
	bool bClearOverride = false;

	if (g::local_player)
		bClearOverride = visuals::players::chams(pResults, info, pBoneToWorld, flFlexWeights, flFlexDelayedWeights, vecModelOrigin, nFlags);

	oDrawModel(thisptr, edx, pResults, info, pBoneToWorld, flFlexWeights, flFlexDelayedWeights, vecModelOrigin, nFlags);

	if (bClearOverride)
		interfaces::studio_render->forced_material_override(nullptr);
}

void __fastcall hooks::hkSceneEnd(void* thisptr)
{
	static auto oSceneEnd = detour::scene_end.GetOriginal<decltype(&hkSceneEnd)>();
	oSceneEnd(thisptr);

	/*static i_material* vecMaterials[2] =
	{
		interfaces::material_system->find_material("debug/debugambientcube", "Model Textures"),
		interfaces::material_system->find_material("debug/debugdrawflat", "Model Textures")
	};

	i_material* material = vecMaterials[C::Get<bool>(Vars.flat_chams)];

	for (auto entity : entity_cache::groups[group_type_t::PLAYERS_ENEMIES])
	{
		if (!entity || entity->dormant() || !entity->is_alive())
			continue;

		if (material->is_error_material() || !material)
			continue;

		if (C::Get<bool>(Vars.chams_xqz))
		{
			material->increment_reference_count();
			material->set_material_var_flag(material_var_ignorez, true);
			material->color_modulate(C::Get<Color>(Vars.chams_xqz_col));
			material->alpha_modulate(C::Get<Color>(Vars.chams_xqz_col));
			interfaces::model_render->override_material(material);
			entity->draw_model(0x1, 255);
			interfaces::model_render->override_material(nullptr);
		}

		if (C::Get<bool>(Vars.chams))
		{
			material->increment_reference_count();
			material->set_material_var_flag(material_var_ignorez, false);
			material->color_modulate(C::Get<Color>(Vars.chams_col));
			material->alpha_modulate(C::Get<Color>(Vars.chams_col));
			interfaces::model_render->override_material(material);
			entity->draw_model(0x1, 255);
			interfaces::model_render->override_material(nullptr);
		}
	}*/
}

bool __fastcall hooks::hkOverrideConfig(void* pThis, void* edx, MaterialSystemConfig_t* pConfig, bool bUpdate)
{
	static auto oOverrideConfig = detour::override_config.GetOriginal<decltype(&hkOverrideConfig)>();

	if (C::Get<bool>(Vars.full_bright))
		pConfig->uFullbright = true;

	static auto bUpdateNight = false, bToggle = false;
	static auto flBrightness = 100.0f;

	if (bToggle != C::Get<bool>(Vars.nightmode) ||
		flBrightness != C::Get<float>(Vars.nightmode_brightness))
	{
		bToggle = C::Get<bool>(Vars.nightmode);
		flBrightness = C::Get<float>(Vars.nightmode_brightness);
		bUpdateNight = true;
	}

	if (bUpdateNight)
	{
		bUpdateNight = false;
		visuals::modulate_world();
	}

	return oOverrideConfig(pThis, edx, pConfig, bUpdate);
}