#pragma once
#ifndef UIO_BASE_SYMBOL_H
#define UIO_BASE_SYMBOL_H

#include "mydef.h"

// example : UIO_SET_SYMBOL_ATTRS(int, tid, single, compound)
#define UIO_SET_SYMBOL_ATTRS(T, ...)\
template<>\
struct uio::detail::_HIDE_SymbolAttrs<T> { bool _place_holder, ##__VA_ARGS__ ; };

namespace uio {

	enum class SymbolTypes : size_t {
		tid, // can be get by tid
		id, // has id inside struct
		part, // can and only can be part of card
		obj, // has state, pos, resolution
		def, // has state
		single, // not has state
		place, // place type
		region, // region type
		happen, // event type
		status, // visual of state
		condition, // upper of condition
		compound, // many symbol's gather
		_max
	};

	namespace detail {
		template<typename T>
		struct _HIDE_SymbolAttrs {};
	}
	template<typename T>
	struct SymbolAttr {
		//using enum SymbolTypes;
		constexpr static bool is(SymbolTypes type) {
			switch (type) {
			case SymbolTypes::tid: return is_tid<T>::value;
			case SymbolTypes::id: return is_id<T>::value;
			case SymbolTypes::part: return is_part<T>::value;
			case SymbolTypes::exists: return is_exist<T>::value;
			case SymbolTypes::obj: return is_obj<T>::value;
			case SymbolTypes::compound: return is_compound<T>::value;
			}
			return false;
		}
	private:
		LET_CONCEPT_BE_CHECKABLE_R(is_tid, requires {detail::_HIDE_SymbolAttrs<T>::tid; })
		LET_CONCEPT_BE_CHECKABLE_R(is_id, requires {T::id; })
		LET_CONCEPT_BE_CHECKABLE_R(is_part, requires {detail::_HIDE_SymbolAttrs<T>::part; })
		LET_CONCEPT_BE_CHECKABLE_R(is_exist, requires {detail::_HIDE_SymbolAttrs<T>::exist; })
		LET_CONCEPT_BE_CHECKABLE_R(is_obj, requires {detail::_HIDE_SymbolAttrs<T>::obj; })
		LET_CONCEPT_BE_CHECKABLE_R(is_compound, requires {detail::_HIDE_SymbolAttrs<T>::compound; })
	};
}
#endif // ! UIO_BASE_SYMBOL_H
