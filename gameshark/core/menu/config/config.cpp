#include "config.h"

bool C::Setup(std::string_view szDefaultFileName)
{
	if (!std::filesystem::is_directory(fsPath / szConfigPath.c_str()))
	{
		std::filesystem::remove(fsPath / szConfigPath.c_str());
		if (!std::filesystem::create_directories(fsPath / szConfigPath.c_str()))
			return false;
	}

	if (!std::filesystem::exists(fsPath / szConfigPath.c_str() / szDefaultFileName))
	{
		if (!Save(szDefaultFileName))
			return false;
	}

	if (!Load(szDefaultFileName))
		return false;

	Refresh();
}

bool C::Save(std::string_view szFileName)
{
	std::filesystem::path fsFilePath(szFileName);
	if (fsFilePath.extension() != (".cfg"))
		fsFilePath.replace_extension((".cfg"));

	const std::string szFile = std::filesystem::path(fsPath / szConfigPath.c_str() / fsFilePath).string();
	nlohmann::json config;

	for (auto& variable : vecVariables)
	{
		nlohmann::json entry;

		// save hashes to compare it later
		entry[("name-id")] = variable.uNameHash;
		entry[("type-id")] = variable.uTypeHash;

		// get current variable
		switch (variable.uTypeHash)
		{
		case FNV1A::HashConst("int"):
		{
			entry[("value")] = variable.Get<int>();
			break;
		}
		case FNV1A::HashConst("float"):
		{
			entry[("value")] = variable.Get<float>();
			break;
		}
		case FNV1A::HashConst("bool"):
		{
			entry[("value")] = variable.Get<bool>();
			break;
		}
		case FNV1A::HashConst("Color"):
		{
			auto colVariable = variable.Get<Color>();

			nlohmann::json sub;

			sub.push_back(colVariable.r);
			sub.push_back(colVariable.g);
			sub.push_back(colVariable.b);
			sub.push_back(colVariable.a);

			entry[("value")] = sub.dump();
			break;
		}
		case FNV1A::HashConst("std::deque<bool>"):
		{
			auto vecVariable = variable.Get<std::deque<bool>>();

			nlohmann::json sub;

			for (auto& bValue : vecVariable)
				sub.push_back((bool)bValue);

			entry[("value")] = sub.dump();
			break;
		}
		case FNV1A::HashConst("std::deque<std::string>"):
		{
			auto vecVariable = variable.Get<std::deque<std::string>>();

			nlohmann::json sub;

			for (auto& bValue : vecVariable)
				sub.push_back((std::string)bValue);

			entry[("value")] = sub.dump();
			break;
		}
		case FNV1A::HashConst("std::deque<int>"):
		{
			auto vecVariable = variable.Get<std::deque<int>>();

			nlohmann::json sub;

			for (auto& bValue : vecVariable)
				sub.push_back((int)bValue);

			entry[("value")] = sub.dump();
			break;
		}
		case FNV1A::HashConst("std::deque<float>"):
		{
			auto vecVariable = variable.Get<std::deque<float>>();

			nlohmann::json sub;

			for (auto& bValue : vecVariable)
				sub.push_back((float)bValue);

			entry[("value")] = sub.dump();
			break;
		}
		default:
			break;
		}

		config.push_back(entry);
	}

	std::ofstream ofsOutFile(szFile, std::ios::out | std::ios::trunc);

	if (!ofsOutFile.good())
		return false;

	try
	{
		ofsOutFile << config.dump(4);
		ofsOutFile.close();
	}
	catch (std::ofstream::failure& ex)
	{
		return false;
	}

	return true;
}

bool C::Load(std::string_view szFileName)
{
	std::filesystem::path fsFilePath(szFileName);
	if (fsFilePath.extension() != (".cfg"))
		fsFilePath.replace_extension((".cfg"));

	const std::string szFile = std::filesystem::path(fsPath / szConfigPath.c_str() / fsFilePath).string();
	nlohmann::json config;

	std::ifstream ifsInputFile(szFile, std::ios::in);

	if (!ifsInputFile.good())
		return false;

	try
	{
		ifsInputFile >> config;
		ifsInputFile.close();
	}
	catch (std::ifstream::failure& ex)
	{
		return false;
	}

	for (auto& variable : config)
	{
		int nIndex = GetVariableIndex(variable[("name-id")].get<FNV1A_t>());

		if (nIndex == C_INVALID_VARIABLE)
			continue;

		auto& entry = vecVariables.at(nIndex);

		switch (variable[("type-id")].get<FNV1A_t>())
		{
		case FNV1A::HashConst("bool"):
		{
			entry.Set<bool>(variable[("value")].get<bool>());
			break;
		}
		case FNV1A::HashConst("float"):
		{
			entry.Set<float>(variable[("value")].get<float>());
			break;
		}
		case FNV1A::HashConst("int"):
		{
			entry.Set<int>(variable[("value")].get<int>());
			break;
		}
		case FNV1A::HashConst("Color"):
		{
			auto color = nlohmann::json::parse(variable[("value")].get<std::string>());

			entry.Set<Color>(Color(
				color.at(0).get<int>(),
				color.at(1).get<int>(),
				color.at(2).get<int>(),
				color.at(3).get<int>()
			));

			break;
		}
		case FNV1A::HashConst("std::deque<bool>"):
		{
			auto vector = nlohmann::json::parse(variable[("value")].get<std::string>());
			auto& vecVariable = entry.Get<std::deque<bool>>();

			for (std::size_t i = 0U; i < vector.size(); i++)
			{
				if (i < vecVariable.size())
					vecVariable.at(i) = vector.at(i).get<bool>();
			}

			break;
		}
		case FNV1A::HashConst("std::deque<std::string>"):
		{
			auto vector = nlohmann::json::parse(variable[("value")].get<std::string>());
			auto& vecVariable = entry.Get<std::deque<std::string>>();

			for (std::size_t i = 0U; i < vector.size(); i++)
			{
				if (i < vecVariable.size())
					vecVariable.at(i) = vector.at(i).get<std::string>();
			}

			break;
		}
		case FNV1A::HashConst("std::deque<int>"):
		{
			auto vector = nlohmann::json::parse(variable[("value")].get<std::string>());
			auto& vecVariable = entry.Get<std::deque<int>>();

			for (std::size_t i = 0U; i < vector.size(); i++)
			{
				if (i < vecVariable.size())
					vecVariable.at(i) = vector.at(i).get<int>();
			}

			break;
		}
		case FNV1A::HashConst("std::deque<float>"):
		{
			auto vector = nlohmann::json::parse(variable[("value")].get<std::string>());
			auto& vecVariable = entry.Get<std::deque<float>>();

			for (std::size_t i = 0U; i < vector.size(); i++)
			{
				if (i < vecVariable.size())
					vecVariable.at(i) = vector.at(i).get<float>();
			}

			break;
		}
		default:
			break;
		}
	}

	return true;
}

void C::Remove(std::string_view szFileName)
{
	std::filesystem::path fsFilePath(szFileName);
	if (fsFilePath.extension() != (".cfg"))
		fsFilePath.replace_extension((".cfg"));

	if (!szFileName.compare(("default")))
		return;

	const std::string szFile = std::filesystem::path(fsPath / szConfigPath.c_str() / szFileName).string();

	std::remove(szFile.c_str());
}

void C::Refresh()
{
	vecFileNames.clear();

	for (const auto& it : std::filesystem::directory_iterator(fsPath / szConfigPath.c_str()))
	{
		if (it.path().filename().extension() == (".cfg"))
		{
			std::string szName = it.path().filename().string();

			vecFileNames.push_back(szName);
		}
	}
}

std::size_t C::GetVariableIndex(const FNV1A_t uNameHash)
{
	for (std::size_t i = 0U; i < vecVariables.size(); i++)
	{
		if (vecVariables.at(i).uNameHash == uNameHash)
			return i;
	}

	return C_INVALID_VARIABLE;
}

std::filesystem::path C::GetWorkingPath()
{
	std::filesystem::path fsWorkingPath;

	if (PWSTR pszPathToDocuments; SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0UL, nullptr, &pszPathToDocuments)))
	{
		fsWorkingPath.assign(pszPathToDocuments);
		CoTaskMemFree(pszPathToDocuments);
	}

	return fsWorkingPath;
}
