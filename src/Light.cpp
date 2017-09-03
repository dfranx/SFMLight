#include <Light.h>
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
	}
	Light::~Light()
	{
	}
	void Light::draw(sf::RenderTarget & tgt, sf::RenderStates states) const
	{
		tgt.draw(&m_verts[0], m_verts.size(), sf::TrianglesFan, states);
	}
}