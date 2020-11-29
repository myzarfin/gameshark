#include "../gui.h"

void GUI::CONTROLS::BeginGroupbox(const std::string& szName, const int iHeight, const int iColumn)
{
	if (!m_bInitialized)
		return;

	m_iColumn = iColumn;

	m_vOldGroupboxPos.x = m_iColumn ? 218 : 6;

	m_vOffset.x = m_vPos.x + 15 + m_vOldGroupboxPos.x;
	m_vOffset.y = m_vPos.y + m_vOldGroupboxPos.y + 8;

	m_szGroupboxName = szName;

	auto flOffsetFromLeft = m_vPos.x + 6;
	auto flOffsetFromTop = m_vPos.y + m_vOldGroupboxPos.y - 3;

	render::rect(flOffsetFromLeft + m_vOldGroupboxPos.x - 1,
		flOffsetFromTop - 1,
		206, iHeight, Color(25, 25, 25, CONTROLS::m_flMenuAlpha));

	render::outlined_rect(flOffsetFromLeft + m_vOldGroupboxPos.x - 1,
		flOffsetFromTop - 1,
		206,
		iHeight + 2,
		Color(0, 0, 0, CONTROLS::m_flMenuAlpha));

	render::outlined_rect(flOffsetFromLeft + m_vOldGroupboxPos.x,
		flOffsetFromTop,
		204,
		iHeight,
		Color(50, 50, 50, CONTROLS::m_flMenuAlpha));

	render::rect(flOffsetFromLeft + m_vOldGroupboxPos.x + 8,
		flOffsetFromTop - 2,
		render::get_text_size(render::fonts::menu_font[g::scaling], m_szGroupboxName.c_str()).right + 10,
		2,
		Color(23, 23, 23, CONTROLS::m_flMenuAlpha));

	render::rect(flOffsetFromLeft + m_vOldGroupboxPos.x + 8,
		flOffsetFromTop,
		render::get_text_size(render::fonts::menu_font[g::scaling], m_szGroupboxName.c_str()).right + 10,
		1,
		Color(28, 28, 28, CONTROLS::m_flMenuAlpha));

	render::string(flOffsetFromLeft + m_vOldGroupboxPos.x + 12,
		flOffsetFromTop - 6,
		render::fonts::menu_font[g::scaling], m_szGroupboxName.c_str(),
		false,
		Color(205, 205, 205, CONTROLS::m_flMenuAlpha));
}
void GUI::CONTROLS::EndGroupbox()
{
	if (!m_bInitialized)
		return;

	m_vOldGroupboxPos.y += m_vOffset.y - ((m_vPos.y + m_vOldGroupboxPos.y - 3) + 16) + 45;
}