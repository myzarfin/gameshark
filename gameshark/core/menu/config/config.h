#pragma once
#include <any>
#include <filesystem>
#include <deque>
#include <vector>

#include "../../../dependencies/utilities/csgo.hpp"

template <typename T, std::size_t S>
inline std::deque<T> GetFilledVector(const T& fill)
{
	std::deque<T> vecTemp(S);
	std::fill(vecTemp.begin(), vecTemp.begin() + S, fill);
	return vecTemp;
}

#define C_ADDVARIABLE( Type, szName, pDefault ) const std::uint32_t szName = C::AddVariable<Type>(FNV1A::HashConst(#szName), FNV1A::HashConst(#Type), pDefault);
#define C_ADDVARIABLE_VECTOR( Type, uSize, szName, pDefault ) const std::uint32_t szName = C::AddVariable<std::deque<Type>>(FNV1A::HashConst(#szName), FNV1A::HashConst("std::deque<" #Type ">"), GetFilledVector<Type, uSize>(pDefault));
#define C_INVALID_VARIABLE (std::size_t)-1

struct VariableObject_t
{
	VariableObject_t() = default;

	explicit VariableObject_t(const FNV1A_t uNameHash, const FNV1A_t uTypeHash, const std::any pDefault)
		: uNameHash(uNameHash), uTypeHash(uTypeHash), pValue(pDefault) { }

	~VariableObject_t() = default;

	template<typename T>
	T& Get()
	{
		return *(T*)(std::any_cast<T>(&pValue));
	}

	template<typename T>
	void Set(T value)
	{
		pValue.emplace<T>(value);
	}

	FNV1A_t		uNameHash;
	FNV1A_t		uTypeHash;
	std::any	pValue;
};

namespace C
{
	bool Setup(std::string_view szDefaultFileName);
	bool Save(std::string_view szFileName);
	bool Load(std::string_view szFileName);

	void Remove(std::string_view szFileName);
	void Refresh();

	std::size_t GetVariableIndex(const FNV1A_t uNameHash);
	std::filesystem::path GetWorkingPath();

	inline std::vector<VariableObject_t> vecVariables = { };

	template <typename T>
	T& Get(const std::uint32_t nIndex)
	{
		return vecVariables.at(nIndex).Get<T>();
	}

	template <typename T>
	std::uint32_t AddVariable(const FNV1A_t uNameHash, const FNV1A_t uTypeHash, const T pDefault)
	{
		vecVariables.push_back(VariableObject_t(uNameHash, uTypeHash, std::make_any<T>(pDefault)));
		return vecVariables.size() - 1U;
	}

	const std::filesystem::path fsPath = GetWorkingPath();
	inline std::deque<std::string> vecFileNames = { };
	inline std::string szConfigPath = "gameshark";
}
