#pragma once
#ifndef _MAKE_SWS_
#define _MAKE_SWS_(name, ...) \
		inline auto& XM_CALLCONV name () { return this->swizzle<__VA_ARGS__ >(); } \
		inline const auto& XM_CALLCONV name() const { return this->swizzle<__VA_ARGS__ >(); }
#endif