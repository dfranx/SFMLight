#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;


#define LIGHT_RADIUS 400

enum LightDetail
{
	LIGHTDETAIL_LOW,
	LIGHTDETAIL_MEDIUM,
	LIGHTDETAIL_HIGH
};

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

	Line getLine(int i) {
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
	else if (crossProduct(r, s) != 0 && (T >= 0 && T <= 1) && (U >= 0 && U <= 1)) { // intersecting
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

	vector<Object> objs(2);
	objs[0].add(400, 300);
	objs[0].add(400, 315);
	objs[0].add(375, 335);
	objs[0].add(400, 350);
	objs[0].add(450, 350);
	objs[0].add(450, 300);
	objs[0].add(425, 275);

	objs[1].add(550, 500);
	objs[1].add(550, 700);
	objs[1].add(750, 700);
	objs[1].add(750, 500);

	sf::CircleShape inter;
	inter.setRadius(4);
	inter.setOrigin(4, 4);

	sf::Clock fpsClock;
	float fps;

	char precisionMode = LIGHTDETAIL_MEDIUM;

	while (wnd.isOpen()) {
		while (wnd.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				wnd.close();
			else if (event.type == sf::Event::Resized)
				wnd.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
			else if (event.type == sf::Event::KeyPressed)
				printf("FPS: %.3fs\n", fps);
		}
		
		fps = fpsClock.restart().asMilliseconds() / 1000.0f;

		wnd.clear();

		q.a = sf::Vector2f(sf::Mouse::getPosition(wnd));
		bool skipObject = false;
		//std::vector<sf::Vertex> varr;
		//varr.push_back(sf::Vertex(q.a, sf::Color::White));
		unordered_map<int, bool> highDefColObjs;
		for (float angle = 0; angle < 2 * M_PI; angle += 2 * M_PI / 32) {
			sf::Vector2f resPos = q.b = sf::Vector2f(q.a.x + cos(angle) * LIGHT_RADIUS, q.a.y + sin(angle) * LIGHT_RADIUS);

			skipObject = false;
			for (int j = 0; j < objs.size(); j++) {
				sf::Vector2f interPos;
				int cnt = objs[j].getLineCount();
				for (int i = 0; i < cnt; i++) {
					if (doIntersect(q, objs[j].getLine(i), interPos)) {
						if (length(q.a, resPos) > length(q.a, interPos)) {
							resPos = interPos;
							highDefColObjs[j] = true;
						}
						skipObject = true;
					}
				}
			}

			if (skipObject && precisionMode == LIGHTDETAIL_MEDIUM)
				continue;

			inter.setPosition(resPos);
			q.b = resPos;

			// RADIAL GRADIENT:
			//sf::Color resColor(255, 255, 255, 255 - length(q.a, q.b)*(255.0f / LIGHT_RADIUS));
			//varr.push_back(sf::Vertex(resPos, resColor));

			q.draw(wnd, sf::Color::Red);
			wnd.draw(inter);
		}

		if (precisionMode > LIGHTDETAIL_LOW) {
			for (int j = 0; j < objs.size(); j++) {
				if (!highDefColObjs[j])
					continue;

				Object* obj = &objs[j];
				for (int i = 0; i < obj->points.size(); i++) {
					sf::Vector2f point = obj->points[i];

					if (length(q.a, point) > LIGHT_RADIUS)
						continue;

					float newAngle = atan2(point.y - q.a.y, point.x - q.a.x);
					printf("%.4f\n", newAngle);
					// TODO: (newAngle - 0.01f) and (newAngle + 0.01f) cases
					q.b = sf::Vector2f(q.a.x + cos(newAngle) * LIGHT_RADIUS, q.a.y + sin(newAngle) * LIGHT_RADIUS);

					sf::Vector2f interPos, resPos = q.b;
					
					//for (int k = std::max(i - 1, 0); k < std::min(i + 1, (int)obj->points.size()); k++) {
					for (int k = 0; k < (int)obj->points.size(); k++) {
						if (doIntersect(q, obj->getLine(k), interPos)) {
							if (length(q.a, resPos) > length(q.a, interPos)) {
								resPos = interPos;
							}
						}
					}

					q.b = resPos;

					q.draw(wnd, sf::Color::Blue);
				}
			}
		}


		//varr.push_back(varr[1]);
		//wnd.draw(&varr[0], varr.size(), sf::TrianglesFan);

		for (int i = 0; i < objs.size(); i++)
			objs[i].draw(wnd);

		wnd.display();
	}
	return 0;
}