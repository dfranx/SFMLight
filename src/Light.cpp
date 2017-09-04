#include <Light.h>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace sfl
{
	Light::Light()
	{
		SetPosition(0, 0);
		m_radius = 300;
		m_rayCount = 24;
		m_detail = LightDetail::Medium;
		m_inClr = sf::Color::White;
		m_outClr = sf::Color::White;
		m_debug = false;
	}
	Light::~Light()
	{
	}
	void Light::Render(sf::RenderTarget & tgt, sf::RenderStates states)
	{
		tgt.draw(&m_verts[0], m_verts.size(), sf::TrianglesFan, states);

		if (m_debug) {
			std::vector<sf::Vertex> arr((m_verts.size() - 2)*2);
			for (size_t i = 0; i < arr.size(); i+=2) {
				arr[i].position = m_verts[0].position;
				arr[i].color = sf::Color::Red;

				arr[i + 1].position = m_verts[i / 2 + 1].position;
				arr[i + 1].color = sf::Color::Red;
			}

			tgt.draw(&arr[0], arr.size(), sf::Lines);
		}
	}
}