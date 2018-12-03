#pragma once
#include "Spirit.h"

typedef struct _TTF_Font TTF_Font;

class Text :
	public Spirit
{
public:

	enum Rendering
	{
		solid,
		shaded,
		biend,
	};

	Text()
		:Text(0, { 0, 0 }) {}
	Text(Scene *scene, const std::wstring &text = L"", Rendering rendering = shaded);
	int setString(const std::wstring &text = L"");
	virtual ~Text();
private:
	static TTF_Font *sm_font;
	Animation m_texture;
	size_t m_textAnimationIndex;
	SDL_Color m_stringColor;
	Rendering m_rendering;
};

