#pragma once
#ifndef UIO_BASE_MYDEF_H
#define UIO_BASE_MYDEF_H

#define IS_VA_ARGS_MACRO(...) ( sizeof( (char[]){#__VA_ARGS__} ) == 1 )

#define CAT_WITH_COMMA(A,B) A##,##B

#define FUNC_MAKE_CONSTEXPR_ARR_RECURSIZE_TEMPLATE(FUNC_NAME, TEMPLATE_TYPENAME, TEMPLATE_VAR, SIZE, TVAL, VALUE)\
	template<TEMPLATE_TYPENAME TEMPLATE_VAR, size_t s = SIZE, int i = 0>\
	constexpr std::array<TVAL, s> FUNC_NAME() { \
		using arr_t = std::array<TVAL, s>; \
		if constexpr (i == s) { \
			return arr_t{}; \
		} \
		else { \
			arr_t narr = FUNC_NAME<TEMPLATE_VAR, s, i + 1>();\
			narr[i] = VALUE ;\
			return narr;\
		}\
	}

#define FUNC_MAKE_CONSTEXPR_ARR_RECURSIZE(FUNC_NAME, SIZE, TVAL, VALUE)\
	template<size_t s = SIZE, int i = 0>\
	constexpr std::array<TVAL, s> FUNC_NAME() { \
		using arr_t = std::array<TVAL, s>; \
		if constexpr (i == s) { \
			return arr_t{}; \
		} \
		else { \
			arr_t narr = FUNC_NAME<s, i + 1>();\
			narr[i] = VALUE ;\
			return narr;\
		}\
	}

// example : LET_CONCEPT_BE_CHECKABLE(is_intte, std::is_integral<T>)
#define LET_CONCEPT_BE_CHECKABLE(STRUCT_NAME, CONCEPT)\
	template<typename T> \
	struct STRUCT_NAME {\
		template<CONCEPT T>\
		constexpr static bool func() { return true; } \
		template<typename T> \
		constexpr static bool func() { return false; } \
		constexpr static bool value = func<T>(); };

// example : LET_CONCEPT_BE_CHECKABLE(is_intte, requires{ T::value; })
#define LET_CONCEPT_BE_CHECKABLE_R(STRUCT_NAME, REQUIRE) \
	template<typename T>\
	struct STRUCT_NAME { \
		template<typename T> requires REQUIRE \
		constexpr static bool func() { return true; } \
		template<typename T> \
		constexpr static bool func() { return false; } \
		constexpr static bool value = func<T>(); };

#endif // ! UIO_BASE_MYDEF_H
