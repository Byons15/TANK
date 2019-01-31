#pragma once
#include "Spirit.h"

typedef struct _TTF_Font TTF_Font;

/*
Text(����)��
Text�̳���Spirit�� ����������ʾ���֣�����ȫ֧��Spirit�Ĳ�����
*/
class Text :
	public Spirit
{
public:
	enum RenderFlags
	{
		solid,
		shaded,
		biend,
	};

	Text()
		:Text(0) {}
	Text(Scene *scene, const std::wstring &text = L"", RenderFlags flags = biend);
	int setString(const std::wstring &text = L"");
	void setTextRenderFlags(RenderFlags flags);
	void setFontSize(int size);
	void setFontColor(const SDL_Color &color);
	int fontSize() const {
		return m_fontSize;
	}
	virtual ~Text();
private:
	static TTF_Font *sm_font;
	Animation m_texture;
	size_t m_textAnimationIndex;
	SDL_Color m_stringColor;
	RenderFlags m_flags;
	int m_fontSize;
};

