#pragma once
#ifndef UIO_BASE_TID_H
#define UIO_BASE_TID_H

#include <type_traits>
#include <any>
#include <array>

#include "symbol.h"
#include "external/Dvec.hpp"

// example : UIO_SET_SYMBOL_TID(int, 4)
#define UIO_SET_SYMBOL_TID(T, KIND_, TID_)\
template<>\
struct TID<KIND_, T>{static constexpr int value = TID_ ; };\
template<>\
struct TIDT<KIND_, TID_>{\
using type = T;\
using tid_t = TID<KIND_, T>;\
using attr = SymbolAttr<T>;};

#define UIO_CORE_MUST_DO_INIT_TID  uio::detail::collect_TID_serialize_funcs();

namespace uio {

	enum class TIDkind : size_t {
		part, happen, _max
	};

	template<TIDkind kind, typename T>	// ex. TID<TIDtype::part, Weapon>::value == 4;
	struct TID {						// the map of type & int
		static constexpr int value = 0 ;
	};

	template<TIDkind kind, int tid> // ex. using card = TIDT<TIDtype::part, 4>::type;
	struct TIDT {					// , bool is_id = TIDT<4>::is(...);
		using type = void;
		using tid_t = TID<kind, void>;
		using attr = SymbolAttr<void>;
	};

	serialize::Dvec tid_to_dvec(TIDkind kind, int tid, std::any a) {
		if ((size_t)kind == (size_t)TIDkind::_max || tid <= 0 || tid >= detail::TIDmax)
			return serialize::Dvec{};
		return (*detail::TIDto_dvecs[(size_t)kind][tid])(a);
	}
	std::any tid_from_dvec(TIDkind kind, int tid, serialize::Dvec a) {
		if ((size_t)kind == (size_t)TIDkind::_max || tid <= 0 || tid >= detail::TIDmax)
			return std::any();
		return (*detail::TIDfrom_dvecs[(size_t)kind][tid])(a);
	}

	namespace detail {
		using Dvec = serialize::Dvec;

		typedef Dvec(*TID_to_dev_func_t)(std::any);
		typedef std::any(*TID_from_dev_func_t)(Dvec);

		constexpr size_t TIDmax = 50;
		std::array<TID_to_dev_func_t, TIDmax> TIDto_dvecs[(size_t)TIDkind::_max];
		std::array<TID_from_dev_func_t, TIDmax> TIDfrom_dvecs[(size_t)TIDkind::_max];

		template<typename T>
		concept is_type_tid_c = !std::is_same_v<T, void> && requires{
			((TID<TIDkind::part, T>::value > 0) && (TID<TIDkind::part, T>::value < TIDmax))
				|| ((TID<TIDkind::happen, T>::value > 0) && (TID<TIDkind::happen, T>::value < TIDmax));
		};
		LET_CONCEPT_BE_CHECKABLE(is_type_tid, is_type_tid_c);

		// below must defined after all tid defined, ex. at core.h
		template<TIDkind kind, int index>
		constexpr TID_to_dev_func_t _collect_TID_serialize_funcs_HIDE_to_dvec() {
			using type = TIDT<kind, index>::type;
			if constexpr (!is_type_tid<type>::value)
				return nullptr;
			else
				return [&](std::any a) -> Dvec {
				type t = std::any_cast<type>(a);
				return a.has_value() ? to_dvec<type>(t) : Dvec{};
			};
		}
		template<TIDkind kind, int index>
		constexpr TID_from_dev_func_t _collect_TID_serialize_funcs_HIDE_from_dvec() {
			using type = TIDT<kind, index>::type;
			if constexpr (!is_type_tid<type>::value)
				return nullptr;
			else
				return [&](Dvec d) -> std::any {
				return std::make_any<type>(from_dvec<type>(d));
			};
		}

		FUNC_MAKE_CONSTEXPR_ARR_RECURSIZE_TEMPLATE(_TID_to_dvec_arr_HIDE_make, TIDkind, kind
			, TIDmax, TID_to_dev_func_t
			, _collect_TID_serialize_funcs_HIDE_to_dvec<CAT_WITH_COMMA(kind, i)>());
		FUNC_MAKE_CONSTEXPR_ARR_RECURSIZE_TEMPLATE(_TID_from_dvec_arr_HIDE_make, TIDkind, kind
			, TIDmax, TID_from_dev_func_t
			, _collect_TID_serialize_funcs_HIDE_from_dvec<CAT_WITH_COMMA(kind, i)>());

		void collect_TID_serialize_funcs() {
			TIDto_dvecs[(size_t)TIDkind::part] = _TID_to_dvec_arr_HIDE_make<TIDkind::part>();
			TIDto_dvecs[(size_t)TIDkind::happen] = _TID_to_dvec_arr_HIDE_make<TIDkind::happen>();
			TIDfrom_dvecs[(size_t)TIDkind::part] = _TID_from_dvec_arr_HIDE_make<TIDkind::part>();
			TIDfrom_dvecs[(size_t)TIDkind::happen] = _TID_from_dvec_arr_HIDE_make<TIDkind::happen>();
		}
	}
}

#endif // ! UIO_BASE_TID_H
