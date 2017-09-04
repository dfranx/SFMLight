#include <LightScene.h>
#include <Utils.h>

#define _USE_MATH_DEFINES
#include <math.h>

namespace sfl
{
	LightScene::LightScene()
	{
		m_debug = false;
		m_dobj = false;
	}

	LightScene::~LightScene()
	{
	}

	void LightScene::Add(const sf::Shape & shp)
	{
		Object obj;
		size_t cnt = shp.getPointCount();
		for (size_t i = 0; i < cnt; i++) {
			sf::Vector2f p = shp.getPosition() + shp.getPoint(i);
			obj.Add(p.x, p.y);
		}
		
		Add(obj);
	}

	void LightScene::Update(sfl::Light& light)
	{
		Line ray;
		bool skipObject;
		std::vector<sf::Vertex>& varr = light.GetVertices();
		std::unordered_map<int, bool> highDefColObjs;

		skipObject = false;
		ray.Start = light.GetPosition();

		varr.clear();
		varr.push_back(sf::Vertex(ray.Start, light.GetInnerColor()));
		
		for (size_t i = 0; i < m_objs.size(); i++)
			m_lit[i] = false;

		// For every light detail
		for (float angle = 0; angle < 2 * M_PI; angle += 2 * M_PI / light.GetRayCount()) {
			sf::Vector2f resPos = ray.End = sf::Vector2f(ray.Start.x + cos(angle) * light.GetRadius(), ray.Start.y + sin(angle) * light.GetRadius());

			skipObject = false;
			for (size_t i = 0; i < m_objs.size(); i++) {
				Object* obj = &m_objs[i];
				sf::Vector2f interPos;

				for (size_t j = 0; j < obj->GetLineCount(); j++) {
					if (Utils::Length(ray.Start, obj->Points[j]) < light.GetRadius())
						highDefColObjs[i] = true;

					if (ray.Intersects(obj->GetLine(j), interPos)) {
						if (Utils::Length(ray.Start, resPos) > Utils::Length(ray.Start, interPos)) {
							resPos = interPos;
							highDefColObjs[i] = true;
							m_lit[i] = true;
						}
						skipObject = true;
					}
				}
			}

			if (skipObject && light.GetDetailLevel() == LightDetail::Medium)
				continue;

			sf::Color clr = light.GetOuterColor();
			clr.a = 255-Utils::Length(ray.Start, resPos)*((255.0f-clr.a)/light.GetRadius());
			
			varr.push_back(sf::Vertex(resPos, clr));
		}


		// For medium and high light detail
		if (light.GetDetailLevel() > LightDetail::Low) {
			for (size_t i = 0; i < m_objs.size(); i++) {

				if (!highDefColObjs[i])
					continue;

				Object* obj = &m_objs[i];

				for (size_t j = 0; j < obj->Points.size(); j++) {
					sf::Vector2f point = obj->Points[j];

					if (Utils::Length(ray.Start, point) > light.GetRadius())
						continue;

					float newAngle = atan2(point.y - ray.Start.y, point.x - ray.Start.x);

					// cast 3 rays for each point in Object
					for (float angle = newAngle - 0.01f; angle <= newAngle + 0.01f; angle += 0.01f) {
						ray.End = sf::Vector2f(ray.Start.x + cos(angle) * light.GetRadius(), ray.Start.y + sin(angle) * light.GetRadius());

						int res = 0;
						sf::Vector2f interPos, resPos = ray.End;
						for (size_t k = 0; k < m_objs.size(); k++) {
							Object* obj2 = &m_objs[k];
							for (size_t l = 0; l < obj2->Points.size(); l++)
								if (ray.Intersects(obj2->GetLine(l), interPos))
									if (Utils::Length(ray.Start, resPos) > Utils::Length(ray.Start, interPos)) {
										resPos = interPos;
										res = k;
									}
						}

						if (res == j)
							m_lit[res] = true;


						float tempLength = Utils::Length(ray.Start, resPos);
						sf::Color clr = light.GetOuterColor();
						clr.r = 255 - tempLength*((255.0f - clr.r) / light.GetRadius());
						clr.g = 255 - tempLength*((255.0f - clr.g) / light.GetRadius());
						clr.b = 255 - tempLength*((255.0f - clr.b) / light.GetRadius());
						clr.a = 255 - tempLength*((255.0f - clr.a) / light.GetRadius());

						varr.push_back(sf::Vertex(resPos, clr));
					}
				}
			}
		}

		std::sort(varr.begin() + 1, varr.end(), [=](sf::Vertex a, sf::Vertex b) {
			return atan2(a.position.y - ray.Start.y, a.position.x - ray.Start.x) < atan2(b.position.y - ray.Start.y, b.position.x - ray.Start.x);
		});

		varr.push_back(varr[1]);
	}

	void LightScene::Render(sf::RenderTarget & target, sf::RenderStates states)
	{
		if (m_debug) {
			std::vector<sf::Vertex> varr;

			for (auto oit = m_objs.begin(); oit != m_objs.end(); oit++) {
				varr.resize(oit->Points.size());
				sf::Vertex* ver = &varr[0];
				for (auto pit = oit->Points.begin(); pit != oit->Points.end(); pit++, ver++) {
					ver->position = *pit;
					ver->color = sf::Color::Green;
				}

				varr.push_back(sf::Vertex(oit->Points[0], sf::Color::Green));
				target.draw(&varr[0], varr.size(), sf::LineStrip);
			}
		}
		if (m_dobj) {
			std::vector<sf::Vertex> varr;
			for (size_t i = 0; i < m_objs.size(); i++) {
				if (!m_lit[i])
					continue;

				Object* obj = &m_objs[i];
				varr.resize(obj->Points.size());
				sf::Vertex* ver = &varr[0];
				for (auto pit = obj->Points.begin(); pit != obj->Points.end(); pit++, ver++) {
					ver->position = *pit;
					ver->color = sf::Color::White;
				}

				varr.push_back(sf::Vertex(obj->Points[0], sf::Color::White));
				target.draw(&varr[0], varr.size(), sf::TriangleFan);
			}
		}
	}
}