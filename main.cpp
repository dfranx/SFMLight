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
	scene.Add(cs);
	scene.Add(rs);

	sfl::Light light;
	light.SetDetailLevel(sfl::LightDetail::Medium);
	light.SetRadius(300);
	light.SetRayCount(24);
	light.SetPosition(sf::Vector2f(550, 450));
	scene.Update(light);

	while (wnd.isOpen()) {
		while (wnd.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				wnd.close();
			else if (event.type == sf::Event::Resized)
				wnd.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
		}


		wnd.clear();
		
		wnd.draw(light);
		wnd.draw(scene);

		wnd.display();
	}
	return 0;
}