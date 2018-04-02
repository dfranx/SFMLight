#ifndef __SFMLIGHT_LINE_H__
#define __SFMLIGHT_LINE_H__

#include <SFML/System/Vector2.hpp>
#include <sfl/Config.h>

namespace sfl
{
	class SFMLIGHT_EXPORT Line
	{
	public:
		sf::Vector2f Start, End; // start and end position of a line

		inline void Create(float x1, float y1, float x2, float y2) {
			Start = { x1, y1 };
			End = { x2, y2 };
		}

		inline void Create(const sf::Vector2f& a, const sf::Vector2f& b) {
			Start = a;
			End = b;
		}
		
		/////////////////////////////////////////////////////////
		// check if thise line intersects another
		// line
		// return true => collision occured
		// sf::Vector2f& ret => where did the collision occur
		/////////////////////////////////////////////////////////
		bool Intersects(const Line& ln, sf::Vector2f& ret);
	};
}

#endif //__SFMLIGHT_LINE_H__