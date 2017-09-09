#include <SFML/Graphics.hpp>
#include <LightScene.h>
#include <Light.h>
#include <iostream>

using namespace std;

int main() {
	sf::RenderWindow wnd(sf::VideoMode(1200, 800), "Light example", sf::Style::Titlebar | sf::Style::Close);
	sf::Event event;

	sfl::LightScene scene;
	
	sfl::Light light;
	light.SetRadius(300);
	light.SetDetailLevel(sfl::LightDetail::High);
	light.SetOuterColor(sf::Color(255, 255, 255, 0));

#pragma region TRUCK
	sf::Texture truckTex;
	truckTex.loadFromFile("examples/data/truck.png");

	sf::Sprite truckSpr;
	truckSpr.setTexture(truckTex);
	truckSpr.setOrigin(truckTex.getSize().x / 2, truckTex.getSize().y / 2);
	truckSpr.setRotation(45);
	truckSpr.setPosition(300, 300);
	truckSpr.setScale(2, 2);

	sfl::Object truck;
	truck.Points = {
		{ 207, 474 }, { 289, 393 }, { 289, 382 }, { 271, 363 },
		{ 291, 342 }, { 304, 358 }, { 322, 340 }, { 328, 348 },
		{ 340, 347 }, { 350, 336 }, { 366, 320 }, { 386, 300 },
		{ 398, 286 }, { 406, 278 }, { 411, 273 }, { 416, 266 },
		{ 425, 282 }, { 428, 278 }, { 426, 259 }, { 478, 206 },
		{ 480, 188 }, { 472, 176 }, { 393, 100 }, { 382, 95  },
		{ 371, 95  }, { 366, 103 }, { 366, 110 }, { 360, 114 },
		{ 354, 121 }, { 318, 152 }, { 299, 146 }, { 297, 154 },
		{ 307, 164 }, { 235, 237 }, { 235, 251 }, { 219, 268 },
		{ 218, 270 }, { 232, 285 }, { 218, 300 }, { 196, 281 },
		{ 191, 277 }, { 153, 321 }, { 173, 343 }, { 165, 353 },
		{ 147, 331 }, { 107, 368 }, { 128, 391 }, { 119, 401 },
		{ 124, 408 }, { 116, 419 }, { 119, 430 }, { 123, 430 },
		{ 144, 450 }, { 148, 461 }, { 159, 462 }, { 164, 453 },
		{ 169, 451 }, { 177, 459 }, { 182, 455 }
	};
	scene.Add(truck);
#pragma endregion

#pragma region GRASS
	sf::Texture grassTex;
	grassTex.loadFromFile("examples/data/grass.png");

	sf::Sprite grassSpr;
	grassSpr.setTexture(grassTex);
#pragma endregion

#pragma region BOXES
	sf::Texture boxTex;
	boxTex.loadFromFile("examples/data/crate.png");

	sf::Sprite boxSpr;
	boxSpr.setTexture(boxTex);

	std::vector<sf::Vector2f> boxPos = {
		{ 1030, 210 },
		{ 670, 450 },
		{ 790, 340 },
		{ 590, 290 },
	};

	for (auto pos : boxPos) {
		sf::RectangleShape rs;
		rs.setSize(sf::Vector2f(boxSpr.getGlobalBounds().width, boxSpr.getGlobalBounds().height));
		rs.setPosition(pos);
		scene.Add(rs);
	}
#pragma endregion

#pragma region BARRELS
	sf::Texture barrelTex;
	barrelTex.loadFromFile("examples/data/barrel.png");

	sf::Sprite barrelSpr;
	barrelSpr.setTexture(barrelTex);

	std::vector<sf::Vector2f> barrelPos = {
		{ 830, 650 },
		{ 950, 330 }
	};

	for (auto pos : barrelPos) {
		sf::CircleShape cs;
		cs.setPointCount(8);
		cs.setRadius(barrelSpr.getGlobalBounds().width/2);
		cs.setPosition(pos);
		scene.Add(cs);
	}
#pragma endregion

#pragma region RT BLEND
	sf::RenderTexture blendRT;
	blendRT.create(1200, 800);

	sf::Sprite blendSpr;
	blendSpr.setTexture(blendRT.getTexture());
#pragma endregion

	while (wnd.isOpen()) {
		while (wnd.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				wnd.close();
			else if (event.type == sf::Event::Resized)
				wnd.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
		}

		light.SetPosition(sf::Vector2f(sf::Mouse::getPosition(wnd)));
		scene.Update(light);


		blendRT.clear(sf::Color(90, 90, 90));
		light.Render(blendRT);

		blendRT.display();


		wnd.clear();

		wnd.draw(grassSpr);
		wnd.draw(truckSpr);
		for (auto pos : boxPos) {
			boxSpr.setPosition(pos);
			wnd.draw(boxSpr);
		}
		for (auto pos : barrelPos) {
			barrelSpr.setPosition(pos);
			wnd.draw(barrelSpr);
		}

		wnd.draw(blendSpr, sf::BlendMultiply);

		scene.Render(wnd);

		wnd.display();
	}
	return 0;
}