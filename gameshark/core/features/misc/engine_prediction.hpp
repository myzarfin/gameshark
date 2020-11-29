#pragma once
#include "../../../dependencies/utilities/csgo.hpp"

namespace prediction {
	void Start();
	void End();

	inline player_move_data data;
	inline float old_cur_time;
	inline float old_frame_time;
	inline int* prediction_random_seed;
};
