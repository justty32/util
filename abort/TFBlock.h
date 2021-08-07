#pragma once
#ifndef TFBLOCK_HPP
#define TFBLOCK_HPP
#include <algorithm>
#include <tuple>
#include <type_traits>
#include <memory>
#include <optional>
#include <functional>
#include <vector>
#include <unordered_map>
#include <initializer_list>
#include "MyDef.h"
#include "mfcontrol.hpp"

namespace zz {
	typedef std::tuple<char*, int, bool> TFBlock;
	template<typename T>
	std::optional<std::tuple<char*, int, bool>> ToBlock(T& target);
	template<typename T>
	bool FromBlock(T& refTarget, int& refSize, char* pp, int s, bool r = false);
	std::optional<std::tuple<char*, int, bool>> CatBlocks(std::initializer_list<std::tuple<char*, int, bool>> bl);
	std::vector<std::tuple<char*, int, bool>> UnCatBlocks(char* ptr, int size, bool r = false); template<typename Tk, typename Tv>
		std::optional<std::tuple<char*, int, bool>> ToBlockUmap(std::unordered_map<Tk, Tv>& umap);
	template<typename Tk, typename Tv>
	bool FromBlockUmap(std::unordered_map<Tk, Tv>& umap, char* pp, int size, bool r = false);
	template<typename T>
	std::optional<std::tuple<char*, int, bool>> ToBlockParr(T* arr, int arrsize);
	template<typename T>
	bool FromBlockParr(T** arrPtr, int& refArrSize, char* pp, int size, bool r);
	template<typename T>
	std::optional<std::tuple<char*, int, bool>> ToBlockVec(std::vector<T>& ve);
	template<typename T>
	bool FromBlockVec(std::vector<T>& ve, char* pp, int size, bool r = false);
	std::optional<std::tuple<char*, int, bool>> ToBlockStr(std::string& str);
	bool FromBlockStr(std::string& str, char* pp, int size, bool r = false);
}

#endif // !TFBLOCK_HPP

