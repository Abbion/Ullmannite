#pragma once

namespace Ull
{
	template<typename T>
	class NotOwner
	{
	public:
		NotOwner(T* ptr) { m_ptr = ptr; }
		~NotOwner() {}

		void operator delete(void*) = delete;
		void* operator new(size_t size) = delete;

		inline T* operator->() const { return m_ptr; }
		inline T* Get() const { return m_ptr; }

	private:
		T* m_ptr;
	};
}