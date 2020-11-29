#include "../gui.h"

void GUI::CONTROLS::Colorpicker(const std::string& szName, Color* pColor, int* pAlpha, const bool bAlpha, const int iColumn)
{
	if (!m_bInitialized)
		return;

	bool bMouseInBound = UTILS::MouseInRegion(m_vOffset.x + 165, m_vOffset.y, 25, 9);
	static auto bBuffer = false;
	bool bSkip = false;

	if (UTILS::KeyDown(VK_CONTROL) && UTILS::KeyDown(0x43) && bMouseInBound)
		m_cCopiedColor = *pColor;

	if (UTILS::KeyDown(VK_CONTROL) && UTILS::KeyDown(0x56) && bMouseInBound)
		*pColor = m_cCopiedColor;

	if (UTILS::KeyPressed(VK_LBUTTON) && bMouseInBound)
	{
		bBuffer ? (uInfo.m_bOpen = !uInfo.m_bOpen, uInfo.m_szName = szName, uInfo.m_flAlpha = 0.0f, bBuffer = false) : NULL;
		bSkip = true;
	}
	else if (bMouseInBound)
	{
		bBuffer = true;
	}

	auto bInColorBox = UTILS::MouseInRegion(uInfo.m_vOffset.x, uInfo.m_vOffset.y, 164, 150);
	if (uInfo.m_bOpen && uInfo.m_szName == szName)
	{
		if ((UTILS::KeyDown(VK_LBUTTON) && !(bMouseInBound || bInColorBox)) && !bSkip || m_bCloseBoxes)
			uInfo.m_bOpen = false;
	}

	if (uInfo.m_bOpen && uInfo.m_szName == szName)
	{
		if (m_bCloseBoxes)
			uInfo.m_bOpen = false;

		uInfo.m_iPrevControl = COLORPICKER;
		uInfo.m_pColor = pColor;
		uInfo.m_iAlpha = pAlpha;
		uInfo.m_iColumn = iColumn;
		uInfo.m_vOffset.x = m_vOffset.x + 165;
		uInfo.m_vOffset.y = m_vOffset.y + 20;
		uInfo.m_bAlphaSlider = bAlpha;
		m_bDisableBound = true;
	}

	render::rect_fade(m_vOffset.x + 167, m_vOffset.y + 4, 14, 7, *pColor, Color(0, 0, 0, CONTROLS::m_flMenuAlpha));
	render::outlined_rect(m_vOffset.x + 166, m_vOffset.y + 3, 16, 9, Color(0, 0, 0, CONTROLS::m_flMenuAlpha));

	m_iLastControl = COLORPICKER;
}

std::unique_ptr<Color[]> pGradient = nullptr;

void ColorSpectrum(int x, int y, int w, int h)
{
	if (!pGradient)
	{
		pGradient = std::make_unique<Color[]>(w * h);

		float hue = 0.0f, sat = 0.99f, lum = 1.0f;
		for (int i = 0; i < w; i++)
		{
			for (int j = 0; j < h; j++)
			{
				*(Color*)(pGradient.get() + j + i * h) = Color::HSLToRGB(hue, sat, lum);

				hue += (1.f / w);
			}

			lum -= (1.f / h);
			hue = 0.f;
		}
	}
}

Color GetOfficialColor(int x, int y)
{
	return *(Color*)(pGradient.get() + x + y * 150);
}

void GUI::CONTROLS::ColorpickerFocus()
{
	if (uInfo.m_iPrevControl == COLORPICKER)
	{
		if (uInfo.m_bOpen)
		{
			if (m_bCloseBoxes)
				uInfo.m_bOpen = false;

			const vec2_t vecPosition = uInfo.m_vOffset;
			const vec2_t vecRelative = { UTILS::m_vMousePos.x - vecPosition.x, UTILS::m_vMousePos.y - vecPosition.y };
			static vec2_t vecSize = { 150, 150 };

			render::rect(vecPosition.x - 4, vecPosition.y - 4, uInfo.m_bAlphaSlider ? 173 : 158, 158, Color(40, 40, 40, CONTROLS::m_flMenuAlpha));
			render::outlined_rect(vecPosition.x - 4, vecPosition.y - 4, uInfo.m_bAlphaSlider ? 173 : 158, 158, Color(0, 0, 0, CONTROLS::m_flMenuAlpha));
			render::outlined_rect(vecPosition.x - 3, vecPosition.y - 3, uInfo.m_bAlphaSlider ? 171 : 156, 156, Color(60, 60, 60, CONTROLS::m_flMenuAlpha));

			ColorSpectrum(vecPosition.x, vecPosition.y, vecSize.x, vecSize.y);

			colorPicker.Draw(vecPosition.x, vecPosition.y, Color(255, 255, 255, CONTROLS::m_flMenuAlpha), (1 + (0.25 * g::scaling)));

			static vec2_t vecMousPos;

			if (UTILS::KeyDown(VK_LBUTTON) &&
				UTILS::MouseInRegion(vecPosition.x, vecPosition.y, vecSize.x, vecSize.y))
			{
				*uInfo.m_pColor =
					Color(
						GetOfficialColor(vecRelative.x, vecRelative.y).r,
						GetOfficialColor(vecRelative.x, vecRelative.y).g,
						GetOfficialColor(vecRelative.x, vecRelative.y).b,
						*uInfo.m_iAlpha
					);

				vecMousPos = UTILS::m_vMousePos;
			}

			if (vecMousPos.x < vecPosition.x)
				vecMousPos.x = vecPosition.x;
			if (vecMousPos.y < vecPosition.y)
				vecMousPos.y = vecPosition.y;
			if (vecMousPos.x + 4 > vecPosition.x + vecSize.x)
				vecMousPos.x = vecPosition.x + vecSize.x - 4;
			if (vecMousPos.y + 4 > vecPosition.y + vecSize.y)
				vecMousPos.y = vecPosition.y + vecSize.y - 4;

			render::rect(vecMousPos.x + 1, vecMousPos.y + 1, 2, 2, Color(124, 159, 222, CONTROLS::m_flMenuAlpha));
			render::outlined_rect(vecMousPos.x, vecMousPos.y, 4, 4, Color(0, 0, 0, CONTROLS::m_flMenuAlpha));

			if (uInfo.m_bAlphaSlider)
			{
				// Create our Alpha slider position.
				const vec2_t vecAlphaPos = { vecPosition.x + vecSize.x + 3, vecPosition.y };
				const vec2_t vecAlphaSize = { 12, vecSize.y };

				if (UTILS::KeyDown(VK_LBUTTON) && UTILS::MouseInRegion(vecAlphaPos.x, vecAlphaPos.y, vecAlphaSize.x, vecAlphaSize.y))
				{
					*uInfo.m_iAlpha = 255 * (UTILS::m_vMousePos.y - vecAlphaPos.y) / vecAlphaSize.y;

					if (*uInfo.m_iAlpha < 5)
						*uInfo.m_iAlpha = 0;

					if (*uInfo.m_iAlpha > 245)
						*uInfo.m_iAlpha = 255;
				}

				const float flValueMod = (*uInfo.m_iAlpha / 255.0f) * vecAlphaSize.y;
				transparentBackground.Draw(vecAlphaPos.x, vecAlphaPos.y, Color(255, 255, 255, CONTROLS::m_flMenuAlpha), (1 + (0.25 * g::scaling)));
				render::rect_fade(vecAlphaPos.x, vecAlphaPos.y, vecAlphaSize.x, vecAlphaSize.y, Color(*uInfo.m_pColor, 0), Color(*uInfo.m_pColor, CONTROLS::m_flMenuAlpha), false, true);

				float flYPos = flValueMod;
				if (flValueMod >= vecAlphaSize.y - 3)
					flYPos = vecAlphaSize.y - 3;

				render::rect(vecAlphaPos.x + 1, vecAlphaPos.y + flYPos + 1, vecAlphaSize.x - 2, 1, Color(124, 159, 222, CONTROLS::m_flMenuAlpha));
				render::outlined_rect(vecAlphaPos.x, vecAlphaPos.y + flYPos, vecAlphaSize.x, 3, Color(0, 0, 0, CONTROLS::m_flMenuAlpha));
				render::outlined_rect(vecAlphaPos.x, vecAlphaPos.y, vecAlphaSize.x, vecAlphaSize.y, Color(0, 0, 0, CONTROLS::m_flMenuAlpha));
			}
		}
	}
}