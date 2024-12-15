#pragma once

#include "types.hpp"

namespace mstl
{

	class RefCounted
	{
	public:
		virtual ~RefCounted(void) = default;

		inline u32 CountedRefs(void) const { return _RefCount; }

	protected:
		RefCounted(void) = default;

	private:
		inline void IncRefCount(void) const { ++_RefCount; }
		inline void DecRefCount(void) const { --_RefCount; }

		inline u32 RefCount(void) const { return _RefCount; }

		mutable u32 _RefCount = 0; // TODO (zeyo): use atomic for thread safety???

		template<typename T>
		friend class Ref;

	};

	template<typename T>
	class Ref final
	{
		static_assert(std::is_base_of<RefCounted, T>::value, "Class is not RefCounted, use Scope");

	public:
		Ref(void) = default;
		inline Ref(std::nullptr_t n) : _ptr(nullptr) {}
		inline Ref(T* instance) : _ptr(instance) { IncRef(); }
		template<typename T2>
		inline Ref(const Ref<T2>& other) : _ptr((T*)other._ptr) { IncRef(); }
		template<typename T2>
		inline Ref(Ref<T2>&& other) : _ptr((T*)other._ptr) { other._ptr = nullptr; }
		inline Ref(const Ref<T>& other) : _ptr(other._ptr) { IncRef(); }

		inline ~Ref(void) { DecRef(); }

		template<typename T2>
		inline Ref<T2> As(void) { return Ref<T2>(*this); }
		template<typename T2>
		inline const Ref<T2> As(void) const { return Ref<T2>(*this); }

		inline Ref& operator=(std::nullptr_t)
		{
			DecRef();

			_ptr = nullptr;

			return *this;
		}
		inline Ref& operator=(Ref<T>& other)
		{
			other.IncRef();
			DecRef();

			_ptr = other._ptr;

			return *this;
		}
		inline Ref& operator=(const Ref<T>& other)
		{
			other.IncRef();
			DecRef();

			_ptr = other._ptr;

			return *this;
		}

		template<typename T>
		inline T& operator *(void) { return *_ptr; }

		template<typename T2>
		inline Ref& operator=(const Ref<T2>& other)
		{
			other.IncRef();
			DecRef();

			_ptr = other._ptr;

			return *this;
		}

#if 1
		template<typename T2>
		inline Ref& operator=(Ref<T2>&& other)
		{
			DecRef();

			_ptr = other._ptr;
			other._ptr = nullptr;

			return *this;
		}
#endif

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
			DecRef();

			_ptr = instance;
		}
		inline void Swap(Ref<T>& other)
		{
			auto otherPtr = other._ptr;

			other._ptr = _ptr;
			_ptr = otherPtr;
		}

		template<typename... Args>
		static Ref<T> Create(Args&&... args) { return Ref<T>(new T(std::forward<Args>(args)...)); }

	private:
		inline void IncRef(void) const
		{
			if (_ptr)
				_ptr->IncRefCount();
		}
		inline void DecRef(void) const
		{
			if (!_ptr)
				return;

			_ptr->DecRefCount();

			if (_ptr->RefCount() == 0)
				delete _ptr;
		}

		T* _ptr = nullptr;

		template<class T2> friend class Ref;

	};

}
