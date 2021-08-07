#pragma once
#ifndef MY_DEF_H
#define MY_DEF_H
#include <functional>

#define ZZ_TRT(FF) {if(FF){return true;}}
#define ZZ_TRTFUN(FF, FUNC) {if(FF){ FUNC##; return true;}}
#define ZZ_TRF(FF) {if(FF){return false;}}
#define ZZ_TRFFUN(FF, FUNC) {if(FF){ FUNC##; return false;}}
#define ZZ_TRN(FF) {if(FF){return nullptr;}}
#define ZZ_TRNFUN(FF, FUNC) {if(FF){ FUNC##; return nullptr;}}
#define ZZ_TRON(FF) {if(FF){return std::nullopt;}}
#define ZZ_TRONFUN(FF, FUNC) {if(FF){ FUNC##; return std::nullopt;}}

struct defer {
    std::function<void()> fn;
    ~defer() { fn(); }
};

#define DEFER_MERGE_(a,b)  a##b
#define DEFER_LABEL_(a)    DEFER_MERGE_(unique_name_, a)
#define DEFER_UNIQUE_NAME  DEFER_LABEL_(__LINE__)
#define defer        defer DEFER_UNIQUE_NAME; DEFER_UNIQUE_NAME.fn = [&]

#endif // ! MY_DEF_H