#pragma once

namespace Ull
{
	//                                 V -- Its the type of Features, a template that takes one type
	template<typename T, typename Tag, template<typename> typename... Features>
	class StrongType : private Features<StrongType<T, Tag, Features...>> ...
	{
	public:
		explicit StrongType(T const& value) : m_value{ value } {}
		explicit StrongType(T const&& value) : m_value{ std::move(value) } {}
		T& Get() { return m_value; }
		T const& Get() const noexcept { return m_value; }

	private:
		T m_value;
	};

	template<typename T>
	class StrongTypeEq
	{
		friend bool operator==(const T& lhs, const T& rhs) noexcept
		{
			return lhs.Get() == rhs.Get();
		}

		friend bool operator!=(const T& lhs, const T& rhs) noexcept
		{
			return !(lhs == rhs);
		}
	};
}