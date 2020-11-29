#include "player_esp.h"

bool visuals::world_to_screen(const vec3_t& origin, vec3_t& screen) {
	return interfaces::debug_overlay->world_to_screen(origin, screen);
}

void visuals::glow()
{
	for (int i = 0; i < interfaces::glow_manager->vecGlowObjectDefinitions.size; i++)
	{
		auto& hGlowObject = interfaces::glow_manager->vecGlowObjectDefinitions[i];
		if (hGlowObject.IsEmpty())
			continue;

		auto pEntity = hGlowObject.pEntity;
		if (pEntity == nullptr)
			continue;

		auto pClientClass = pEntity->client_class();
		if (pClientClass == nullptr)
			continue;

		const auto nIndex = pClientClass->class_id;
		switch (nIndex)
		{
		case class_ids::ccsplayer:
		{
			if (pEntity->dormant() || !pEntity->is_alive())
				break;

			if (pEntity->team() != g::local_player->team())
				hGlowObject.Set(C::Get<Color>(Vars.glow_col));
			break;
		}
		default:
		{
			hGlowObject.flAlpha = 0.0f;
			break;
		}
		}

		hGlowObject.bRenderWhenOccluded = true;
		hGlowObject.bRenderWhenUnoccluded = false;
	}
}

void visuals::players::run()
{
	if (!g::local_player || !interfaces::engine->is_in_game())
		return;

	for (auto entity : entity_cache::groups[group_type_t::PLAYERS_ENEMIES])
	{
		if (!entity || !entity->is_alive())
			continue;

		const int index = entity->index();

		handle_dormancy(entity, index);
		if (entity_alpha[index] <= 0.0f)
			continue;

		bbox_t bbox;
		if (!bounding_box(entity, bbox))
			continue;

		if (C::Get<bool>(Vars.box))
		{
			render::outlined_rect(bbox.x, bbox.y, bbox.w, bbox.h, Color(C::Get<Color>(Vars.box_col), entity->dormant() ? entity_alpha[index] : C::Get<Color>(Vars.box_col).a));
			render::outlined_rect(bbox.x - 1, bbox.y - 1, bbox.w + 2, bbox.h + 2, Color(0, 0, 0, entity_alpha[index]));
			render::outlined_rect(bbox.x + 1, bbox.y + 1, bbox.w - 2, bbox.h - 2, Color(0, 0, 0, entity_alpha[index]));
		}

		if (C::Get<bool>(Vars.name))
		{
			player_info_t playerInfo;
			interfaces::engine->get_player_info(index, &playerInfo);

			std::string player_name;
			if (playerInfo.fakeplayer)
				player_name += ("BOT ");

			player_name += playerInfo.name;

			render::string(bbox.x + (bbox.w / 2) - (render::get_text_size(render::fonts::menu_bold_font[g::scaling], player_name.c_str()).right / 2), bbox.y - 12, render::fonts::menu_bold_font[g::scaling], player_name.c_str(), false, Color(C::Get<Color>(Vars.name_col), entity->dormant() ? entity_alpha[index] : C::Get<Color>(Vars.name_col).a));
		}

		if (C::Get<bool>(Vars.health))
		{
			auto health = std::clamp(entity->health(), 0, 100);

			auto green = health * 2.55;
			auto red = 255 - green;

			auto bar = (bbox.h / 100.0f) * health;
			auto delta = bbox.h - bar;

			render::rect(bbox.x - 6, bbox.y, 2, bbox.h, Color(80, 80, 80, entity->dormant() ? 125 : entity_alpha[index]));
			render::rect(bbox.x - 6, bbox.y + delta + 1, 2, bar - 2,Color(red, green, 0, entity_alpha[index]));
			render::outlined_rect(bbox.x - 7, bbox.y, 4, bbox.h, Color(0, 0, 0, entity_alpha[index]));

			if (health < 100.0f) 
				render::string(bbox.x - 6, bbox.y + delta, render::fonts::small_fonts[g::scaling], std::to_string(health).c_str(), true, Color(255, 255, 255, entity->dormant() ? 125 : entity_alpha[index]));
		}

		const auto weapon = entity->active_weapon();
		if (weapon)
		{
			const auto data = interfaces::weapon_system->get_weapon_data(weapon->item_definition_index());
			if (data)
			{
				if (C::Get<bool>(Vars.weapon))
				{
					std::string weapon_name = data->weapon_name;
					weapon_name = weapon_name.substr(7);
					std::transform(weapon_name.begin(), weapon_name.end(), weapon_name.begin(), toupper);

					render::string(bbox.x + (bbox.w / 2), C::Get<bool>(Vars.ammo) && !(data->weapon_type == WEAPONTYPE_C4 || data->weapon_type == WEAPONTYPE_GRENADE || data->weapon_type == WEAPONTYPE_KNIFE) ? (bbox.y + bbox.h) + 10 : (bbox.y + bbox.h) + 6, render::fonts::small_fonts[g::scaling], weapon_name, true, Color(C::Get<Color>(Vars.weapon_col), entity->dormant() ? entity_alpha[index] : C::Get<Color>(Vars.weapon_col).a));
				}
				if (C::Get<bool>(Vars.ammo))
				{
					if (data->weapon_type == WEAPONTYPE_C4 ||
						data->weapon_type == WEAPONTYPE_GRENADE ||
						data->weapon_type == WEAPONTYPE_KNIFE) {
						return;
					}

					float bar_width = weapon->clip1_count() * bbox.w / data->weapon_max_clip;

					auto animlayer = entity->get_anim_overlays()[1];
					if (animlayer.pOwner) {
						auto activity = entity->get_sequence_activity(animlayer.nSequence);
						if ((activity == 967 && animlayer.flWeight != 0.0f) &&
							animlayer.flCycle < 0.99) {
							bar_width = (animlayer.flCycle * bbox.w) / 1.f;
						}
					}

					render::rect(bbox.x + 1, bbox.y + bbox.h + 3, bbox.w - 2, 2, Color(80, 80, 80, entity->dormant() ? entity_alpha[index] : 125));
					render::rect(bbox.x + 1, bbox.y + bbox.h + 3, bar_width - 2, 2, Color(C::Get<Color>(Vars.ammo_col), entity->dormant() ? entity_alpha[index] : C::Get<Color>(Vars.ammo_col).a));
					render::outlined_rect(bbox.x + 1, bbox.y + bbox.h + 2, bbox.w - 2, 4, Color(0, 0, 0, entity_alpha[index]));

					if (weapon->clip1_count() < data->weapon_max_clip && entity->get_sequence_activity(animlayer.nSequence) != 967)
						render::string(bbox.x + bar_width, bbox.y + bbox.h + 4, render::fonts::small_fonts[g::scaling], std::to_string(weapon->clip1_count()).c_str(), true, Color(255, 255, 255, entity->dormant() ? entity_alpha[index] : 125));
				}
			}
		}
	}
}

void visuals::players::handle_dormancy(c_base_entity* entity, int index)
{
	if (entity->dormant() && entity_alpha[index] > 0)
	{
		if (!time_set[index])
		{
			time_since_dormany[index] = interfaces::globals->cur_time;
			time_set[index] = true;
		}

		float fade_time = C::Get<float>(Vars.dormancy);
		if (fade_time <= 0.0f)
			fade_time = 0.01f;

		entity_alpha[index] = 175.0f - ((interfaces::globals->cur_time - time_since_dormany[index]) * (175.0f / fade_time));
		if (entity_alpha[index] < 0.0f)
			entity_alpha[index] = 0.0f;
	}

	if (!entity->dormant())
	{
		entity_alpha[index] = 255.0f;
		time_since_dormany[index] = 0.0f;
		time_set[index] = false;
	}
}

bool visuals::players::bounding_box(c_base_entity* entity, bbox_t& bbox)
{
	vec3_t origin, mins, maxs;
	vec3_t bottom, top;

	origin = entity->origin();

	mins = entity->collideable()->mins();
	maxs = entity->collideable()->maxs();

	mins = { origin.x, origin.y, mins.z - 4.0f};
	maxs = { origin.x, origin.y, maxs.z + 4.0f };

	if (!world_to_screen(mins, bottom) || 
		!world_to_screen(maxs, top))
		return false;

	bbox.h = bottom.y - top.y;
	bbox.w = bbox.h / 2.f;
	bbox.x = bottom.x - (bbox.w / 2.f);
	bbox.y = bottom.y - bbox.h;
	return true;
}

i_material* visuals::players::create_material(std::string szName, std::string szShader, std::string szBaseTexture, std::string szEnvMap, bool bIgnorez, bool bWireframe, std::string szProxies)
{
	std::string szMaterial = fmt::format((R"#("{shader}"
	{{
		"$basetexture"	"{texture}"
		"$ignorez"		"{ignorez}"
		"$envmap"		"{envmap}"
		"$wireframe"	"{wireframe}"

		"$model"		"1"
		"$selfillum"	"1"
		"$halflambert"	"1"
		"$nofog"		"1"
		"$nocull"       "0"
		"$znearer"		"0"
		"$flat"			"1" 
		"$normalmapalphaenvmask" "1"

		"$bumpmap"		"effects\flat_normal"

		"$envmaptint"		"[0 0 0]"
		"$envmapfresnel" "0"

		"$phongalbedotint" "1"
		"$phong" "1"
		"$phongexponent" "15"
		"$phongboost" "0"
		"$phongtint"		"[0 0 0]"

		"$rimlight" "1"
		"$rimlightexponent" "15"
		"$rimlightboost" "1"

		"$phongfresnelranges" "[.5 .5 .5]"

		"$pearlescent" "0"

		"proxies"
		{{ 
			{proxies}
		}}
	}})#"), fmt::arg(("shader"), szShader), fmt::arg(("texture"), szBaseTexture), fmt::arg(("envmap"), szEnvMap), fmt::arg(("ignorez"), bIgnorez ? 1 : 0), fmt::arg(("wireframe"), bWireframe ? 1 : 0), fmt::arg(("proxies"), szProxies));

	CKeyValues* pKeyValues = (CKeyValues*)CKeyValues::operator new(sizeof(CKeyValues));
	pKeyValues->Init(szShader.data());
	pKeyValues->LoadFromBuffer(szName.data(), szMaterial.c_str());

	return interfaces::material_system->CreateMaterial(szName.data(), pKeyValues);
}

bool visuals::players::chams(DrawModelResults_t* pResults, const DrawModelInfo_t& info, matrix3x4_t* pBoneToWorld, float* flFlexWeights, float* flFlexDelayedWeights, const vec3_t& vecModelOrigin, int nFlags)
{
	static auto oDrawModel = detour::draw_model.GetOriginal<decltype(&hooks::hkDrawModel)>();
	auto* pRenderable = (i_client_renderable*)info.pClientEntity;
	if (pRenderable == nullptr)
		return false;

	c_base_entity* pEntity = (c_base_entity*)pRenderable->GetIClientUnknown()->GetBaseEntity();
	if (pEntity == nullptr)
		return false;

	static i_material* arrMaterials[1] =
	{
		create_material(("custom"), ("VertexLitGeneric"), ("vgui/white"), ("env_cubemap")),
	};

	std::string_view szModelName = info.pStudioHdr->name_char_array;
	if (szModelName.find(("player")) != std::string_view::npos && szModelName.find(("shadow")) == std::string_view::npos)
	{
		if (nFlags & (STUDIO_RENDER | STUDIO_SKIP_FLEXES | STUDIO_DONOTMODIFYSTENCILSTATE | STUDIO_NOLIGHTING_OR_CUBEMAP | STUDIO_SKIP_DECALS))
			return false;

		if (!pEntity->is_alive() || pEntity->dormant())
			return false;

		if (g::local_player->team() != pEntity->team())
		{
			i_material* pMaterial = arrMaterials[0];

			if (pMaterial == nullptr || pMaterial->is_error_material())
				return false;

			Color colVisible = C::Get<Color>(Vars.chams_col);
			Color colHidden = C::Get<Color>(Vars.chams_xqz_col);

			if (C::Get<bool>(Vars.chams_xqz))
			{
				pMaterial->increment_reference_count();
				pMaterial->set_material_var_flag(material_var_ignorez, true);

				interfaces::studio_render->set_color_modulation(colHidden.Base().data());
				interfaces::studio_render->set_alpha_modulation(colHidden.aBase());
				interfaces::studio_render->forced_material_override(pMaterial);

				oDrawModel(interfaces::studio_render, 0, pResults, info, pBoneToWorld, flFlexWeights, flFlexDelayedWeights, vecModelOrigin, nFlags);
			}

			if (C::Get<bool>(Vars.chams))
			{
				pMaterial->increment_reference_count();
				pMaterial->set_material_var_flag(material_var_ignorez, false);

				interfaces::studio_render->set_color_modulation(colVisible.Base().data());
				interfaces::studio_render->set_alpha_modulation(colVisible.aBase());
				interfaces::studio_render->forced_material_override(pMaterial);
			}

			const float flPearlescent = C::Get<float>(Vars.pearlescent);
			i_material_var* pPearl = pMaterial->find_var(("$pearlescent"), nullptr);
			pPearl->SetVector(flPearlescent / 10.0f, flPearlescent / 10.0f, flPearlescent / 10.0f);

			if (C::Get<bool>(Vars.phong))
			{
				const auto phong_col = C::Get<Color>(Vars.phong_col);

				i_material_var* phongboost = pMaterial->find_var(("$phongboost"), nullptr);
				phongboost->SetFloat((phong_col.a / 255.0f) * 10.0f);

				i_material_var* phong_tint = pMaterial->find_var(("$phongtint"), nullptr);
				phong_tint->SetVector(phong_col.r / 255.0f, phong_col.g / 255.0f, phong_col.b / 255.0f);
			}
			else {
				i_material_var* phongboost = pMaterial->find_var(("$phongboost"), nullptr);
				phongboost->SetFloat(0.0f);
			}

			if (C::Get<bool>(Vars.reflectivity))
			{
				const auto reflectivity_col = C::Get<Color>(Vars.reflectivity_col);
				i_material_var* reflectivity = pMaterial->find_var(("$envmaptint"), nullptr);
				reflectivity->SetVector((reflectivity_col.r / 255.0f), (reflectivity_col.g / 255.0f), (reflectivity_col.b / 255.0f));
			}
			else {
				i_material_var* reflectivity = pMaterial->find_var(("$envmaptint"), nullptr);
				reflectivity->SetVector(0, 0, 0);
			}

			return true;
		}
	}
	return false;
}

bool visuals::players::chams()
{
	return false;
}