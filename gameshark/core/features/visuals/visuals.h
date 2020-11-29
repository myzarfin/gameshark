#pragma once

#include "../../../dependencies/utilities/csgo.hpp"
#include "../../menu/variables.h"

namespace visuals
{
	void modulate_world();

	CFlashLightEffect* create_flashlight(int, const char*, float, float, float);
	void destroy_flashlight(CFlashLightEffect*);
	void update_flashlight(CFlashLightEffect*, const vec3_t&, const vec3_t&, const vec3_t&, const vec3_t&);
	void run_flashlight();
}