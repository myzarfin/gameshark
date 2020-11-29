#include "visuals.h"

void visuals::modulate_world()
{
	if (!g::local_player || !interfaces::engine->is_in_game())
		return;

	static auto pDrawSpecificStaticProps = interfaces::console->get_convar(("r_drawspecificstaticprop"));
	std::vector< i_material* > vecWorld, vecProps;

	for (
		uint16_t h = interfaces::material_system->first_material();
		h != interfaces::material_system->invalid_material_handle();
		h = interfaces::material_system->next_material(h))
	{
		i_material* pMaterial = interfaces::material_system->get_material(h);
		if (!pMaterial || pMaterial->is_error_material())
			continue;

		switch (FNV1A::HashConst(pMaterial->get_texture_group_name()))
		{
		case FNV1A::HashConst(("World textures")):
			vecWorld.push_back(pMaterial);
			break;

		case FNV1A::HashConst(("StaticProp textures")):
			vecProps.push_back(pMaterial);
			break;
		}
	}

	if (C::Get<bool>(Vars.nightmode)) {
		const float flWorld = 1.f - (C::Get<float>(Vars.nightmode_brightness) / 100.f);
		const float flProps = 1.3f - (C::Get<float>(Vars.nightmode_brightness) / 100.f);

		for (const auto& hWorld : vecWorld)
			hWorld->color_modulate(flWorld, flWorld, flWorld);

		pDrawSpecificStaticProps->set_value(0);

		for (const auto& hProps : vecProps)
			hProps->color_modulate(flProps, flProps, flProps);
	}
	else {
		for (const auto& hWorld : vecWorld)
			hWorld->color_modulate(1.f, 1.f, 1.f);

		pDrawSpecificStaticProps->set_value(-1);

		for (const auto& hProps : vecProps)
			hProps->color_modulate(1.f, 1.f, 1.f);
	}
}

CFlashLightEffect* visuals::create_flashlight(int nEntIndex, const char* pszTextureName, float flFov, float flFarZ, float flLinearAtten)
{
	static DWORD oConstructor = (DWORD)utilities::pattern_scan("client.dll", "55 8B EC F3 0F 10 45 ? B8");

	CFlashLightEffect* pFlashLight = (CFlashLightEffect*)interfaces::mem_alloc->Alloc(sizeof(CFlashLightEffect));
	if (!pFlashLight)
		return NULL;

	__asm
	{
		movss xmm3, flFov
		mov ecx, pFlashLight
		push flLinearAtten
		push flFarZ
		push pszTextureName
		push nEntIndex
		call oConstructor
	}

	return pFlashLight;
}

void visuals::destroy_flashlight(CFlashLightEffect* pFlashlight)
{
	static DWORD oDestructor = (DWORD)utilities::pattern_scan("client.dll", "56 8B F1 E8 ? ? ? ? 8B 4E 28");
	__asm
	{
		mov ecx, pFlashlight
		push ecx
		call oDestructor
	}
}

void visuals::update_flashlight(CFlashLightEffect* pFlashLight, const vec3_t& vecPos, const vec3_t& vecForward, const vec3_t& vecRight, const vec3_t& vecUp)
{
	typedef void(__thiscall* UpdateLight_t)(void*, int, const vec3_t&, const vec3_t&, const vec3_t&, const vec3_t&, float, float, float, bool, const char*);

	static UpdateLight_t oUpdateLight = NULL;
	if (!oUpdateLight)
	{
		DWORD callInstruction = (DWORD)utilities::pattern_scan("client.dll", "E8 ? ? ? ? 8B 06 F3 0F 10 46"); // get the instruction address
		DWORD relativeAddress = *(DWORD*)(callInstruction + 1); // read the rel32
		DWORD nextInstruction = callInstruction + 5; // get the address of next instruction
		oUpdateLight = (UpdateLight_t)(nextInstruction + relativeAddress); // our function address will be nextInstruction + relativeAddress
	}

	oUpdateLight(pFlashLight, pFlashLight->m_nEntIndex, vecPos, vecForward, vecRight, vecUp, pFlashLight->m_flFov, pFlashLight->m_flFarZ, pFlashLight->m_flLinearAtten, pFlashLight->m_bCastsShadows, pFlashLight->m_szTextureName);
}

void visuals::run_flashlight()
{
	static CFlashLightEffect* pFlashLight = NULL;
	if (!C::Get<bool>(Vars.flashlight) ||
		!interfaces::engine->is_in_game() ||
		!g::local_player)
	{
		if (pFlashLight)
		{
			destroy_flashlight(pFlashLight);
			pFlashLight = NULL;
		}
		return;
	}

	static bool bIsOn = false;
	static bool bToggled = false;
	bIsOn = GUI::UTILS::KeybindMethod(C::Get<int>(Vars.flashlight_key), C::Get<int>(Vars.flashlight_key_method), &bToggled);

	if (bIsOn)
	{
		if (!pFlashLight)
			pFlashLight = create_flashlight(g::local_player->index(), ("effects/flashlight001"), C::Get<float>(Vars.flashlight_fov), 1000, 1000);
	}
	else
	{
		if (pFlashLight)
		{
			destroy_flashlight(pFlashLight);
			pFlashLight = NULL;
		}
		return;
	}

	if (pFlashLight)
	{
		vec3_t f, r, u, viewangles;

		interfaces::engine->get_view_angles(viewangles);
		math::angle_vectors(viewangles, &f, &r, &u);

		pFlashLight->m_bIsOn = true;
		pFlashLight->m_bCastsShadows = false;
		pFlashLight->m_flFov = C::Get<float>(Vars.flashlight_fov);
		update_flashlight(pFlashLight, g::local_player->get_eye_pos(), f, r, u);
	}
}