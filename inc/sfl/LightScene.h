#ifndef __LIGHT_SCENE_H__
#define __LIGHT_SCENE_H__

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <sfl/Object.h>
#include <sfl/Light.h>
#include <vector>
#include <unordered_map>

namespace sfl
{
	class LightScene
	{
	public:
		LightScene();
		~LightScene();

		inline void Add(const Object& obj) { m_objs.push_back(obj); }
		void Add(const sf::Shape& shp);

		inline int GetObjectCount() { return m_objs.size(); }
		inline Object& Get(int i) { return m_objs[i]; }

		inline void SetDebugDraw(bool deb) { m_debug = deb; }
		inline bool GetDebugDraw() { return m_debug; }

		inline void SetObjectDraw(bool drw) { m_dobj = drw; }
		inline bool GetObjectDraw() { return m_dobj; }

		void Update(sfl::Light& l);

		void Render(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);

	private:
		bool m_debug, m_dobj;
		std::vector<Object> m_objs;
		std::unordered_map<int, bool> m_lit;
	};
}

#endif //__LIGHT_SCENE_H__