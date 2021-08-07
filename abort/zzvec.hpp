#pragma once
#ifndef ZZVEC_H
#define ZZVEC_H
#include <functional>
#include <optional>
#include <tuple>
#include <algorithm>

template<typename Tv, int cap>
class zzvec {
public:
	enum rt {
		good, outofrange, notfind, full, empty, bad
	};
	Tv arr[cap];
	int _st = 0; //how many elements
	bool isfull();
	bool isempty();
	bool has(Tv val);
	int add(Tv val);
	int insert(int index, Tv val);
	int remove(Tv val);
	int removeat(int index);
	std::tuple<Tv, int> popback();
	std::optional<Tv> at(int index);
	std::tuple<Tv, int> get(int index);
	std::tuple<Tv&, int> getref(int index);
	Tv* getptr(int index);
	std::optional<int> getindex(Tv val);
	int set(int index, Tv val);
	int swap(int index1, int index2);
	int count();
	int capacity();
	void fill(Tv val);
	void resize(int st);
	void clear();
	int foreach(std::function<bool(int index, Tv& val)> func);
	int foreachv(std::function<void(int index, Tv& val)> func);
};

#endif // ! ZZVEC_H

template<typename Tv, int cap>
inline bool zzvec<Tv, cap>::isfull()
{
	return _st >= cap;
}

template<typename Tv, int cap>
inline bool zzvec<Tv, cap>::isempty()
{
	return _st <= 0;
}

template<typename Tv, int cap>
inline bool zzvec<Tv, cap>::has(Tv val)
{
	for(int i = 0; i < _st; i++)
		if(arr[i] == val)
			return true;
	return false;
}

template<typename Tv, int cap>
inline int zzvec<Tv, cap>::add(Tv val)
{
	if (_st >= cap) return full;
	memcpy(&(arr[_st]), &val, sizeof(Tv));
	_st++;
	return good;
}

template<typename Tv, int cap>
inline int zzvec<Tv, cap>::insert(int index, Tv val)
{
	if (_st >= cap) return full;
	if (index < 0 || index >= _st) return outofrange;
	memcpy(&(arr[index+1]), &(arr[index]), sizeof(Tv) * (_st-1-index));
	memcpy(&(arr[index]), &val, sizeof(Tv));
	_st++;
	return good;
}

template<typename Tv, int cap>
inline int zzvec<Tv, cap>::remove(Tv val)
{
	if (_st <= 0) return empty;
	for(int i = 0; i < _st; i++)
		if (arr[i] == val) {
			memcpy(&(arr[i]), &(arr[i + 1]), sizeof(Tv) * (_st - 1 - i));
			_st--;
			return good;
		}
	return notfind;
}

template<typename Tv, int cap>
inline int zzvec<Tv, cap>::removeat(int index)
{
	if (_st <= 0) return empty;
	if (index < 0 || index >= _st) return outofrange;
	memcpy(&(arr[index]), &(arr[index + 1]), sizeof(Tv) * (_st - 1 - index));
	_st--;
	return good;
}

template<typename Tv, int cap>
inline std::tuple<Tv, int> zzvec<Tv, cap>::popback()
{
	if (_st <= 0) return std::make_tuple(Tv{}, empty);
	_st--;
	return std::make_tuple(arr[_st], good);
}

template<typename Tv, int cap>
inline std::optional<Tv> zzvec<Tv, cap>::at(int index)
{
	if (index < 0 || index >= cap) return std::nullopt;
	return arr[index];
}

template<typename Tv, int cap>
inline std::tuple<Tv, int> zzvec<Tv, cap>::get(int index)
{
	if (index < 0 || index >= _st) return std::tuple<Tv, int>(Tv{}, outofrange);
	return std::tuple<Tv, int>(arr[index], good);
}

template<typename Tv, int cap>
inline std::tuple<Tv&, int> zzvec<Tv, cap>::getref(int index)
{
	static Tv t;
	if (index < 0 || index >= _st) return std::tuple<Tv&, int>(t, outofrange);
	return std::tuple<Tv&, int>(arr[index], good);
}

template<typename Tv, int cap>
inline Tv* zzvec<Tv, cap>::getptr(int index)
{
	if (index < 0 || index >= _st) return nullptr;
	return &(arr[index]);
}

template<typename Tv, int cap>
inline std::optional<int> zzvec<Tv, cap>::getindex(Tv val)
{
	for(int i = 0; i < _st; i++)
		if(arr[i] == val)
			return i;
	return std::nullopt;
}

template<typename Tv, int cap>
inline int zzvec<Tv, cap>::set(int index, Tv val)
{
	if (index < 0 || index >= _st) return outofrange;
	memcpy(&(arr[index]), &val, sizeof(Tv));
	return good;
}

template<typename Tv, int cap>
inline int zzvec<Tv, cap>::swap(int index1, int index2)
{
	if (index1 < 0 || index1 >= _st) return outofrange;
	if (index2 < 0 || index2 >= _st) return outofrange;
	Tv t;
	memcpy(&t, &(arr[index1]), sizeof(Tv));
	memcpy(&(arr[index1]), &(arr[index2]), sizeof(Tv));
	memcpy(&(arr[index2]), &t, sizeof(Tv));
	return good;
}

template<typename Tv, int cap>
inline int zzvec<Tv, cap>::count()
{
	return _st;
}

template<typename Tv, int cap>
inline int zzvec<Tv, cap>::capacity()
{
	return cap;
}

template<typename Tv, int cap>
inline void zzvec<Tv, cap>::fill(Tv val)
{
	for(int i = 0; i < cap; i++)
		memcpy(&(arr[i]), &val, sizeof(Tv));
	_st = cap;
}

template<typename Tv, int cap>
inline void zzvec<Tv, cap>::resize(int st)
{
	if (st < 0 || st > cap) return;
	_st = st;
}

template<typename Tv, int cap>
inline void zzvec<Tv, cap>::clear()
{
	_st = 0;
}

template<typename Tv, int cap>
inline int zzvec<Tv, cap>::foreach(std::function<bool(int index, Tv& val)> func)
{
	for (int i = 0; i < _st; i++)
		if (func(i, arr[i])) return bad;
	return good;
}

template<typename Tv, int cap>
inline int zzvec<Tv, cap>::foreachv(std::function<void(int index, Tv& val)> func)
{
	for (int i = 0; i < _st; i++)
		func(i, arr[i]);
	return good;
}
