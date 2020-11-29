#include "renderer.hpp"

int render::scale_dpi(int value, int scaling)
{
	return (value * (1 + (0.25f * scaling)));
}

int render::scale_dpi(int value)
{
	return (value * (1 + (0.25f * g::scaling)));
}

void render::initialize()
{
	for (int i = 0; i < 5; i++)
	{
		fonts::verdana[i] = interfaces::surface->font_create();
		interfaces::surface->set_font_glyph(fonts::verdana[i], "Verdana", scale_dpi(12, i), 350, 0, 0, FONTFLAG_DROPSHADOW);

		fonts::small_fonts[i] = interfaces::surface->font_create();
		interfaces::surface->set_font_glyph(fonts::small_fonts[i], "Small Fonts", scale_dpi(9, i), 350, 0, 0, FONTFLAG_OUTLINE | FONTFLAG_ANTIALIAS);

		fonts::verdana_big[i] = interfaces::surface->font_create();
		interfaces::surface->set_font_glyph(fonts::verdana_big[i], "Verdana Bold", scale_dpi(25, i), 500, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS);

		fonts::menu_font[i] = interfaces::surface->font_create();
		interfaces::surface->set_font_glyph(fonts::menu_font[i], "Verdana", scale_dpi(12, i), 100, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS);

		fonts::menu_bold_font[i] = interfaces::surface->font_create();
		interfaces::surface->set_font_glyph(fonts::menu_bold_font[i], "Verdana Bold", scale_dpi(12, i), 100, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS);

		fonts::menu_slider_font[i] = interfaces::surface->font_create();
		interfaces::surface->set_font_glyph(fonts::menu_slider_font[i], "Verdana", scale_dpi(12, i), 500, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS);
	}
}

void render::outlined_rect(int x, int y, int w, int h, Color col) 
{
	interfaces::surface->set_drawing_color(col);
	interfaces::surface->draw_outlined_rect(x, y, x + w, y + h);
}

void render::line(int x, int y, int x1, int y1, Color col) 
{
	interfaces::surface->set_drawing_color(col);
	interfaces::surface->draw_line(x, y, x1, y1);
}

void render::rect(int x, int y, int w, int h, Color col) 
{
	interfaces::surface->set_drawing_color(col);
	interfaces::surface->draw_filled_rectangle(x, y, x + w, y + h);
}

void render::rect_fade(int x, int y, int x1, int y1, Color first, Color second, bool horizontal, bool secondary_vert)
{
	if (!horizontal && !secondary_vert)
	{
		interfaces::surface->set_drawing_color(first);
		interfaces::surface->draw_filled_rect_fade(x, y, x + x1, y + y1, 255, 255, horizontal);

		interfaces::surface->set_drawing_color(second);
		interfaces::surface->draw_filled_rect_fade(x, y, x + x1, y + y1, 0, 255, horizontal);
	}
	else if (horizontal)
	{
		interfaces::surface->set_drawing_color(first);
		interfaces::surface->draw_filled_rect_fade(x, y, x + x1, y + y1, 255, 0, horizontal);

		interfaces::surface->set_drawing_color(first);
		interfaces::surface->draw_filled_rect_fade(x, y, x + x1, y + y1, 128, 0, horizontal);

		interfaces::surface->set_drawing_color(second);
		interfaces::surface->draw_filled_rect_fade(x, y, x + x1, y + y1, 0, 255, horizontal);

		interfaces::surface->set_drawing_color(second);
		interfaces::surface->draw_filled_rect_fade(x, y, x + x1, y + y1, 0, 128, horizontal);

		interfaces::surface->set_drawing_color(Color(0, 0, 0));
		interfaces::surface->draw_filled_rect_fade(x, y, x + x1, y + y1, 0, 255, false);
	}
	else if (secondary_vert)
	{
		interfaces::surface->set_drawing_color(first);
		interfaces::surface->draw_filled_rect_fade(x, y, x + x1, y + y1, 255, 0, false);

		interfaces::surface->set_drawing_color(first);
		interfaces::surface->draw_filled_rect_fade(x, y, x + x1, y + y1, 128, 0, false);

		interfaces::surface->set_drawing_color(second);
		interfaces::surface->draw_filled_rect_fade(x, y, x + x1, y + y1, 0, 255, false);

		interfaces::surface->set_drawing_color(second);
		interfaces::surface->draw_filled_rect_fade(x, y, x + x1, y + y1, 0, 125, false);
	}
}

void render::string(int x, int y, Color col, unsigned long font, const char* text, ...) 
{
	if (text == NULL) return;

	va_list			va_alist;
	char			buffer[1024];
	wchar_t			string[1024];

	va_start(va_alist, text);
	_vsnprintf(buffer, sizeof(buffer), text, va_alist);
	va_end(va_alist);

	MultiByteToWideChar(CP_UTF8, 0, buffer, 256, string, 256);

	interfaces::surface->draw_text_pos(x, y);
	interfaces::surface->draw_text_font(font);
	interfaces::surface->set_text_color(col);
	interfaces::surface->draw_render_text(string, wcslen(string));
}

void render::string(int x, int y, unsigned long font, std::string string, int mode, Color colour) 
{
	const auto converted_text = std::wstring(string.begin(), string.end());

	auto text_size = get_text_size(font, string.c_str());
	vec2_t pos;

	switch (mode) {
	case 0: pos = vec2_t(x, y); break;
	case 1: pos = vec2_t(int(x - (int(.5 + (text_size.left / 2)))), int(y - (int(.5 + (text_size.bottom / 2))) + 1)); break;
	case 2: pos = vec2_t(int(x - text_size.left), int(y - (int(.5 + (text_size.bottom / 2))) + 1)); break;
	case 3: pos = vec2_t(x, int(y - (int(.5 + (text_size.bottom / 2))) + 1)); break;
	case 4: pos = vec2_t(int(x - (int(.5 + (text_size.left / 2)))), y); break;
	}

	interfaces::surface->draw_text_pos(pos.x, pos.y);
	interfaces::surface->draw_text_font(font);
	interfaces::surface->set_text_color(colour);
	interfaces::surface->draw_render_text(converted_text.c_str(), wcslen(converted_text.c_str()));
}

RECT render::get_text_size(unsigned long font, std::string text) 
{
	size_t origsize = strlen(text.c_str()) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wcstring[newsize];
	mbstowcs_s(&convertedChars, wcstring, origsize, text.c_str(), _TRUNCATE);

	RECT rect; int x, y;
	interfaces::surface->get_text_size(font, wcstring, x, y);
	rect.left = x; rect.bottom = y;
	rect.right = x;
	return rect;
}