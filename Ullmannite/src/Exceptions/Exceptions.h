#include <exception>

namespace Ull
{
	class InitializationException : public std::exception
	{
	public:
		InitializationException(const char* str) : m_message(str) {}

		const char* what() {
			return m_message;
		}

	private:
		const char* m_message;
	};
}