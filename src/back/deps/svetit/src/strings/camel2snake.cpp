#include <svetit/strings/camel2snake.hpp>

namespace svetit {

std::string Camel2Snake(const std::string_view& camel)
{
	std::string snake;
	snake.reserve(camel.size() * 1.2f);
	snake.resize(camel.size());
	std::size_t i = 0;
	bool isUnderlined = true;
	for (auto&& c : camel)
	{
		if (c >= 'A' && c <= 'Z')
		{
			if (!isUnderlined)
			{
				snake.resize(snake.size() + 1);
				snake[i++] = '_';
			}
			snake[i++] = std::tolower(c);
		}
		else
			snake[i++] = c;
		isUnderlined = c == '_';
	}
	return snake;
}

} // namespace svetit
