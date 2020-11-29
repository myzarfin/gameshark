#include "../features.hpp"
#include "../../menu/variables.h"

void misc::Bunnyhop()
{
	if (!C::Get<bool>(Vars.bhop) || m_bIsJumpbugging)
		return;

	if (g::local_player->move_type() == MOVETYPE_LADDER || 
		g::local_player->move_type() == MOVETYPE_NOCLIP || 
		g::local_player->move_type() == MOVETYPE_OBSERVER)
		return;

	if (!(g::local_player->flags() & FL_ONGROUND))
		g::cmd->buttons &= ~IN_JUMP;
}

void misc::InfiniteDuck()
{
	if (!C::Get<bool>(Vars.fast_duck))
		return;

	g::cmd->buttons |= IN_BULLRUSH;
}

void misc::EdgeJump(int iFlags)
{
	static bool bToggled = false;
	if (!C::Get<bool>(Vars.edge_jump) ||
		!GUI::UTILS::KeybindMethod(C::Get<int>(Vars.edge_jump_key), C::Get<int>(Vars.edge_key_method), &bToggled))
		return;

	if (!(g::local_player->flags() & FL_ONGROUND) &&
		(iFlags & FL_ONGROUND))
		g::cmd->buttons |= IN_JUMP;
}

void misc::Jumpbug(int iFlags)
{
	m_bIsJumpbugging = false;

	static bool bToggled = false;
	if (!C::Get<bool>(Vars.jump_bug) ||
		!GUI::UTILS::KeybindMethod(C::Get<int>(Vars.jump_bug_key), C::Get<int>(Vars.jump_key_method), &bToggled))
		return;

	m_bIsJumpbugging = true;
}

void misc::Edgebug(int iFlags)
{
	static bool bToggled = false;
	if (!C::Get<bool>(Vars.edge_bug) ||
		!GUI::UTILS::KeybindMethod(C::Get<int>(Vars.edge_bug_key), C::Get<int>(Vars.edge_bug_method), &bToggled))
		return;
}