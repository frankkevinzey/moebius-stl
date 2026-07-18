#pragma once

#include "types.hpp"

namespace mstl
{

	template<typename T>
	class Scope;

	template<typename T>
	class ScopeRef final
	{
	public:
		inline ScopeRef(void) : _obj(nullptr) {}
		inline ScopeRef(T* _ptr) : _obj(_ptr) {}
		inline ScopeRef(std::nullptr_t) : _obj(nullptr) {}
		inline ScopeRef(ScopeRef<T>&& other) : _obj(other._obj) { other._obj = nullptr; }
		inline ScopeRef(const ScopeRef<T>& other) : _obj(other._obj) {}
		~ScopeRef(void) = default;

		ScopeRef(const Scope<T>& other);
		ScopeRef<T>& operator=(const Scope<T>& other);

		inline ScopeRef<T>& operator=(ScopeRef<T>&& other)
		{
			_obj = other._obj;
			other._obj = nullptr;

			return *this;
		}

		inline ScopeRef<T>& operator=(const ScopeRef<T>& other)
		{
			_obj = other._obj;

			return *this;
		}

		template<typename T2>
		inline       T2& As(void) const { return *((T2*)_obj); }
		template<typename T2>
		inline const T2& As(void) const { return *((T2*)_obj); }

		inline operator bool(void)       { return _obj != nullptr; }
		inline operator bool(void) const { return _obj != nullptr; }

		inline       T* operator ->(void)       { return _obj; }
		inline const T* operator ->(void) const { return _obj; }

		inline       T& operator *(void)       { return *_obj; }
		inline const T& operator *(void) const { return *_obj; }

	private:
		T* _obj;

	};

	template<typename T>
	class Scope final
	{
	public:
		inline Scope(void) : _ptr(nullptr) {}
		inline Scope(std::nullptr_t) : _ptr(nullptr) {}
		inline Scope(T* instance) { _ptr = instance; }
		inline ~Scope(void) { Release(); }

		template<typename D>
			requires std::derived_from<D, T>
		inline Scope(Scope<D>&& other) : _ptr((T*)other.Transfer()) {}

		Scope<T>& operator=(const Scope<T>& other) = delete;

		inline Scope<T>& operator=(std::nullptr_t)
		{
			Release();

			return *this;
		}

		inline Scope<T>& operator=(Scope<T>&& other)
		{
			Release();

			_ptr = other._ptr;
			other._ptr = nullptr;

			return *this;
		}

		template<typename T2>
		inline ScopeRef<T2> As(void) { return (T2*)_ptr; }
		template<typename T2>
		inline ScopeRef<const T2> As(void) const { return (T2*)_ptr; }

		inline operator bool(void)       { return _ptr != nullptr; }
		inline operator bool(void) const { return _ptr != nullptr; }

		inline T* operator ->(void)             { return _ptr; }
		inline const T* operator ->(void) const { return _ptr; }

		inline T* const Raw(void)             { return _ptr; }
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

		/// <summary>
		/// Transfers ownership out of this Scope.
		/// </summary>
		inline T* Transfer(void) { T* _tmp = _ptr; _ptr = nullptr; return _tmp; }

		template<typename... Args>
		static inline Scope<T> Create(Args&&... args) { return Scope<T>(new T(std::forward<Args>(args)...)); }

	private:
		inline void Release(void) { if (_ptr) delete _ptr; _ptr = nullptr; }

		T* _ptr = nullptr;

		friend class ScopeRef<T>;

	};

	template<typename T>
	ScopeRef<T>::ScopeRef(const Scope<T>& other) : _obj(other._ptr) {}

	template<typename T>
	ScopeRef<T>& ScopeRef<T>::operator=(const Scope<T>& other)
	{
		_obj = other._ptr;

		return *this;
	}

}
