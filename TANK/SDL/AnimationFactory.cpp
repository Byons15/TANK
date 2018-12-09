#include "AnimationFactory.h"
#include "../FileLoader.h"
#include <sstream>
#include "../Renderer.h"
#include <SDL_image.h>

AnimationFactory::AnimationFactory()
	:m_renderer(0)
{

}

void AnimationFactory::createFactory(Renderer * renderer)
{
	m_renderer = renderer;

	std::vector<std::string> data;
	if (!fileLoad("texture\\texture.data", "texture", &data))
		throw std::runtime_error("��\"texture\\texture.data\"ʧ��");

	for (auto & line : data) {
		std::istringstream is(line);
		std::string name, textureFile;
		is >> name >> textureFile;
		if (!m_texturePool[name]) {
			SDL_Surface *sur = IMG_Load(textureFile.c_str());
			if (!sur)
				throw std::runtime_error("�������ļ���\"" + textureFile + "\"ʧ�ܣ�");

			m_texturePool[name] = SDL_CreateTextureFromSurface(renderer->renderer(), sur);
			SDL_FreeSurface(sur);
		}
		auto &a = m_animationPool[name];
		a.texture = m_texturePool[name];
		is >> a.clipRect.x >> a.clipRect.y >> a.clipRect.w >> a.clipRect.h >> a.frameCount >> a.frameInterval;
	}
}

const Animation & AnimationFactory::findAnimation(const std::string & name) const
{
	auto iter = m_animationPool.find(name);
	if (iter == m_animationPool.end())
		throw std::out_of_range("û���ҵ�������"+ name);
	
	return iter->second;
}

AnimationFactory::~AnimationFactory()
{
	for (auto & t : m_texturePool) 
		SDL_DestroyTexture(t.second);
}
