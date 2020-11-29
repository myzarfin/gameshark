#include "csgo.hpp"

void entity_cache::fill()
{
	if (!g::local_player)
		return;

	for (int n = 1; n < interfaces::globals->max_clients; n++)
	{
		const auto entity = interfaces::entity_list->get<c_base_entity>(n);
		if (!entity)
			continue;

		groups[group_type_t::PLAYERS_ALL].push_back(entity);
		groups[entity->team() != g::local_player->team() ? group_type_t::PLAYERS_ENEMIES : group_type_t::PLAYERS_TEAMMATES].push_back(entity);
	}
}

void entity_cache::clear()
{
	for (auto& group : groups)
		group.second.clear();
}

const std::vector<c_base_entity*>& entity_cache::get_entity_group(const group_type_t& group)
{
	return groups[group];
}