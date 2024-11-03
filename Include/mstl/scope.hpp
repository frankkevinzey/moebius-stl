#pragma once

#include "types.hpp"

namespace mstl
{

	template<typename T>
	class Scope final
	{
	public:

	public:
		Scope(void) = default;
		inline Scope(std::nullptr_t n) : _ptr(nullptr) {}
		inline Scope(T* instance) : _ptr(instance) {}
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

		template<typename T>
		inline T& operator *(void) { return *_ptr; }

		inline operator bool(void) { return _ptr != nullptr; }
		inline operator bool(void) const { return _ptr != nullptr; }

		inline T* operator->(void) { return _ptr; }
		inline const T* operator->(void) const { return _ptr; }

		inline T& operator*(void) { return *_ptr; }
		inline const T& operator*(void) const { return *_ptr; }

		inline T* Raw(void) { return _ptr; }
		inline const T* Raw(void) const { return _ptr; }

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

	private:
		inline void Release(void) { if (_ptr) delete _ptr; _ptr = nullptr; }

		T* _ptr = nullptr;

	};

}
