#pragma once
#include "config\config.h"

struct Variables_t
{

	#pragma region player visuals
		C_ADDVARIABLE(bool, box, false);
		C_ADDVARIABLE(bool, name, false);
		C_ADDVARIABLE(bool, weapon, false);
		C_ADDVARIABLE(bool, ammo, false);
		C_ADDVARIABLE(bool, health, false);
		C_ADDVARIABLE(bool, glow, false);
		C_ADDVARIABLE(float, dormancy, 5.0f);
		C_ADDVARIABLE(bool, chams, false);
		C_ADDVARIABLE(bool, chams_xqz, false);
		C_ADDVARIABLE(bool, flat_chams, false);


		C_ADDVARIABLE(bool, phong, false);
		C_ADDVARIABLE(Color, phong_col, Color(255, 255, 255, 255));
		C_ADDVARIABLE(float, pearlescent, 0);
		C_ADDVARIABLE(bool, reflectivity, false);
		C_ADDVARIABLE(Color, reflectivity_col, Color(255, 255, 255, 255));

		C_ADDVARIABLE(Color, box_col, Color(255, 255, 255, 128));
		C_ADDVARIABLE(Color, name_col, Color(255, 255, 255, 200));
		C_ADDVARIABLE(Color, weapon_col, Color(255, 255, 255, 200));
		C_ADDVARIABLE(Color, ammo_col, Color(80, 140, 200, 230));
		C_ADDVARIABLE(Color, glow_col, Color(180, 60, 120, 153));
		C_ADDVARIABLE(Color, chams_col, Color(150, 200, 60, 255));
		C_ADDVARIABLE(Color, chams_xqz_col, Color(60, 120, 180, 179));
	#pragma endregion player visuals

	#pragma region visuals
		C_ADDVARIABLE(bool, nightmode, false);
		C_ADDVARIABLE(bool, full_bright, false);
		C_ADDVARIABLE(float, nightmode_brightness, 100.0f);

		C_ADDVARIABLE(int, flashlight_key, 0);
		C_ADDVARIABLE(int, flashlight_key_method, 1);
		C_ADDVARIABLE(bool, flashlight, false);
		C_ADDVARIABLE(float, flashlight_fov, 35);
	#pragma endregion visuals

	#pragma region misc/movement
		C_ADDVARIABLE(bool, bhop, false);
		C_ADDVARIABLE(bool, fast_duck, false);
		C_ADDVARIABLE(int, edge_jump_key, 0);
		C_ADDVARIABLE(int, edge_key_method, 1);
		C_ADDVARIABLE(bool, edge_jump, false);
		C_ADDVARIABLE(int, jump_bug_key, 0);
		C_ADDVARIABLE(int, jump_key_method, 1);
		C_ADDVARIABLE(bool, jump_bug, 0);
		C_ADDVARIABLE(int, edge_bug_key, 0);
		C_ADDVARIABLE(int, edge_bug_method, 1);
		C_ADDVARIABLE(bool, edge_bug, 0);
	#pragma endregion misc/movement

	#pragma region menu pos
		C_ADDVARIABLE(int, menu_x, 400);
		C_ADDVARIABLE(int, menu_y, 200);
	#pragma endregion menu pos
};

inline Variables_t Vars;
