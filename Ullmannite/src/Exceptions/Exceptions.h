#include <exception>

namespace Ull
{
	class InitializationException : public std::exception
	{
	public:
		InitializationException(const char* str) : m_message(str) {}

		const char* what() const override noexcept {
			return m_message;
		}

	private:
		const char* m_message;
	};
}