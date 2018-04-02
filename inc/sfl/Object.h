#ifndef __SFMLIGHT_OBJECT_H__
#define __SFMLIGHT_OBJECT_H__

#include <sfl/Config.h>
#include <sfl/Line.h>
#include <vector>

namespace sfl
{
	class SFMLIGHT_EXPORT Object
	{
	public:
		std::vector<sf::Vector2f> Points;	// list of points that represent this object

		inline void Add(sf::Vector2f vec) {
			Points.push_back(vec);
		}

		inline void Add(float x, float y) {
			Points.push_back(sf::Vector2f(x, y));
		}

		inline size_t GetLineCount() {
			return Points.size();
		}

		/////////////////////////////////////////////////////////
		// get line at given index
		/////////////////////////////////////////////////////////
		Line GetLine(int index);
	};
}

#endif //__SFMLIGHT_OBJECT_H__