#include <sfl/Line.h>
#include <sfl/Utils.h>

namespace sfl
{
	bool Line::Intersects(const Line & ln, sf::Vector2f & ret)
	{
		sf::Vector2f p = this->Start;
		sf::Vector2f q = ln.Start;

		sf::Vector2f r = this->End - p;
		sf::Vector2f s = ln.End - q;

		float T = Utils::CrossProduct(q - p, s) / Utils::CrossProduct(r, s);
		float U = Utils::CrossProduct(q - p, r) / Utils::CrossProduct(r, s);

		if (Utils::CrossProduct(r, s) != 0 && (T >= 0 && T <= 1) && (U >= 0 && U <= 1)) { // intersecting
			ret = q + U*s;
			return true;
		}

		return false;
	}
}