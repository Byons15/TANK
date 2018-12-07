#include "..\Text.h"
#include <SDL_ttf.h>
#include "../Scene.h"
#include "../Renderer.h"

TTF_Font *Text::sm_font = 0;

Text::Text(Scene * scene, const std::wstring & text, Rendering rendering)
	:m_rendering(rendering), Spirit(scene), m_textAnimationIndex(0)
{
	if (!sm_font) 
		sm_font = TTF_OpenFont("simkai.ttf", 12);
}

int Text::setString(const std::wstring & text, int fontSize)
{
	if (m_texture.texture)
		SDL_DestroyTexture(m_texture.texture);

	if (text.empty())
		return 0;

	SDL_Surface* sur;
	switch (m_rendering)
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

	if (!scene())
		return -1;

	m_texture.texture = SDL_CreateTextureFromSurface(scene()->renderer()->renderer(), sur);
	m_texture.frameCount = 1;
	m_texture.clipRect = sur->clip_rect;
	SDL_FreeSurface(sur);

	showAnimation(m_texture);
}

Text::~Text()
{
	if (m_texture.texture)
		SDL_DestroyTexture(m_texture.texture);
}
