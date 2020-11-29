#include "../gui.h"
#include <algorithm>
#include "../variables.h"

#include "../../../resource.h"

void GUI::CONTROLS::BeginWindow(const vec2_t vPos, const vec2_t vSize)
{
	static auto iDPIScaling = 0;
	if (iDPIScaling != g::scaling)
	{
		iDPIScaling = g::scaling;
		m_bInitialized = !m_bInitialized;
	}

	if (!m_bInitialized)
	{
		m_vSize = vSize;
		m_vPos = vPos;
		m_vTabs = { ("aimbot"), "players", ("visuals"), ("misc"), ("skins") };

		backgroundTexture = CTextureHolder(CTextureHolder::DecodeResourceImage(IDB_PNG2, 428, 487), 428, 487);
		colorPicker = CTextureHolder(CTextureHolder::DecodeResourceImage(IDB_PNG1, 150, 150), 150, 150);
		transparentBackground = CTextureHolder(CTextureHolder::DecodeResourceImage(IDB_PNG3, 12, 150), 12, 150);

		m_bInitialized = !m_bInitialized;
	}

	auto vecPos = m_vPos = vec2_t((float)C::Get<int>(Vars.menu_x), (float)C::Get<int>(Vars.menu_y));

	static bool bDrag = false;
	static vec2_t vecLastMousePos;
	static vec2_t vecLastMenuPos;

	if (UTILS::KeyDown(VK_LBUTTON))
	{
		if (!bDrag && UTILS::MouseInRegion(m_vPos.x, m_vPos.y, m_vSize.x, m_vSize.y) &&
			!UTILS::MouseInRegion(m_vPos.x, m_vPos.y + render::scale_dpi(25), m_vSize.x, m_vSize.y - render::scale_dpi(25)))
		{
			bDrag = true;
			vecLastMousePos = UTILS::m_vMousePos;
			vecLastMenuPos = m_vPos;
		}

		if (bDrag)
		{
			m_bDisableBound = true;
			m_bCloseBoxes = true;

			C::Get<int>(Vars.menu_x) = vecLastMenuPos.x + (UTILS::m_vMousePos.x - vecLastMousePos.x);
			C::Get<int>(Vars.menu_y) = vecLastMenuPos.y + (UTILS::m_vMousePos.y - vecLastMousePos.y);
		}
	}
	else
		bDrag = false;

	m_bGrabbingCheatGUI = bDrag;

	if (C::Get<int>(Vars.menu_x) < 0)
		C::Get<int>(Vars.menu_x) = 0;

	if (C::Get<int>(Vars.menu_y) < 0)
		C::Get<int>(Vars.menu_y) = 0;

	if ((C::Get<int>(Vars.menu_x) + m_vSize.x) > g::screen_size.x)
		C::Get<int>(Vars.menu_x) = g::screen_size.x - m_vSize.x;

	if ((C::Get<int>(Vars.menu_y) + m_vSize.y) > g::screen_size.y)
		C::Get<int>(Vars.menu_y) = g::screen_size.y - m_vSize.y;

	render::rect(m_vPos.x + render::scale_dpi(2), m_vPos.y + render::scale_dpi(2), m_vSize.x - render::scale_dpi(4), m_vSize.y - render::scale_dpi(4), Color(40, 40, 40, CONTROLS::m_flMenuAlpha));
	backgroundTexture.Draw(m_vPos.x + render::scale_dpi(6), m_vPos.y + render::scale_dpi(27), Color(255, 255, 255, CONTROLS::m_flMenuAlpha), (1 + (0.25 * g::scaling)));
	render::outlined_rect(m_vPos.x, m_vPos.y, m_vSize.x, m_vSize.y, Color(0, 0, 0, CONTROLS::m_flMenuAlpha));
	render::outlined_rect(m_vPos.x + render::scale_dpi(1), m_vPos.y + render::scale_dpi(1), m_vSize.x - render::scale_dpi(2), m_vSize.y - render::scale_dpi(2), Color(60, 60, 60, CONTROLS::m_flMenuAlpha));
	render::outlined_rect(m_vPos.x + render::scale_dpi(5), m_vPos.y + render::scale_dpi(25), m_vSize.x - render::scale_dpi(10), m_vSize.y - render::scale_dpi(30), Color(60, 60, 60, CONTROLS::m_flMenuAlpha));
	render::outlined_rect(m_vPos.x + render::scale_dpi(6), m_vPos.y + render::scale_dpi(26), m_vSize.x - render::scale_dpi(12), m_vSize.y - render::scale_dpi(32), Color(0, 0, 0, CONTROLS::m_flMenuAlpha));

	render::string(m_vPos.x + render::scale_dpi(6), m_vPos.y + render::scale_dpi(12), render::fonts::menu_font[g::scaling], "game", 3, Color(208, 208, 208, CONTROLS::m_flMenuAlpha));
	render::string(m_vPos.x + render::scale_dpi(6) + render::get_text_size(render::fonts::menu_font[g::scaling], "game").right, m_vPos.y + render::scale_dpi(12), render::fonts::menu_font[g::scaling], "shark", 3, Color(CONTROLS::m_cDefaultMenuCol, CONTROLS::m_flMenuAlpha));

	if (!m_vTabs.empty())
	{
		static int iCurrentTab = m_iSelectedTab;
		if (m_iSelectedTab != iCurrentTab)
		{
			m_bCloseBoxes = true;
			iCurrentTab = m_iSelectedTab;
		}

		render::rect(m_vPos.x + render::scale_dpi(7), m_vPos.y + render::scale_dpi(27), render::scale_dpi(11) + (m_iSelectedTab * render::scale_dpi(80)), render::scale_dpi(19), Color(30, 30, 30, CONTROLS::m_flMenuAlpha));
		render::line(m_vPos.x + render::scale_dpi(6), m_vPos.y + render::scale_dpi(46), m_vPos.x + render::scale_dpi(6) + (m_iSelectedTab * render::scale_dpi(80)) + render::scale_dpi(12), m_vPos.y + render::scale_dpi(46), Color(0, 0, 0, CONTROLS::m_flMenuAlpha));
		render::line(m_vPos.x + render::scale_dpi(5), m_vPos.y + render::scale_dpi(47), m_vPos.x + render::scale_dpi(5) + (m_iSelectedTab * render::scale_dpi(80)) + render::scale_dpi(14), m_vPos.y + render::scale_dpi(47), Color(60, 60, 60, CONTROLS::m_flMenuAlpha));

		render::rect(m_vPos.x + render::scale_dpi(6) + (m_iSelectedTab * render::scale_dpi(80)) + render::scale_dpi(93), m_vPos.y + render::scale_dpi(27), m_vSize.y - (m_iSelectedTab * render::scale_dpi(80)) - render::scale_dpi(187), render::scale_dpi(19), Color(30, 30, 30, CONTROLS::m_flMenuAlpha));
		render::line(m_vPos.x + render::scale_dpi(6) + (m_iSelectedTab * render::scale_dpi(80)) + render::scale_dpi(93), m_vPos.y + render::scale_dpi(46), m_vPos.x + m_vSize.x - render::scale_dpi(7), m_vPos.y + render::scale_dpi(46), Color(0, 0, 0, CONTROLS::m_flMenuAlpha));
		render::line(m_vPos.x + render::scale_dpi(5) + (m_iSelectedTab * render::scale_dpi(80)) + render::scale_dpi(94), m_vPos.y + render::scale_dpi(47), m_vPos.x + m_vSize.x - render::scale_dpi(6), m_vPos.y + render::scale_dpi(47), Color(60, 60, 60, CONTROLS::m_flMenuAlpha));

		for (int i = 0; i < m_vTabs.size(); i++)
		{
			const RECT tabArea = { m_vPos.x + render::scale_dpi(19) + (i * render::scale_dpi(80)), m_vPos.y + render::scale_dpi(26), render::scale_dpi(80), render::scale_dpi(20) };

			if (UTILS::MouseInRegion(tabArea.left, tabArea.top, tabArea.right, tabArea.bottom) &&
				UTILS::KeyDown(VK_LBUTTON))
				m_iSelectedTab = i;

			if (m_iSelectedTab == i)
			{
				render::line(tabArea.left - render::scale_dpi(1), tabArea.top, tabArea.left - render::scale_dpi(1), tabArea.top + tabArea.bottom, Color(0, 0, 0, CONTROLS::m_flMenuAlpha));
				render::line(tabArea.left, tabArea.top + render::scale_dpi(1), tabArea.left, tabArea.top + tabArea.bottom + render::scale_dpi(1), Color(60, 60, 60, CONTROLS::m_flMenuAlpha));
			
				render::line(tabArea.left + tabArea.right, tabArea.top + render::scale_dpi(1), tabArea.left + tabArea.right, tabArea.top + tabArea.bottom + render::scale_dpi(1), Color(60, 60, 60, CONTROLS::m_flMenuAlpha));
				render::line(tabArea.left + tabArea.right + render::scale_dpi(1), tabArea.top, tabArea.left + tabArea.right + render::scale_dpi(1), tabArea.top + tabArea.bottom, Color(0, 0, 0, CONTROLS::m_flMenuAlpha));
			}

			render::string(
				tabArea.left + (tabArea.right * 0.5f),
				tabArea.top + render::scale_dpi(4),
				render::fonts::menu_font[g::scaling],
				m_vTabs[i],
				4,
				m_iSelectedTab == i ? Color(CONTROLS::m_cDefaultMenuCol, CONTROLS::m_flMenuAlpha) : Color(208, 208, 208, CONTROLS::m_flMenuAlpha));
		}
	}
}
void GUI::CONTROLS::EndWindow()
{
	PushFocus();
	ResetVars();
}

void GUI::CONTROLS::ResetVars()
{
	m_iColumn = 0;

	m_iLastControl = NONE;
	m_bCloseBoxes = false;
	m_bDisableBound = false;
	m_vOffset = vec2_t();
	m_vGroupboxPos = vec2_t();
	m_vOldGroupboxPos = { 19, 63 };
	m_szGroupboxName = ("");

	if (!m_vecColorPickers.empty())
		m_vecColorPickers.clear();
}

void GUI::CONTROLS::PopGroupboxY()
{
	if (!m_bInitialized)
		return;

	m_vOldGroupboxPos.y = 63;
}

void GUI::CONTROLS::PushFocus()
{
	ComboboxFocus();
	MultiboxFocus();
	KeybindFocus();
	ColorpickerFocus();
}