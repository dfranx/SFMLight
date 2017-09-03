#ifndef __LIGHT_SCENE_H__
#define __LIGHT_SCENE_H__

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <Object.h>
#include <Light.h>
#include <vector>

namespace sfl
{
	class LightScene : public sf::Drawable
	{
	public:
		inline void Add(const Object& obj) { m_objs.push_back(obj); }
		void Add(const sf::Shape& shp);

		inline int GetObjectCount() { return m_objs.size(); }

		inline Object& Get(int i) { return m_objs[i]; }

		void Update(sfl::Light& l);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		std::vector<Object> m_objs;
	};
}

#endif //__LIGHT_SCENE_H__