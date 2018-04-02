#include <SFML/Graphics.hpp>
#include <SFMLight.h>
#include <sstream>

using namespace std;

int main() {
	sf::RenderWindow wnd(sf::VideoMode(1200, 800), "Light example", sf::Style::Titlebar | sf::Style::Close);
	sf::Event event;

	sfl::LightScene scene;

	sfl::Light light;
	light.SetRadius(300);
	light.SetDetailLevel(sfl::LightDetail::High);
	light.SetOuterColor(sf::Color(255, 255, 255, 0));

	std::vector<sfl::Light> lights;
	
#pragma region TRUCK
	sf::Texture truckTex;
	truckTex.loadFromFile("data/truck.png");

	sf::Sprite truckSpr;
	truckSpr.setTexture(truckTex);
	truckSpr.setOrigin(truckTex.getSize().x / 2, truckTex.getSize().y / 2);
	truckSpr.setRotation(45);
	truckSpr.setPosition(300, 300);
	truckSpr.setScale(2, 2);

	sfl::Object truck;
	truck.Points = {
		{ 207, 474 },{ 289, 393 },{ 289, 382 },{ 271, 363 },
		{ 291, 342 },{ 304, 358 },{ 322, 340 },{ 328, 348 },
		{ 340, 347 },{ 350, 336 },{ 366, 320 },{ 386, 300 },
		{ 398, 286 },{ 406, 278 },{ 411, 273 },{ 416, 266 },
		{ 425, 282 },{ 428, 278 },{ 426, 259 },{ 478, 206 },
		{ 480, 188 },{ 472, 176 },{ 393, 100 },{ 382, 95 },
		{ 371, 95 },{ 366, 103 },{ 366, 110 },{ 360, 114 },
		{ 354, 121 },{ 318, 152 },{ 299, 146 },{ 297, 154 },
		{ 307, 164 },{ 235, 237 },{ 235, 251 },{ 219, 268 },
		{ 218, 270 },{ 232, 285 },{ 218, 300 },{ 196, 281 },
		{ 191, 277 },{ 153, 321 },{ 173, 343 },{ 165, 353 },
		{ 147, 331 },{ 107, 368 },{ 128, 391 },{ 119, 401 },
		{ 124, 408 },{ 116, 419 },{ 119, 430 },{ 123, 430 },
		{ 144, 450 },{ 148, 461 },{ 159, 462 },{ 164, 453 },
		{ 169, 451 },{ 177, 459 },{ 182, 455 }
	};
	scene.Add(truck);
#pragma endregion

#pragma region GRASS
	sf::Texture grassTex;
	grassTex.loadFromFile("data/grass.png");

	sf::Sprite grassSpr;
	grassSpr.setTexture(grassTex);
#pragma endregion

#pragma region BOXES
	sf::Texture boxTex;
	boxTex.loadFromFile("data/crate.png");

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
	barrelTex.loadFromFile("data/barrel.png");

	sf::Sprite barrelSpr;
	barrelSpr.setTexture(barrelTex);

	std::vector<sf::Vector2f> barrelPos = {
		{ 830, 650 },
		{ 950, 330 }
	};

	for (auto pos : barrelPos) {
		sf::CircleShape cs;
		cs.setPointCount(8);
		cs.setRadius(barrelSpr.getGlobalBounds().width / 2);
		cs.setPosition(pos);
		scene.Add(cs);
	}
#pragma endregion

#pragma region TEXT
	sf::Font font;
	font.loadFromFile("data/arial.ttf");

	sf::Text text;
	text.setFont(font);
	text.setPosition(10, 10);
	text.setCharacterSize(14);
	text.setFillColor(sf::Color::White);
#pragma endregion

#pragma region RT BLEND
	sf::RenderTexture blendRT;
	blendRT.create(1200, 800);

	sf::Sprite blendSpr;
	blendSpr.setTexture(blendRT.getTexture());
#pragma endregion

	// properties
	int layer = 0;
	bool drawUI = true, dark = false;

	srand(time(0));

	while (wnd.isOpen()) {
		while (wnd.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				wnd.close();
			else if (event.type == sf::Event::Resized)
				wnd.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
			else if (event.type == sf::Event::KeyReleased) {
				if (event.key.code >= sf::Keyboard::F1 && event.key.code <= sf::Keyboard::F3)
					layer = event.key.code - sf::Keyboard::F1;
				else if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::S)
					light.SetDetailLevel(std::max(0, std::min(sfl::LightDetail::High, light.GetDetailLevel() + (event.key.code == sf::Keyboard::W ? 1 : -1))));
				else if (event.key.code == sf::Keyboard::D) {
					light.SetDebugDraw(!light.GetDebugDraw());
					scene.SetDebugDraw(light.GetDebugDraw());

					for (size_t i = 0; i < lights.size(); i++)
						lights[i].SetDebugDraw(!lights[i].GetDebugDraw());
				}
				else if (event.key.code == sf::Keyboard::A) {
					sf::Color clr = light.GetOuterColor();
					if (clr.a == 255)
						clr.a = 0;
					else clr.a = 255;
					light.SetOuterColor(clr);
				}
				else if (event.key.code == sf::Keyboard::R) {
					sf::Color clrIn = sf::Color(rand() % 255, rand() % 255, rand() % 255, light.GetInnerColor().a);
					sf::Color clrOut = sf::Color(rand() % 255, rand() % 255, rand() % 255, light.GetOuterColor().a);
					light.SetOuterColor(clrOut);
					light.SetInnerColor(clrIn);
				}
				else if (event.key.code == sf::Keyboard::M) {
					sf::Color clrIn = sf::Color(255, 0, 0, 255);
					sf::Color clrOut = sf::Color(0, 0, 255, 25);
					light.SetOuterColor(clrOut);
					light.SetInnerColor(clrIn);
					dark = true;
				}
				else if (event.key.code == sf::Keyboard::I)
					drawUI = !drawUI;
				else if (event.key.code == sf::Keyboard::Q)
					dark = !dark;
				else if (event.key.code == sf::Keyboard::L)
					scene.SetObjectDraw(!scene.GetObjectDraw());
			} else if (event.type == sf::Event::MouseWheelScrolled) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && light.GetRayCount() >= 3)
					light.SetRayCount(std::max(3, light.GetRayCount() + (int)event.mouseWheelScroll.delta));
				else if (light.GetRadius() >= 50)
					light.SetRadius(std::max(50, light.GetRadius() + (int)event.mouseWheelScroll.delta * 4));
			} else if (event.type == sf::Event::MouseButtonReleased) {
				scene.Cache(light);
				lights.push_back(light);
			}
		}

		std::stringstream ss;
		ss << "Left click to place a light\nLayer: " << layer << " [F1 & F2]\nRadius: " << light.GetRadius() << " [Wheel]\nRay count: " << light.GetRayCount() << " [Shift + Wheel]\nLevel detail: " << light.GetDetailLevel() << " [W & S]\nDebug draw: " << light.GetDebugDraw() << " [D]\nOuter color alpha: " << (int)light.GetOuterColor().a << " [A]\nMy colored light [M]\nRandom colored light [R]\nComplete darkness: " << dark << " [Q]\nDraw lit objects: " << scene.GetObjectDraw() << " [L]\nUI: " << drawUI << " [I]";
		text.setString(ss.str());
		
		light.SetPosition(sf::Vector2f(sf::Mouse::getPosition(wnd)));
		scene.Update(light);


		blendRT.clear(dark ? sf::Color(0, 0, 0) : sf::Color(90, 90, 90));
		for (size_t i = 0; i < lights.size(); i++)
			lights[i].Render(blendRT, sf::BlendAdd);
		scene.Render(blendRT);
		light.Render(blendRT, sf::BlendAdd);
		blendRT.display();



		wnd.clear();

		wnd.draw(grassSpr);

		if (layer == 0) {
			wnd.draw(truckSpr);
			for (auto pos : boxPos) {
				boxSpr.setPosition(pos);
				wnd.draw(boxSpr);
			}
			for (auto pos : barrelPos) {
				barrelSpr.setPosition(pos);
				wnd.draw(barrelSpr);
			}
		}

		wnd.draw(blendSpr, sf::BlendMultiply);

		if (layer == 1) {
			wnd.draw(truckSpr);
			for (auto pos : boxPos) {
				boxSpr.setPosition(pos);
				wnd.draw(boxSpr);
			}
			for (auto pos : barrelPos) {
				barrelSpr.setPosition(pos);
				wnd.draw(barrelSpr);
			}
		}

		if (drawUI)
			wnd.draw(text);

		if (scene.GetDebugDraw()) {
			bool status = scene.GetObjectDraw();
			scene.SetObjectDraw(false);
			scene.Render(wnd);
			scene.SetObjectDraw(status);
		}

		wnd.display();
	}
	return 0;
}