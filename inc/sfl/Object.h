#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <sfl/Line.h>
#include <vector>

namespace sfl
{
	class Object
	{
	public:
		std::vector<sf::Vector2f> Points;

		inline void Add(float x, float y) {
			Points.push_back(sf::Vector2f(x, y));
		}

		inline size_t GetLineCount() {
			return Points.size();
		}

		Line GetLine(int index);
	};
}

#endif //__OBJECT_H__