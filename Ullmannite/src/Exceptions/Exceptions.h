#pragma once
#include <exception>

namespace Ull
{
	class InitializationException : public std::exception
	{
	public:
		InitializationException(const char* str) : m_message(str) {}

		const char* what() const noexcept override {
			return m_message;
		}

	private:
		const char* m_message;
	};

	class NegativeDimensionsException : public std::exception
	{
	public:
		NegativeDimensionsException(const char* str) : m_message(str) {}

		const char* what() const noexcept override {
			return m_message;
		}

	private:
		const char* m_message;
	};
}