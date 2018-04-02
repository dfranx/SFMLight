#include <sfl/LightScene.h>
#include <sfl/Utils.h>
#include <Thor/Math/Triangulation.hpp>
#include <Thor/Shapes/ConcaveShape.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

namespace sfl
{
	struct TriangleGenerator
	{
		TriangleGenerator(sf::VertexArray& triangleVertices, const sf::Color& color)
			: triangleVertices(&triangleVertices)
			, color(color)
		{
			triangleVertices.clear();
		}
		
		// Fake dereferencing
		TriangleGenerator& operator* ()
		{
			return *this;
		}
		
		// Fake pre-increment
		TriangleGenerator& operator++ ()
		{
			return *this;
		}
		
		// Fake post-increment
		TriangleGenerator& operator++ (int)
		{
			return *this;
		}
		
		// Assignment from triangle
		TriangleGenerator& operator= (const thor::Triangle<const sf::Vector2f>& triangle)
		{
			triangleVertices->append(sf::Vertex(triangle[0], color));
			triangleVertices->append(sf::Vertex(triangle[1], color));
			triangleVertices->append(sf::Vertex(triangle[2], color));
			
			return *this;
		}
		
		sf::VertexArray*	triangleVertices;
		sf::Color			color;
	};

	LightScene::LightScene()
	{
		m_debug = false;
		m_dobj = false;
	}

	LightScene::~LightScene()
	{
	}

	void LightScene::Add(const sfl::Object& obj)
	{
		m_objs.push_back(obj);
		m_lit.push_back(false);
		m_cachedLit.push_back(false);
		m_cachedObjs.push_back(sf::VertexArray(sf::PrimitiveType::Triangles));

		sf::VertexArray& varr = m_cachedObjs[m_cachedObjs.size()-1];

		thor::triangulatePolygon(obj.Points.begin(), obj.Points.end(), TriangleGenerator(varr, sf::Color::White));
	}

	void LightScene::Add(const sf::Shape& shp)
	{
		Object obj;
		size_t cnt = shp.getPointCount();
		for (size_t i = 0; i < cnt; i++) {
			sf::Vector2f p = shp.getPosition() + shp.getPoint(i);
			obj.Add(p.x, p.y);
		}
		
		Add(obj);
	}
	
	void LightScene::Cache(sfl::Light& l)
	{
		Update(l);

		for (size_t i = 0; i < m_lit.size(); i++)
			if (m_lit[i])
				m_cachedLit[i] = true;
	}

	void LightScene::Update(sfl::Light& light)
	{
		Line ray;
		std::vector<sf::Vertex>& varr = light.GetVertices();	// lights vertex data that we will modify
		std::vector<bool> highDefColObjs(m_objs.size());		// defines whether this object has some points that is inside our light's radius
		
		float angleStep = 2 * M_PI / light.GetRayCount();			// step per ray
		float detailedAngleStep = ((light.GetDetailLevel() == LightDetail::Medium) ? 0.02f : 0.01f); // step per detail ray

		sf::Color gClr = light.GetOuterColor(); // get lights outer color (goal color)
		sf::Color cClr = light.GetInnerColor(); // get lights inner color (current color)

		ray.Start = light.GetPosition();

		// clear the vertex array and start over
		varr.clear();
		varr.push_back(sf::Vertex(ray.Start, light.GetInnerColor()));
		
		// reset the lit objects
		for (size_t i = 0; i < m_lit.size(); i++)
			m_lit[i] = false;

		// For every light detail
		for (float angle = 0; angle < 2 * M_PI; angle += angleStep) {
			// get the end position of a ray
			sf::Vector2f resPos = ray.End = sf::Vector2f(sin(angle) * light.GetRadius() + ray.Start.x, cos(angle) * light.GetRadius() + ray.Start.y);

			// should we add additional ray on LightDetail::Medium if it collides with the object
			bool skipObject = false;

			// save the last object we hit wit our ray
			int res_lit = -1;

			for (size_t i = 0; i < m_objs.size(); i++) { // go through object list
				Object* obj = &m_objs[i];	// current object

				for (size_t j = 0; j < obj->GetLineCount(); j++) {	// go through object's edges
					sfl::Line edge = obj->GetLine(j);

					// check if edge is inside of the lights radius
					if (Utils::Length(ray.Start, edge.Start) < light.GetRadius() || Utils::Length(ray.Start, edge.End) < light.GetRadius())
						highDefColObjs[i] = true; // cast multiple rays per point since it is in light's radius
					else continue;	// its not inside the radius so we can skip it

					sf::Vector2f interPos; // where does the ray intersect the edge?

					if (ray.Intersects(obj->GetLine(j), interPos)) { // check if they do and get the intersection point
						if (Utils::Length(ray.Start, resPos) > Utils::Length(ray.Start, interPos)) {
							resPos = interPos;	// set the new result position
							res_lit = i;		// save the object
						}
						skipObject = true; // we can skip this ray if we are using LightDetail::Medium
					}
				}
			}

			if (res_lit != -1)
				m_lit[res_lit] = true;

			if (skipObject && light.GetDetailLevel() == LightDetail::Medium)
				continue; // skip the ray since it collides with object (saves one vertex per collision!)

			// interpolate the color
			float percent = 1 - Utils::Length(ray.Start, resPos) / light.GetRadius();
			sf::Color clr;
			clr.a = gClr.a + (cClr.a - gClr.a) * percent;
			clr.r = gClr.r + (cClr.r - gClr.r) * percent;
			clr.g = gClr.g + (cClr.g - gClr.g) * percent;
			clr.b = gClr.b + (cClr.b - gClr.b) * percent;
			
			// add the vertex
			varr.push_back(sf::Vertex(resPos, clr));
		}


		// cast rays per object point (only for medium and high light detail)
		if (light.GetDetailLevel() > LightDetail::Low) {
			for (size_t i = 0; i < m_objs.size(); i++) { // go through object list
				if (!highDefColObjs[i])
					continue;

				Object* obj = &m_objs[i];

				for (size_t j = 0; j < obj->Points.size(); j++) { // go through object's points
					sf::Vector2f point = obj->Points[j];

					if (Utils::Length(ray.Start, point) > light.GetRadius()) // check if it is in light's radius
						continue;

					// calculate the angle
					float newAngle = atan2(point.y - ray.Start.y, point.x - ray.Start.x);

					// cast 3 rays (for high level detail) or 2 rays (for medium level detail) for each point in Object
					for (float angle = newAngle - 0.01f; angle <= newAngle + 0.011f; angle += detailedAngleStep) {
						ray.End = sf::Vector2f(cos(angle) * light.GetRadius() + ray.Start.x, sin(angle) * light.GetRadius() + ray.Start.y);

						sf::Vector2f interPos,	// save intersection point
							resPos = ray.End;	// where does the ray actually end

						int res_lit = -1;

						for (size_t k = 0; k < m_objs.size(); k++) {
							Object* obj2 = &m_objs[k];
							for (size_t l = 0; l < obj2->Points.size(); l++) // go through object list again
								if (ray.Intersects(obj2->GetLine(l), interPos)) // check if the ray intersects any of them
									if (Utils::Length(ray.Start, resPos) > Utils::Length(ray.Start, interPos)) { // if its closer to light source its ray's new end position
										resPos = interPos;
										res_lit = k;
									}
						}

						if (res_lit != -1)
							m_lit[res_lit] = true;

						// interpolate color
						float percent = 1 - Utils::Length(ray.Start, resPos) / light.GetRadius();
						sf::Color clr = gClr;
						clr.a = gClr.a + (cClr.a - gClr.a) * percent;
						clr.r = gClr.r + (cClr.r - gClr.r) * percent;
						clr.g = gClr.g + (cClr.g - gClr.g) * percent;
						clr.b = gClr.b + (cClr.b - gClr.b) * percent;

						varr.push_back(sf::Vertex(resPos, clr));
					}
				}
			}
		}

		// sort the vertices so that we can render them
		std::sort(varr.begin() + 1, varr.end(), [=](sf::Vertex a, sf::Vertex b) {
			/* https://stackoverflow.com/questions/7774241/sort-points-by-angle-from-given-axis */

			const sf::Vector2f da = a.position - varr[0].position, db = b.position - varr[0].position, dreference = varr[1].position;
			const double detb = Utils::CrossProduct(dreference, db);

			// nothing is less than zero degrees
			if (detb == 0 && db.x * dreference.x + db.y * dreference.y >= 0) return false;

			const double deta = Utils::CrossProduct(dreference, da);

			// zero degrees is less than anything else
			if (deta == 0 && da.x * dreference.x + da.y * dreference.y >= 0) return true;

			if (deta * detb >= 0) {
				// both on same side of reference, compare to each other
				return Utils::CrossProduct(da, db) > 0;
			}

			// vectors "less than" zero degrees are actually large, near 2 pi
			return deta > 0;
		});

		// close the circle
		varr.push_back(varr[1]);
	}

	void LightScene::Render(sf::RenderTarget & target, sf::RenderStates states)
	{
		// draw object outlines
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

		// draw lit objects
		if (m_dobj) {
			for (size_t i = 0; i < m_cachedObjs.size(); i++) {
				if (!m_lit[i] && !m_cachedLit[i])
					continue;

				target.draw(m_cachedObjs[i]);
			}
		}
	}
}