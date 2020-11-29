#pragma once

#include "../../../dependencies/utilities/csgo.hpp"
#include "../../menu/variables.h"

struct bbox_t
{
	float x = 0.0f;
	float y = 0.0f;
	float w = 0.0f;
	float h = 0.0f;
};

namespace visuals
{
	bool world_to_screen(const vec3_t&, vec3_t&);

	namespace players
	{
		bool bounding_box(c_base_entity*, bbox_t&);
		void handle_dormancy(c_base_entity*, int);
		void run();

		i_material* create_material(std::string, std::string, std::string = ("vgui/white"), std::string = "", bool = false, bool = false, std::string = "");
		bool chams(DrawModelResults_t*, const DrawModelInfo_t&, matrix3x4_t*, float*, float*, const vec3_t&, int);
		bool chams();

		inline float entity_alpha[4096];
		inline float time_since_dormany[4096];
		inline bool  time_set[4096];
	}

	void glow();
}