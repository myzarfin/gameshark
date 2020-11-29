#include "gui.h"
#include "variables.h"

std::uint8_t* CTextureHolder::DecodeResourceImage(unsigned short uID, unsigned int iWidth, unsigned int iHeight)
{
	const HRSRC pResource = FindResource(g::dll, MAKEINTRESOURCE(uID), "PNG");
	const HGLOBAL hLoadedResource = LoadResource(g::dll, pResource);
	const LPVOID pResourcePtr = LockResource(hLoadedResource);
	const DWORD dwResourceSize = SizeofResource(g::dll, pResource);

	std::vector< std::uint8_t > vecImage;

	if (const auto error = lodepng::decode(vecImage, iWidth, iHeight, (unsigned char*)pResourcePtr, dwResourceSize))
		throw std::runtime_error("Failed to decode image!");

	const auto pData = new std::uint8_t[vecImage.size()];

	std::copy(vecImage.begin(), vecImage.end(), pData);

	return pData;
}

void GUI::Run()
{
	if (UTILS::KeyPressed(VK_DELETE) ||
		UTILS::KeyPressed(VK_INSERT))
	{
		m_bOpened = !m_bOpened;

		if (g::local_player && !g::local_player->is_alive() || !interfaces::engine->is_in_game())
			interfaces::inputsystem->enable_input(!m_bOpened);

		C::Refresh();
	}

	static float flMenuAlpha = 0.0f;
	CONTROLS::m_flMenuAlpha = flMenuAlpha * 255.0f;

	if (!m_bOpened)
	{
		CONTROLS::m_bCloseBoxes = false;

		flMenuAlpha -= (1.0f / 0.2f) * interfaces::globals->frame_time;
		CONTROLS::uInfo.m_flAlpha = flMenuAlpha;
		if (flMenuAlpha <= 0.01f)
		{
			flMenuAlpha = 0.0f;
			return;
		}
	}
	else 
	{
		flMenuAlpha += (1.0f / 0.2f) * interfaces::globals->frame_time;
		if (flMenuAlpha >= 0.9f)
			flMenuAlpha = 1.0f;
	}

	CONTROLS::BeginWindow({ (float)C::Get<int>(Vars.menu_x), (float)C::Get<int>(Vars.menu_y) }, { (float)render::scale_dpi(440), (float)render::scale_dpi(520) });
	{
		/*switch (CONTROLS::m_iSelectedTab)
		{
		case 0: break;
		case 1: 
		{
			CONTROLS::BeginGroupbox("esp", 145);
			{
				CONTROLS::Colorpicker(("box_col"), &C::Get<Color>(Vars.box_col), &C::Get<Color>(Vars.box_col).a, true);
				CONTROLS::Checkbox(("box"), &C::Get<bool>(Vars.box));
				CONTROLS::Colorpicker(("name_col"), &C::Get<Color>(Vars.name_col), &C::Get<Color>(Vars.name_col).a, true);
				CONTROLS::Checkbox(("name"), &C::Get<bool>(Vars.name));
				CONTROLS::Checkbox(("health"), &C::Get<bool>(Vars.health));
				CONTROLS::Colorpicker(("weapon_col"), &C::Get<Color>(Vars.weapon_col), &C::Get<Color>(Vars.weapon_col).a, true);
				CONTROLS::Checkbox(("weapon"), &C::Get<bool>(Vars.weapon));
				CONTROLS::Colorpicker(("ammo_col"), &C::Get<Color>(Vars.ammo_col), &C::Get<Color>(Vars.ammo_col).a, true);
				CONTROLS::Checkbox(("ammo"), &C::Get<bool>(Vars.ammo));
				CONTROLS::Colorpicker(("glow_col"), &C::Get<Color>(Vars.glow_col), &C::Get<Color>(Vars.glow_col).a, true);
				CONTROLS::Checkbox(("glow"), &C::Get<bool>(Vars.glow));
				CONTROLS::Slider(("dormancy"), &C::Get<float>(Vars.dormancy), 0.0f, 10.0f, ("s"));
			}
			CONTROLS::EndGroupbox();

			CONTROLS::PopGroupboxY();

			CONTROLS::BeginGroupbox("chams", 145, 1);
			{
				CONTROLS::Colorpicker(("visible_col"), &C::Get<Color>(Vars.chams_col), &C::Get<Color>(Vars.chams_col).a, true);
				CONTROLS::Checkbox(("visible"), &C::Get<bool>(Vars.chams));
				CONTROLS::Colorpicker(("hidden_col"), &C::Get<Color>(Vars.chams_xqz_col), &C::Get<Color>(Vars.chams_xqz_col).a, true);
				CONTROLS::Checkbox(("hidden"), &C::Get<bool>(Vars.chams_xqz));

				CONTROLS::Colorpicker(("phong_col"), &C::Get<Color>(Vars.phong_col), &C::Get<Color>(Vars.phong_col).a, true);
				CONTROLS::Checkbox(("phong"), &C::Get<bool>(Vars.phong));

				CONTROLS::Colorpicker(("reflectivity_col"), &C::Get<Color>(Vars.reflectivity_col), &C::Get<Color>(Vars.reflectivity_col).a, true);
				CONTROLS::Checkbox(("reflectivity"), &C::Get<bool>(Vars.reflectivity));

				CONTROLS::Slider(("pearlescent"), &C::Get<float>(Vars.pearlescent), 0.0f, 100.0f, (""));
			}
			CONTROLS::EndGroupbox();
		}
		break;
		case 2:
		{
			CONTROLS::BeginGroupbox("visuals", 125);
			{
				CONTROLS::Checkbox("nightmode", &C::Get<bool>(Vars.nightmode));
				CONTROLS::Slider("brightness", &C::Get<float>(Vars.nightmode_brightness), 0.0f, 100.0f, "");
			
				CONTROLS::Keybind(("flashlight_key"), &C::Get<int>(Vars.flashlight_key), &C::Get<int>(Vars.flashlight_key_method));
				CONTROLS::Checkbox(("flashlight"), &C::Get<bool>(Vars.flashlight));

				CONTROLS::Slider(("flashlight fov"), &C::Get<float>(Vars.flashlight_fov), 0.0f, 100.0f, "");
			}
			CONTROLS::EndGroupbox();
		}break;
		case 3: 
		{
			CONTROLS::BeginGroupbox("misc", 35);
			{
				CONTROLS::Colorpicker("menu_col", &CONTROLS::m_cDefaultMenuCol, &CONTROLS::m_cDefaultMenuCol.a);
				CONTROLS::Text("menu accent");
			}
			CONTROLS::EndGroupbox();

			CONTROLS::BeginGroupbox("movement", 95);
			{
				CONTROLS::Checkbox(("bunnyhop"), &C::Get<bool>(Vars.bhop));

				CONTROLS::Checkbox(("infinite duck"), &C::Get<bool>(Vars.fast_duck), Color(170, 170, 100));

				CONTROLS::Keybind(("edgejump_key"), &C::Get<int>(Vars.edge_jump_key), &C::Get<int>(Vars.edge_key_method));
				CONTROLS::Checkbox(("edge jump"), &C::Get<bool>(Vars.edge_jump));

				CONTROLS::Keybind(("jumpbug_key"), &C::Get<int>(Vars.jump_bug_key), &C::Get<int>(Vars.jump_key_method));
				CONTROLS::Checkbox(("jump bug"), &C::Get<bool>(Vars.jump_bug));

				CONTROLS::Keybind(("edgebug_key"), &C::Get<int>(Vars.edge_bug_key), &C::Get<int>(Vars.edge_bug_method));
				CONTROLS::Checkbox(("edge bug"), &C::Get<bool>(Vars.edge_bug));
			}
			CONTROLS::EndGroupbox();

			CONTROLS::PopGroupboxY();

			CONTROLS::BeginGroupbox(("configs"), 165, 1);
			{
				static std::string pText = "new config";
				static int iConfigSelection = 0;
				std::vector<std::string> vConfig = { };
				for (auto i = 0; i < C::vecFileNames.size(); i++)
					vConfig.push_back(C::vecFileNames.at(i));

				CONTROLS::Combobox(("configs"), vConfig, &iConfigSelection);

				CONTROLS::TextField(("config name"), &pText);

				CONTROLS::Button(("create config"), [&](bool& holder) {
					if (pText.empty())
					{
						holder = !holder;
						return;
					}

					C::Save(pText);
					C::Refresh();
					holder = !holder;
					});

				CONTROLS::Button(("save config"), [&](bool& holder) {
					if (vConfig.empty())
					{
						holder = !holder;
						return;
					}

					C::Save(vConfig.at(iConfigSelection));
					holder = !holder;
					});

				CONTROLS::Button(("load config"), [&](bool& holder) {
					if (vConfig.empty())
					{
						holder = !holder;
						return;
					}

					C::Load(vConfig.at(iConfigSelection));
					holder = !holder;
					});
			}
			CONTROLS::EndGroupbox();
		} break;
		case 4: break;
		}*/

		if (GUI::UTILS::KeyPressed(VK_OEM_4))
			g::scaling-=1;

		if (GUI::UTILS::KeyPressed(VK_OEM_6))
			g::scaling+=1;

		if (g::scaling < 0)
			g::scaling = 0;

		if (g::scaling > 4)
			g::scaling = 4;

		static bool bCheckboxOne = false, bCheckboxTwo = false;
		static int iKeyBind, iKeyBindMethod, iCombobox = 0, iSlider;
		static std::deque<bool> vecMulticombo = { false, false, false, false, false };
		static std::string szText, szNumber;
		static float flSlider;

		CONTROLS::BeginGroupbox("Groupbox", 105);
			CONTROLS::Colorpicker("MenuColor", &CONTROLS::m_cDefaultMenuCol, &CONTROLS::m_cDefaultMenuCol.a, true);
			CONTROLS::Checkbox("Checkbox", &bCheckboxOne);

			CONTROLS::Keybind("KeyBind", &iKeyBind, &iKeyBindMethod);
			CONTROLS::Checkbox("Another checkbox", &bCheckboxTwo, Color(170, 170, 100));

			CONTROLS::Slider("Int slider", &iSlider, 0, 100);

			CONTROLS::Slider("Float slider", &flSlider, 0, 1000);
		CONTROLS::EndGroupbox();

		CONTROLS::PopGroupboxY();

		CONTROLS::BeginGroupbox("Another groupbox", 243, 1);

			CONTROLS::Combobox("Combobox", { "One", "Two", "Three", "Four", "Five" }, &iCombobox);
			CONTROLS::Multibox("Multi combobox", { "One", "Two", "Three", "Four", "Five" }, &vecMulticombo);

			CONTROLS::TextField("Text input", &szText);
			CONTROLS::NumberField("Number input", &szNumber);

			CONTROLS::Button("Button", [&](bool& holder) {
				holder = !holder;
			});

			CONTROLS::Combobox("DPI Scaling", { "100%", "125%", "150%", "175%", "200%" }, &g::scaling);

		CONTROLS::EndGroupbox();
	}
	CONTROLS::EndWindow();
}