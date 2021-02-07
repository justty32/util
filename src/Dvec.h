#pragma once
#ifndef DVEC_H
#define DVEC_H

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <list>
#include <set>
#include <tuple>
#include <algorithm>
#include <initializer_list>
#include "./Mfp.h"

// put it outside of class def field, ex. class ee{}; SERIA..(ee)
#define UIO_SERIALIZE_FUNCS_OUT_OF_CLASS(ClassName)\
template<> uio::serialize::Dvec uio::serialize::to_dvec<ClassName>(ClassName& target){ return target.to_dvec(); }\
template<> ClassName uio::serialize::from_dvec<ClassName>(uio::serialize::Dvec dvec){ClassName c{}; c.from_dvec(dvec); return c;}
// put it inside, fill with class members, ex. class ee{ SERIAL..(x,y,c) }
#define UIO_SERIALIZE_FUNCS_IN_CLASS_PUBLIC(...)\
uio::serialize::Dvec to_dvec(){return uio::serialize::detail::to_dvec_pack(__VA_ARGS__);}\
void from_dvec(uio::serialize::Dvec dvec){uio::serialize::detail::from_dvec_pack(dvec, __VA_ARGS__);}

namespace uio{
	namespace serialize{

		using Dvec = std::vector<char>;

		template<typename T>
		Dvec to_dvec(T& target) {
			if constexpr (detail::is_specialization <T, std::vector>::value
				|| detail::is_specialization <T, std::list>::value
				|| detail::is_specialization <T, std::map>::value
				|| detail::is_specialization <T, std::set>::value
				|| std::is_array<T>::value)
			{
				return detail::to_dvec_orderer_container(target);
			}
			else if constexpr (detail::is_specialization <T, std::pair>::value) {
				return detail::to_dvec_pair(target);
			}
			else if constexpr (detail::is_specialization <T, std::tuple>::value) {
				return detail::to_dvec_tuple(target);
			}
			else if constexpr (std::is_same<T, std::string>::value) {
				return detail::to_dvec_str(target);
			}
			else {
				static_assert(std::is_pod<T>::value, "T is not POD type, please define its to_dvec");
				Dvec d;
				d.resize(sizeof(T));
				memcpy(d.data(), &target, d.size());
				return d;
			}
		}

		//pointer set to nullptr
		template<typename T> 
		T from_dvec(Dvec dvec) { 
			if constexpr (detail::is_specialization <T, std::vector>::value
				|| detail::is_specialization <T, std::list>::value
				|| detail::is_specialization <T, std::map>::value
				|| detail::is_specialization <T, std::set>::value
				|| std::is_array<T>::value)
			{
				return detail::from_dvec_orderer_container<T>(dvec);
			}
			else if constexpr (detail::is_specialization <T, std::pair>::value) {
				return detail::from_dvec_pair<T>(dvec);
			}
			else if constexpr (detail::is_specialization <T, std::tuple>::value) {
				return detail::from_dvec_tuple<T>(dvec);
			}
			else if constexpr (std::is_same <T, std::string>::value) {
				return detail::from_dvec_str(dvec);
			}
			else {
				static_assert(std::is_pod<T>::value, "T is not POD type, please define its to_dvec");
				typename std::remove_const<T>::type target{};
				if (dvec.empty()) return target;
				if (dvec.size() <= sizeof(target))
					memcpy(&target, dvec.data(), dvec.size());
				else
					memcpy(&target, dvec.data(), sizeof(T));
				if constexpr (std::is_pointer<T>::value)
					return nullptr;
				else
					return target; 
			}
		}

		template<typename T>
		void from_dvec(T& target, Dvec dvec) { target = from_dvec<T>(dvec); }

		namespace detail {

			template<typename Test, template<typename...> class Ref>
			struct is_specialization : std::false_type {};
			template<template<typename...> class Ref, typename... Args>
			struct is_specialization<Ref<Args...>, Ref> : std::true_type {};

			Dvec to_dvec_str(std::string& target);
			std::string from_dvec_str(Dvec dvec);

			// split-dvec's struct must be like up 
			std::vector<Dvec> split_dvec(Dvec dvec);
			// read data from mf, make it dvec
			// and ref mf will be seeked
			Dvec mf_get_dvec(Mfp& mf);
			// not useful
			size_t cp_with_size(char* ptr, Dvec d);

			// {len, size1, data1, size2, data2, .....}
			template<typename T>
			Dvec merge_dvec(T ds, size_t size = 0) {
				// ds must be vector<Dvec>, or list<Dvec>
				// size means all dvec in ds 's size sum
				if (size == 0)
					for (auto iter = ds.begin(); iter != ds.end(); ++iter)
						size += iter->size();
				Dvec result(size + ds.size() * sizeof(size_t) + sizeof(size_t));
				Mfp mf{ result.data(), result.size() };
				mf.set(ds.size());
				for (auto iter = ds.begin(); iter != ds.end(); ++iter) {
					mf.set(iter->size());
					mf.write(iter->data(), iter->size());
				}
					//fp += cp_with_size(result.data() + fp, *iter);
				return result;
			}

			template<typename T>
			Dvec to_dvec_pair(T& target) {
				Dvec subk = to_dvec(target.first);
				Dvec subv = to_dvec(target.second);
				return merge_dvec(std::vector<Dvec>{subk, subv}
					, subk.size() + subv.size());
			}
			template<typename T>
			T from_dvec_pair(Dvec dvec) { 
				if (dvec.empty()) return {};
				Mfp mf{ dvec.data(), dvec.size() };
				size_t target_size = mf.get<size_t>();
				if (target_size != 2) return {};
				auto subk = mf_get_dvec(mf);
				auto subv = mf_get_dvec(mf);
				return std::make_pair(
					from_dvec<std::remove_const<T::first_type>::type>(subk),
					from_dvec<std::remove_const<T::second_type>::type>(subv)
				);
			}

			template<typename T>
			Dvec to_dvec_tuple(T& target) {
				constexpr size_t ts = std::tuple_size<T>::value;
				std::vector<Dvec> ds;
				size_t size = 0;
				auto pds = [&](Dvec sub) { size += sub.size(); ds.push_back(sub); };
				if constexpr (ts > 5)
					static_assert("not yet support that the tuple size bigger than 5");
				else if constexpr (ts == 1)
					pds(to_dvec(std::get<0>(target)));
				else if constexpr (ts == 2) {
					pds(to_dvec(std::get<0>(target))); pds(to_dvec(std::get<1>(target)));
				}
				else if constexpr (ts == 3) {
					pds(to_dvec(std::get<0>(target))); pds(to_dvec(std::get<1>(target))); 
					pds(to_dvec(std::get<2>(target)));
				}
				else if constexpr (ts == 4) {
					pds(to_dvec(std::get<0>(target))); pds(to_dvec(std::get<1>(target)));
					pds(to_dvec(std::get<2>(target))); pds(to_dvec(std::get<3>(target)));
				}
				else if constexpr (ts == 5) {
					pds(to_dvec(std::get<0>(target))); pds(to_dvec(std::get<1>(target)));
					pds(to_dvec(std::get<2>(target))); pds(to_dvec(std::get<3>(target)));
					pds(to_dvec(std::get<4>(target)));
				}
				return merge_dvec(ds, size);
			}
			template<typename T>
			T from_dvec_tuple(Dvec dvec) { 
				constexpr size_t ts = std::tuple_size<T>::value;
				if (dvec.empty()) return {};
				auto subs = split_dvec(dvec);
				if (subs.size() != ts) return {};
				if constexpr (ts > 5)
					static_assert("not yet support that the tuple size bigger than 5");
				else if constexpr (ts == 1)
					return std::make_tuple(from_dvec<std::tuple_element<0, T>::type>(subs[0]));
				else if constexpr (ts == 2)
					return std::make_tuple(from_dvec<std::tuple_element<0, T>::type>(subs[0])
						, from_dvec<std::tuple_element<1, T>::type>(subs[1]));
				else if constexpr (ts == 3)
					return std::make_tuple(from_dvec<std::tuple_element<0, T>::type>(subs[0])
						, from_dvec<std::tuple_element<1, T>::type>(subs[1])
						, from_dvec<std::tuple_element<2, T>::type>(subs[2]));
				else if constexpr (ts == 4)
					return std::make_tuple(from_dvec<std::tuple_element<0, T>::type>(subs[0])
						, from_dvec<std::tuple_element<1, T>::type>(subs[1])
						, from_dvec<std::tuple_element<2, T>::type>(subs[2])
						, from_dvec<std::tuple_element<3, T>::type>(subs[3]));
				else if constexpr (ts == 5)
					return std::make_tuple(from_dvec<std::tuple_element<0, T>::type>(subs[0])
						, from_dvec<std::tuple_element<1, T>::type>(subs[1])
						, from_dvec<std::tuple_element<2, T>::type>(subs[2])
						, from_dvec<std::tuple_element<3, T>::type>(subs[3])
						, from_dvec<std::tuple_element<4, T>::type>(subs[4]));
				else
					return {};
			}

			template<typename T>
			Dvec to_dvec_orderer_container(T& target) {
				Dvec result;
				std::list<Dvec> subs;
				size_t size = 0;
				for (auto iter = target.begin(); iter != target.end(); ++iter) {
					auto sub = to_dvec(*iter);
					size += sub.size();
					subs.push_back(sub);
				}
				return merge_dvec(subs, size);
			}
			template<typename T>
			T from_dvec_orderer_container(Dvec dvec) { 
				T target{};
				auto ds = split_dvec(dvec);
				if (ds.empty()) return target;
				if constexpr (std::is_array<T>::value)
					if (ds.size() > target.size()) return target;
				for (size_t i = 0; i < ds.size(); i++) {
					if constexpr (is_specialization <T, std::vector>::value
						|| is_specialization <T, std::list>::value) {
						target.push_back(from_dvec<T::value_type>(ds[i]));
					}
					else if constexpr (is_specialization <T, std::map>::value
						|| is_specialization <T, std::set>::value) {
						target.insert(from_dvec<T::value_type>(ds[i]));
					}
					else if constexpr (is_specialization <T, std::array>::value){
						target[i] = from_dvec<T::value_type>(ds[i]);
					}
				}
				return target; 
			}

			template<typename T, typename ... Args>
			void _to_dvec_pack(std::vector<Dvec>& ds, T& arg, Args&... args) {
				ds.push_back(to_dvec<T>(arg));
				if constexpr (sizeof ... (args) == 0)
					return;
				else
					_to_dvec_pack(ds, args...);
			}
			template<typename ... Args>
			Dvec to_dvec_pack(Args&... args) {
				std::vector<Dvec> ds;
				if constexpr (sizeof ... (args) == 0)
					return ds;
				_to_dvec_pack(ds, args...);
				return merge_dvec(ds);
			}
			template<typename T, typename ... Args>
			void _from_dvec_pack (std::vector<Dvec>& ds, int& i, T& arg, Args&... args) {
				arg = from_dvec<T>(ds[i]);
				i++;
				if (i == ds.size()) return;
				if constexpr (sizeof ... (args) == 0)
					return;
				else
					_from_dvec_pack(ds, i, args...);
			}
			template<typename ... Args>
			bool from_dvec_pack(Dvec dvec, Args&... args) {
				if constexpr (sizeof ... (args) == 0)
					return true;
				auto ds = split_dvec(dvec);
				int i = 0;
				_from_dvec_pack(ds, i, args...);
				return false;
			}
		}
	}
}
#endif // !DVEC_H
