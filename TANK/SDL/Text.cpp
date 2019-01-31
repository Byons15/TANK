#include "..\Text.h"
#include <SDL_ttf.h>
#include "../Scene.h"
#include "../Renderer.h"

TTF_Font *Text::sm_font = 0;

static constexpr int defaultFontSize = 30;

Text::Text(Scene * scene, const std::wstring & text, RenderFlags rendering)
	:m_flags(rendering), Spirit(scene), m_textAnimationIndex(0), m_fontSize(defaultFontSize), m_stringColor{255, 255, 255, 0}
{
	if (!sm_font) {
		sm_font = TTF_OpenFont("simkai.ttf", m_fontSize);
	}

	m_texture.frameCount = 1;
	setString(text);
}

int Text::setString(const std::wstring & text)
{
	//参数检查。
	if (m_texture.texture)
		SDL_DestroyTexture(m_texture.texture);

	if (text.empty())
		return 0;

	if (!scene() || !scene()->renderer())
		return -1;

	//设置字体大小。
	TTF_SetFontSize(sm_font, m_fontSize);

	//按照渲染方式生成表面。
	SDL_Surface* sur;
	switch (m_flags)
	{
	case Text::solid:
		sur = TTF_RenderUNICODE_Solid(sm_font, (Uint16 *)text.c_str(), m_stringColor);
		break;
	case Text::shaded: {
		Uint8 bkR = m_stringColor.r, bkG = ~m_stringColor.g, bkB = ~m_stringColor.b;
		sur = TTF_RenderUNICODE_Shaded(sm_font, (Uint16 *)text.c_str(), m_stringColor, { bkR, bkG, bkG });
		SDL_SetColorKey(sur, SDL_TRUE, SDL_MapRGB(sur->format, bkR, bkG, bkB));
		break;
	}
	case Text::biend:
		sur = TTF_RenderUNICODE_Blended(sm_font, (Uint16 *)text.c_str(), m_stringColor);
		break;
	default:
		throw std::runtime_error("class \"Text\" error");
		break;
	}
	TTF_SetFontSize(sm_font, defaultFontSize);   //复原字体大小。

	//从表面生成纹理并释放表面对象。
	m_texture.texture = SDL_CreateTextureFromSurface(scene()->renderer()->renderer(), sur);
	m_texture.clipRect = sur->clip_rect;
	SDL_FreeSurface(sur);

	//将纹理设置到精灵。
	setAnimation(m_texture);
	return 0;
}

void Text::setTextRenderFlags(RenderFlags flags)
{
	m_flags = flags;
}

void Text::setFontSize(int size)
{
	m_fontSize = size;
}

void Text::setFontColor(const SDL_Color & color)
{
	m_stringColor = color;
}

Text::~Text()
{
	if (m_texture.texture)
		SDL_DestroyTexture(m_texture.texture);
}
