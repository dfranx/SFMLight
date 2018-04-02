#ifndef __LIGHT_SCENE_H__
#define __LIGHT_SCENE_H__

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <sfl/Config.h>
#include <sfl/Object.h>
#include <sfl/Light.h>
#include <vector>
#include <unordered_map>

namespace sfl
{
	class SFMLIGHT_EXPORT LightScene
	{
	public:
		LightScene();
		~LightScene();

		// add object to a scene
		void Add(const Object& obj);
		void Add(const sf::Shape& shp);

		// get number of objects and get certain object
		inline int GetObjectCount() { return m_objs.size(); }
		inline Object& Get(int i) { return m_objs[i]; }

		// should we draw debug information?
		inline void SetDebugDraw(bool deb) { m_debug = deb; }
		inline bool GetDebugDraw() { return m_debug; }

		// should we draw lit objects
		inline void SetObjectDraw(bool drw) { m_dobj = drw; }
		inline bool GetObjectDraw() { return m_dobj; }

		/////////////////////////////////////////////////////////
		// save the list of lit objects
		/////////////////////////////////////////////////////////
		void Cache(sfl::Light& l);

		/////////////////////////////////////////////////////////
		// update the light based on the objects
		/////////////////////////////////////////////////////////
		void Update(sfl::Light& l);

		/////////////////////////////////////////////////////////
		// render the scene (objects, debug info)
		/////////////////////////////////////////////////////////
		void Render(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);

	private:
		bool m_debug, m_dobj;			// m_debug -> should we draw object outlines? m_dobj -> should we draw lit objects?
		std::vector<Object> m_objs;		// list of all objects in the scene
		std::vector<bool> m_lit;		// contains info whether the object is lit or not
		std::vector<bool> m_cachedLit;	// contains info whether the object is lit or not

		// cache all concave shapes
		std::vector<sf::VertexArray> m_cachedObjs;
	};
}

#endif //__LIGHT_SCENE_H__