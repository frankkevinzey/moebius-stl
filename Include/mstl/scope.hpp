#pragma once

#include <memory>

template<typename T>
using Scope = std::unique_ptr<T>;

namespace Moebius
{

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) { return std::make_unique<T>(std::forward<Args>(args)...); }

}
