#include "../gui.h"

void GUI::CONTROLS::Text(const std::string& szName, const bool bUseCustomCol)
{
	if (!m_bInitialized)
		return;

	render::string(m_vOffset.x + 21, m_vOffset.y, render::fonts::menu_font[g::scaling], szName, false, Color(210, 210, 210, CONTROLS::m_flMenuAlpha));

	m_vOffset.y += 14;

	m_iLastControl = TEXT;
}