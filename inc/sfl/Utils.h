#ifndef __SFMLIGHT_UTILS_H__
#define __SFMLIGHT_UTILS_H__

#include <SFML/System/Vector2.hpp>
#include <math.h>

namespace sfl
{
	namespace Utils
	{
		inline float Length(sf::Vector2f p1, sf::Vector2f p2) {
			return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
		}

		inline float CrossProduct(sf::Vector2f v, sf::Vector2f w) {
			return v.x*w.y - v.y*w.x;
		}
	}
}

#endif // __SFMLIGHT_UTILS_H__