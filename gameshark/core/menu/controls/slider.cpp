#include "../gui.h"

static int iSliderSize = 146;

void GUI::CONTROLS::Slider(const std::string& szName, int* pValue, int iMin, int iMax, const char* szPrefix)
{
	if (!m_bInitialized)
		return;

	static bool bTakingInput = false, bBuffer = false;
	static std::string szCurrentName;

	render::string(m_vOffset.x + 21, m_vOffset.y, render::fonts::menu_font[g::scaling], szName, false, Color(210, 210, 210, CONTROLS::m_flMenuAlpha));

	const auto bInBound = UTILS::MouseInRegion(m_vOffset.x + 21, m_vOffset.y + 15, iSliderSize, 7);

	const float flMinMaxDelta = iMax - iMin;

	if (!m_bDisableBound)
	{
		bool bSkip = false;
		if (UTILS::KeyPressed(VK_LBUTTON) && bInBound)
		{
			bBuffer ? (bTakingInput = !bTakingInput, szCurrentName = szName, bBuffer = false) : NULL;
			bSkip = true;
		}
		else if (bInBound)
		{
			bBuffer = true;
		}

		if (UTILS::KeyDown(VK_LBUTTON) && bTakingInput && szCurrentName == szName)
		{
			*pValue = iMin + flMinMaxDelta * (UTILS::m_vMousePos.x - (m_vOffset.x + 21)) / iSliderSize;

			if (*pValue < iMin)
				*pValue = iMin;
			if (*pValue > iMax)
				*pValue = iMax;
		}
		else if (!UTILS::KeyDown(VK_LBUTTON) && !bInBound && bTakingInput)
		{
			bTakingInput = false;
		}

		m_bHoldingSlider = bTakingInput;
	}

	const float flValueMinDelta = *pValue - iMin;
	const float flValueMod = (flValueMinDelta / flMinMaxDelta) * iSliderSize;

	render::rect(m_vOffset.x + 21, m_vOffset.y + 15, iSliderSize, 7, Color(40, 40, 40, CONTROLS::m_flMenuAlpha));
	render::rect_fade(m_vOffset.x + 22, m_vOffset.y + 16, flValueMod - 2, 5, Color(CONTROLS::m_cDefaultMenuCol, CONTROLS::m_flMenuAlpha), Color(40, 40, 40, CONTROLS::m_flMenuAlpha));
	render::outlined_rect(m_vOffset.x + 21, m_vOffset.y + 15, iSliderSize, 7, Color(0, 0, 0, CONTROLS::m_flMenuAlpha));

	const std::string szValue = std::to_string(*pValue) + szPrefix;

	render::string(m_vOffset.x + 21 + flValueMod, m_vOffset.y + 16, render::fonts::menu_slider_font[g::scaling], szValue, 4, Color(185, 185, 185, CONTROLS::m_flMenuAlpha));

	m_vOffset.y += 30;

	m_iLastControl = SLIDER;
}
void GUI::CONTROLS::Slider(const std::string& szName, float* pValue, float flMin, float flMax, const char* szPrefix)
{
	if (!m_bInitialized)
		return;

	static bool bTakingInput = false, bBuffer = false;
	static std::string szCurrentName;

	render::string(m_vOffset.x + 21, m_vOffset.y, render::fonts::menu_font[g::scaling], szName, false, Color(210, 210, 210, CONTROLS::m_flMenuAlpha));

	const auto bInBound = UTILS::MouseInRegion(m_vOffset.x + 21, m_vOffset.y + 15, iSliderSize, 7);

	const float flMinMaxDelta = flMax - flMin;

	if (!m_bDisableBound)
	{
		bool bSkip = false;
		if (UTILS::KeyPressed(VK_LBUTTON) && bInBound)
		{
			bBuffer ? (bTakingInput = !bTakingInput, szCurrentName = szName, bBuffer = false) : NULL;
			bSkip = true;
		}
		else if (bInBound)
		{
			bBuffer = true;
		}

		if (UTILS::KeyDown(VK_LBUTTON) && bTakingInput && szCurrentName == szName)
		{
			*pValue = flMin + flMinMaxDelta * (UTILS::m_vMousePos.x - (m_vOffset.x + 21)) / iSliderSize;

			if (*pValue < flMin)
				*pValue = flMin;
			if (*pValue > flMax)
				*pValue = flMax;
		}
		else if (!UTILS::KeyDown(VK_LBUTTON) && !bInBound && bTakingInput)
		{
			bTakingInput = false;
		}

		m_bHoldingSlider = bTakingInput;
	}

	const float flValueMinDelta = *pValue - flMin;
	const float flValueMod = (flValueMinDelta / flMinMaxDelta) * iSliderSize;

	render::rect(m_vOffset.x + 21, m_vOffset.y + 15, iSliderSize, 7, Color(40, 40, 40, CONTROLS::m_flMenuAlpha));
	render::rect_fade(m_vOffset.x + 22, m_vOffset.y + 16, flValueMod - 2, 5, Color(CONTROLS::m_cDefaultMenuCol, CONTROLS::m_flMenuAlpha), Color(40, 40, 40, CONTROLS::m_flMenuAlpha));
	render::outlined_rect(m_vOffset.x + 21, m_vOffset.y + 15, iSliderSize, 7, Color(0, 0, 0, CONTROLS::m_flMenuAlpha));

	const std::string szValue = std::to_string((int)*pValue) + szPrefix;

	render::string(m_vOffset.x + 21 + flValueMod, m_vOffset.y + 16, render::fonts::menu_slider_font[g::scaling], szValue, 4, Color(185, 185, 185, CONTROLS::m_flMenuAlpha));

	m_vOffset.y += 30;

	m_iLastControl = SLIDER;
}