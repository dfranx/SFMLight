#ifndef __LINE_H__
#define __LINE_H__

#include <SFML/System/Vector2.hpp>

namespace sfl
{
	class Line
	{
	public:
		sf::Vector2f Start, End;

		inline void Create(float x1, float y1, float x2, float y2) {
			Start = { x1, y1 };
			End = { x2, y2 };
		}

		inline void Create(const sf::Vector2f& a, const sf::Vector2f& b) {
			Start = a;
			End = b;
		}

		bool Intersects(const Line& ln, sf::Vector2f& ret);
	};
}

#endif //__LINE_H__