#pragma once
#include <vector>
namespace Themp
{
	class D3D;
	class Object3D;
	struct Texture;
	class Font
	{
	public:
		enum FontTexID{INGAME,MENU_NORMAL,MENU_HOVER,MENU_CLICK};
		~Font();
		Font(std::string text, FontTexID fontTexture, bool hiRes);
		
		void SetVisibility(bool val);
		Object3D* m_Renderable = nullptr;
		FontTexID m_TexID = MENU_NORMAL;

	};
};
