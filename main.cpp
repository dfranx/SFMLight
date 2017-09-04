#include <SFML/Graphics.hpp>
#include <LightScene.h>
#include <Light.h>
#include <iostream>


using namespace std;

int main() {
	sf::RenderWindow wnd(sf::VideoMode(800, 600), "Intersection");
	sf::Event event;

	sf::CircleShape cs;
	cs.setPosition(400, 300);
	cs.setRadius(50);

	sf::RectangleShape rs;
	rs.setPosition(600, 500);
	rs.setSize(sf::Vector2f(100, 100));

	sfl::LightScene scene;
	scene.SetDebugDraw(true);
	scene.SetObjectDraw(true);
	scene.Add(cs);
	scene.Add(rs);

	sfl::Light light;
	light.SetOuterColor(sf::Color(255, 255, 255, 0));
	light.SetDetailLevel(sfl::LightDetail::Medium);
	light.SetRadius(300);
	light.SetRayCount(24);
	light.SetDebugDraw(true);

	while (wnd.isOpen()) {
		while (wnd.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				wnd.close();
			else if (event.type == sf::Event::Resized)
				wnd.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
		}

		light.SetPosition(sf::Vector2f(sf::Mouse::getPosition(wnd)));
		scene.Update(light);

		wnd.clear();
		
		light.Render(wnd);
		scene.Render(wnd);

		wnd.display();
	}
	return 0;
}