#include "../gui.h"

void GUI::CONTROLS::Button(const std::string& szName, const std::function<void(bool&)>& func)
{
	static bool bHolder = false;

	const std::vector<std::string> vSplit = UTILS::SplitStr(szName.c_str(), '#');
	const auto bInBound = UTILS::MouseInRegion(m_vOffset.x + 21, m_vOffset.y, 146, 15);
	const auto bActive = m_uBlocking == UTILS::Hash(szName.data());

	if (!m_bDisableBound) {
		if (!bActive && bInBound && UTILS::KeyPressed(VK_LBUTTON))
			m_uBlocking = UTILS::Hash(szName.data());
		else if (bActive && !UTILS::KeyDown(VK_LBUTTON)) {
			m_uBlocking = 0;
			func(bHolder);
		}
	}

	render::rect_fade(m_vOffset.x + 21, m_vOffset.y, 146, 20, Color(30, 30, 30, CONTROLS::m_flMenuAlpha), bInBound ? UTILS::KeyDown(VK_LBUTTON) ? Color(25, 25, 25, CONTROLS::m_flMenuAlpha) : Color(55, 55, 55, CONTROLS::m_flMenuAlpha) : Color(37, 37, 37, CONTROLS::m_flMenuAlpha));
	render::outlined_rect(m_vOffset.x + 21, m_vOffset.y, 146, 20, Color(0, 0, 0, CONTROLS::m_flMenuAlpha));
	render::outlined_rect(m_vOffset.x + 22, m_vOffset.y + 1, 144, 18, Color(50, 50, 50, CONTROLS::m_flMenuAlpha));
	render::string(m_vOffset.x + 21 + (155 / 2), m_vOffset.y + 3, render::fonts::menu_font[g::scaling], szName, 4, Color(165, 165, 165, CONTROLS::m_flMenuAlpha));

	m_vOffset.y += 24;

	m_iLastControl = BUTTON;
}