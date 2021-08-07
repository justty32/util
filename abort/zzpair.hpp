#pragma once
#ifndef ZZPAIR_H
#define ZZPAIR_H
#include <functional>
#include <optional>
#include <tuple>
#include <algorithm>

template<int cap, typename Tv>
class zzpair {
public:
	enum rt {
		good, outofrange, notfind, full, bad
	};
	std::pair<int, Tv> arr[cap];
	int nkey = 0;
	bool isfull();
	bool isempty();
	bool has(int key);
	int add(int key, Tv val);
	int remove(int key);
	std::tuple<Tv, int> get(int key); //second tuple element is result
	std::tuple<Tv&, int> getref(int key); //second tuple element is result
	Tv* getptr(int key);
	std::optional<int> getkey(Tv val);
	int set(int key, Tv val);
	int swap(int key1, int key2);
	int count();
	int capacity();
	void fill(Tv val);
	void setnkey(int nk = 0);
	void clear(int nk = 0);
	std::optional<int> findindex(int key);
	int foreach(std::function<bool(int& key, Tv& val)> func);
	int foreachv(std::function<void(int& key, Tv& val)> func);
	int foreachnk(std::function<bool(int index, int& key, Tv& val)> func);
	int foreachnkv(std::function<void(int index, int& key, Tv& val)> func);
};

template<int cap, typename Tv>
inline bool zzpair<cap, Tv>::isfull()
{
	for (int i = 0; i < cap; i++)
		if (arr[i].first == nkey)
			return false;
	return true;
}

template<int cap, typename Tv>
inline bool zzpair<cap, Tv>::isempty()
{
	for (int i = 0; i < cap; i++)
		if (arr[i].first != nkey)
			return false;
	return true;
}

template<int cap, typename Tv>
inline bool zzpair<cap, Tv>::has(int key)
{
	for (int i = 0; i < cap; i++)
		if (arr[i].first == key)
			return true;
	return false;
}

template<int cap, typename Tv>
inline int zzpair<cap, Tv>::add(int key, Tv val)
{
	for (int i = 0; i < cap; i++)
		if (arr[i].first == nkey) {
			arr[i].first = key;
			memcpy(&(arr[i].second), &val, sizeof(Tv));
			return good;
		}
	return full;
}

template<int cap, typename Tv>
inline int zzpair<cap, Tv>::remove(int key)
{
	for (int i = 0; i < cap; i++)
		if (arr[i].first == key) {
			arr[i].first = nkey;
			return good;
		}
	return notfind;
}

template<int cap, typename Tv>
inline std::tuple<Tv, int> zzpair<cap, Tv>::get(int key)
{
	for (int i = 0; i < cap; i++)
		if (arr[i].first == key)
			return std::make_tuple(arr[i].second, good);
	return std::make_tuple(Tv{}, notfind);
}

template<int cap, typename Tv>
inline std::tuple<Tv&, int> zzpair<cap, Tv>::getref(int key)
{
	static Tv t;
	for (int i = 0; i < cap; i++)
		if (arr[i].first == key)
			return std::make_tuple(arr[i].second, good);
	return std::make_tuple(t, notfind);
}

template<int cap, typename Tv>
inline Tv* zzpair<cap, Tv>::getptr(int key)
{
	for (int i = 0; i < cap; i++)
		if (arr[i].first == key)
			return &(arr[i].second);
	return nullptr;
}

template<int cap, typename Tv>
inline std::optional<int> zzpair<cap, Tv>::getkey(Tv val)
{
	for (int i = 0; i < cap; i++)
		if (arr[i].second == val)
			return arr[i].first;
	return std::nullopt;
}

template<int cap, typename Tv>
inline int zzpair<cap, Tv>::set(int key, Tv val)
{
	for (int i = 0; i < cap; i++)
		if (arr[i].first == key) {
			memcpy(&(arr[i].second), &val, sizeof(Tv));
			return good;
		}
	return notfind;
}

template<int cap, typename Tv>
inline int zzpair<cap, Tv>::swap(int key1, int key2)
{
	inline int i1 = -1; inline int i2 = -1;
	inline Tv* temp = new char[sizeof(Tv)];
	for (int i = 0; i < cap; i++) {
		if (arr[i].first == key1) i1 = i;
		if (arr[i].first == key2) i2 = i;
	}
	if (i1 == i2) return bad;
	if (i1 < 0 || i2 < 0) return notfind;
	memcpy(temp, &(arr[i1].second), sizeof(Tv));
	memcpy(&(arr[i1].second), &(arr[i2].second), sizeof(Tv));
	memcpy(&(arr[i2].second), temp, sizeof(Tv));
	return good;
}

template<int cap, typename Tv>
inline int zzpair<cap, Tv>::count()
{
	int ct = 0;
	for (int i = 0; i < cap; i++)
		if (arr[i].first != nkey)
			ct++;
	return ct;
}

template<int cap, typename Tv>
inline int zzpair<cap, Tv>::capacity()
{
	return cap;
}

template<int cap, typename Tv>
inline void zzpair<cap, Tv>::fill(Tv val)
{
	for (int i = 0; i < cap; i++)
		if (arr[i].first != nkey)
			memcpy(&(arr[i].second), &(val), sizeof(Tv));
}

template<int cap, typename Tv>
inline void zzpair<cap, Tv>::setnkey(int nk)
{
	for (int i = 0; i < cap; i++)
		if (arr[i].first == nkey)
			arr[i].first = nk;
	nkey = nk;
}

template<int cap, typename Tv>
inline void zzpair<cap, Tv>::clear(int nk)
{
	for (int i = 0; i < cap; i++)
		arr[i].first = nk;
}

template<int cap, typename Tv>
inline std::optional<int> zzpair<cap, Tv>::findindex(int key)
{
	for (int i = 0; i < cap; i++)
		if (arr[i].first == key)
			return i;
	return std::nullopt;
}

template<int cap, typename Tv>
inline int zzpair<cap, Tv>::foreach(std::function<bool(int& key, Tv& val)> func)
{
	for (int i = 0; i < cap; i++)
		if(arr[i].first != nkey)
			if (func(arr[i].first, arr[i].second))
				return bad;
	return good;
}

template<int cap, typename Tv>
inline int zzpair<cap, Tv>::foreachv(std::function<void(int& key, Tv& val)> func)
{
	for (int i = 0; i < cap; i++)
		if(arr[i].first != nkey)
			func(arr[i].first, arr[i].second);
	return good;
}

template<int cap, typename Tv>
inline int zzpair<cap, Tv>::foreachnk(std::function<bool(int index, int& key, Tv& val)> func)
{
	for (int i = 0; i < cap; i++)
		if(arr[i].first == nkey)
			if (func(i, arr[i].first, arr[i].second))
				return bad;
	return good;
}

template<int cap, typename Tv>
inline int zzpair<cap, Tv>::foreachnkv(std::function<void(int index, int& key, Tv& val)> func)
{
	for (int i = 0; i < cap; i++)
		if (arr[i].first == nkey)
			func(i, arr[i].first, arr[i].second);
	return good;
}

#endif // ! ZZPAIR_H
