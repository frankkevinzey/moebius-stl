#pragma once

#include "types.hpp"

namespace mstl
{

	/// <summary>
	/// Scopes an objects ownership. Ownership is not transferable by assignment but explicit function calls.
	/// </summary>
	/// <typeparam name="T">Type of object to scope ownership</typeparam>
	template<typename T>
	class Scope final
	{
	public:
		Scope(void) = default;
		inline Scope(std::nullptr_t n) : _ptr(nullptr) {}
		inline Scope(T instance) { _ptr = new T(std::move(instance)); }
		inline ~Scope(void) { Release(); }

		Scope& operator=(const Scope<T>& other) = delete;

		inline Scope& operator=(std::nullptr_t)
		{
			Release();

			return *this;
		}

		inline Scope& operator=(Scope<T>&& other)
		{
			Release();

			_ptr = other._ptr;
			other._ptr = nullptr;

			return *this;
		}

		template<typename T2>
		inline T2* As(void) { return (T2*)_ptr; }

		inline operator bool(void)       { return _ptr != nullptr; }
		inline operator bool(void) const { return _ptr != nullptr; }

		inline T* operator->(void) { return _ptr; }
		inline const T* operator->(void) const { return _ptr; }

		inline T* const Raw(void) { return _ptr; }
		inline const T* const Raw(void) const { return _ptr; }

		inline void Reset(T* instance = nullptr)
		{
			Release();

			_ptr = instance;
		}

		inline void Swap(Scope<T>& other)
		{
			auto otherPtr = other._ptr;

			other._ptr = _ptr;
			_ptr = otherPtr;
		}

		template<typename... Args>
		static inline Scope<T> Create(Args&&... args) { return Scope<T>(new T(std::forward<Args>(args)...)); }

		inline T* Transfer(void) { T* _tmp = _ptr; _ptr = nullptr; return _tmp; }

	private:
		inline void Release(void) { if (_ptr) delete _ptr; _ptr = nullptr; }

		T* _ptr = nullptr;

	};

}
