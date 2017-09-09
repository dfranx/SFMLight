#include <SFMLight.h>
#include <SFML/Graphics.hpp>

int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Example from README");
	sf::Event event;

	sfl::LightScene scene;
	scene.SetDebugDraw(true);

	sfl::Object obj;
	obj.Add(50, 50);
	obj.Add(100, 50);
	obj.Add(100, 100);
	obj.Add(50, 100);

	scene.Add(obj);

	sfl::Light light;
	light.SetRadius(300);
	light.SetPosition(sf::Vector2f(150, 150));

	scene.Update(light);

	while (window.isOpen()) {

		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();


		window.clear();

		light.Render(window);
		scene.Render(window);

		window.display();
	}


	return 0;
}