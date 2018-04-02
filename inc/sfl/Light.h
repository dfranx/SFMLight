#ifndef __SFMLIGHT_LIGHT_H__
#define __SFMLIGHT_LIGHT_H__

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <sfl/Config.h>
#include <vector>

namespace sfl
{
	namespace LightDetail
	{
		const int Low = 0;		// doesnt care about object's points
		const int Medium = 1;	// casts 2 rays per object's point + doesnt cast rays that collide with objects
		const int High = 2;		// casts 3 rays per object's point
	}

	class SFMLIGHT_EXPORT Light
	{
	public:
		Light();
		~Light();

		inline int GetRadius() { return m_radius; }
		inline void SetRadius(int r) { m_radius = r; }

		inline sf::Color GetInnerColor() { return m_inClr; }
		inline sf::Color GetOuterColor() { return m_outClr; }
		inline void SetInnerColor(const sf::Color& clr) { m_inClr = clr; }
		inline void SetOuterColor(const sf::Color& clr) { m_outClr = clr; }

		inline void SetPosition(const sf::Vector2f& pos) { m_pos = pos; }
		inline void SetPosition(float x, float y) { m_pos = sf::Vector2f(x, y); }
		inline sf::Vector2f GetPosition() { return m_pos; }

		inline std::vector<sf::Vertex>& GetVertices() { return m_verts; }

		inline void SetDetailLevel(int det) { m_detail = det; }
		inline int GetDetailLevel() { return m_detail; }

		inline void SetRayCount(int c) { m_rayCount = c; }
		inline int GetRayCount() { return m_rayCount; }

		inline void SetDebugDraw(bool deb) { m_debug = deb; }
		inline bool GetDebugDraw() { return m_debug; }

		/////////////////////////////////////////////////////////
		// render the light to a sf::RenderTarget
		/////////////////////////////////////////////////////////
		void Render(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);

	private:
		bool m_debug;						// should we draw rays
		sf::Vector2f m_pos;					// light position
		std::vector<sf::Vertex> m_verts;	// light vertices
		int m_radius, m_detail, m_rayCount;	// radius of the light, detail level and number of rays
		sf::Color m_inClr, m_outClr;		// outer and inner color of a light
	};
}
#endif //__SFMLIGHT_LIGHT_H__