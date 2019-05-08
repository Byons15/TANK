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

	SDL_Surface *sur = IMG_Load("texture\\texture.png");
	if (!sur)
		throw std::runtime_error("打开纹理文件texture.png失败！");

	m_texture = SDL_CreateTextureFromSurface(renderer->renderer(), sur);
	SDL_FreeSurface(sur);

	fileLoad("texture", &data);
	
	for (auto &l : data) {
		std::istringstream is(l);
		std::string name;
		is >> name;
		auto &a = m_animationPool[name];
		a.texture = m_texture;
		is >> a.clipRect.x >> a.clipRect.y >> a.clipRect.w >> a.clipRect.h >> a.frameCount >> a.frameTimeInterval >> a.frameInterval;
		a.clipRect.x *= 80;
		a.clipRect.y *= 80;
	}
}

const Animation & AnimationFactory::findAnimation(const std::string & name) const
{
	auto iter = m_animationPool.find(name);
	if (iter == m_animationPool.end())
		throw std::out_of_range("没有找到动画："+ name);
	
	return iter->second;
}

AnimationFactory::~AnimationFactory()
{
	for (auto & t : m_texturePool) 
		SDL_DestroyTexture(t.second);
}
