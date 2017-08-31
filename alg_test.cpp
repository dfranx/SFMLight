#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

struct Line
{
	sf::Vector2f a, b;

	inline void create(float x1, float y1, float x2, float y2) {
		a = { x1, y1 };
		b = { x2, y2 };
	}

	void draw(sf::RenderTarget& tgt, sf::Color clr) {
		vector<sf::Vertex> arr = {
			sf::Vertex(a, clr),
			sf::Vertex(b, clr)
		};
		tgt.draw(&arr[0], arr.size(), sf::Lines);
	}
};

inline float crossProduct(sf::Vector2f v, sf::Vector2f w)
{
	return v.x*w.y - v.y*w.x;
}

bool doIntersect(Line l1, Line l2, sf::Vector2f& ret)
{
	sf::Vector2f p = l1.a;
	sf::Vector2f q = l2.a;

	sf::Vector2f r = l1.b - p;
	sf::Vector2f s = l2.b - q;

	//p + T * r = q + U * s
	float T = crossProduct((q - p), s) / crossProduct(r, s);
	float U = crossProduct((q - p), r) / crossProduct(r, s);

	// collinear
	if (crossProduct(r, s) == 0 && crossProduct((q - p), r) == 0)
		return false; // i guess i can ignore (for now) if two lines are collinear
	else if (crossProduct(r, s) != 0 && (T >= 0 && T <= 1) && (U >= 0 && U <= 1)) { // parallel - not intersecting
		ret = q + U*s;
		return true; // pointOfIntersection = q + U*s;
	}

	return false;
}

int main() {
	sf::RenderWindow wnd(sf::VideoMode(800, 600), "Intersection");
	sf::Event event;

	Line p, q;
	p.create(50, 100, 250, 140);
	q.create(150, 50, 250, 100);

	sf::CircleShape inter;
	inter.setRadius(4);
	inter.setOrigin(4, 4);

	sf::Vector2f interPos;
	if (doIntersect(p, q, interPos))
		inter.setPosition(interPos);

	while (wnd.isOpen()) {
		while (wnd.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				wnd.close();
		}

		q.b = sf::Vector2f(sf::Mouse::getPosition(wnd));

		if (doIntersect(p, q, interPos))
			inter.setPosition(interPos);
		else
			inter.setPosition(q.b);
		
		wnd.clear();
		p.draw(wnd, sf::Color::Red);
		q.draw(wnd, sf::Color::Green);
		wnd.draw(inter);
		wnd.display();
	}
	return 0;
}