#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;


#define M_PI 3.14f


struct Line
{
	sf::Vector2f a, b;

	inline void create(float x1, float y1, float x2, float y2) {
		a = { x1, y1 };
		b = { x2, y2 };
	}

	inline void create(sf::Vector2f A, sf::Vector2f B) {
		a = A;
		b = B;
	}

	void draw(sf::RenderTarget& tgt, sf::Color clr) {
		vector<sf::Vertex> arr = {
			sf::Vertex(a, clr),
			sf::Vertex(b, clr)
		};
		tgt.draw(&arr[0], arr.size(), sf::Lines);
	}
};

struct Object
{
	std::vector<sf::Vector2f> points;

	inline void add(float x, float y) {
		points.push_back(sf::Vector2f(x, y));
	}
	
	inline int getLineCount() { return points.size(); }

	inline Line getLine(int i) {
		Line ret;
		ret.create(points[i], points[(i == points.size() - 1) ? 0 : (i + 1)]);
		return ret;
	}

	void draw(sf::RenderTarget& tgt) {
		std::vector<sf::Vertex> varr(points.size());

		for (int i = 0; i < varr.size(); i++) {
			varr[i].position = points[i];
			varr[i].color = sf::Color::Green;
		}

		varr.push_back(sf::Vertex(points[0], sf::Color::Green));

		tgt.draw(&varr[0], varr.size(), sf::LineStrip);
	}
};

inline float length(sf::Vector2f p1, sf::Vector2f p2)
{
	return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

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

	Line q;
	q.create(150, 50, 250, 100);

	Object obj;
	obj.add(400, 300);
	obj.add(400, 315);
	obj.add(375, 335);
	obj.add(400, 350);
	obj.add(450, 350);
	obj.add(450, 300);
	obj.add(425, 275);

	sf::CircleShape inter;
	inter.setRadius(4);
	inter.setOrigin(4, 4);

	while (wnd.isOpen()) {
		while (wnd.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				wnd.close();
			else if (event.type == sf::Event::Resized)
				wnd.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
		}

		wnd.clear();

		q.a = sf::Vector2f(sf::Mouse::getPosition(wnd));
		//std::vector<sf::Vertex> varr;
		//varr.push_back(sf::Vertex(q.a, sf::Color::White));
		for (float angle = 0; angle < 2 * M_PI; angle += 2 * M_PI / 32) {
			q.b = sf::Vector2f(q.a.x + cos(angle)*300, q.a.y + sin(angle)*300);

			sf::Vector2f interPos, resPos = q.b;
			int cnt = obj.getLineCount();
			for (int i = 0; i < cnt; i++)
				if (doIntersect(q, obj.getLine(i), interPos))
					if (length(q.a, resPos) > length(q.a, interPos))
						resPos = interPos;
			inter.setPosition(resPos);
			q.b = resPos;
			
			//varr.push_back(resPos);

			q.draw(wnd, sf::Color::Red);
			wnd.draw(inter);
		}
		//wnd.draw(&varr[0], varr.size(), sf::TrianglesFan);

		obj.draw(wnd);
		wnd.display();
	}
	return 0;
}