#pragma once
#include "../../utilities/csgo.hpp"

enum font_flags {
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

namespace render {
	void initialize();

	void outlined_rect(int x, int y, int w, int h, Color col);
	void line(int x, int y, int x1, int y1, Color col);
	void rect(int x, int y, int w, int h, Color col);
	void rect_fade(int x, int y, int x1, int y1, Color first, Color second, bool = false, bool = false);
	void string(int x, int y, Color col, unsigned long font, const char* text, ...);
	void string(int, int, unsigned long, std::string, int, Color);
	RECT get_text_size(unsigned long font, std::string text);

	int scale_dpi(int value, int scaling);
	int scale_dpi(int value);

	namespace fonts {
		inline unsigned long verdana[5];
		inline unsigned long verdana_big[5];
		inline unsigned long small_fonts[5];

		inline unsigned long menu_font[5];
		inline unsigned long menu_bold_font[5];
		inline unsigned long menu_slider_font[5];
	}
}
