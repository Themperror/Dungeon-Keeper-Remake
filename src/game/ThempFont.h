#pragma once
#include <vector>
#include <DirectXMath.h>
namespace Themp
{
	class D3D;
	class Object2D;
	struct Texture;
	struct GUITexture;
	class Font
	{
	public:
		enum FontTexID{INGAME= 0,MENU_NORMAL0 = 2, MENU_NORMAL1, MENU_NORMAL2, MENU_NORMAL3};
		~Font();
		Font(std::string text, FontTexID fontTexture, bool hiRes, DirectX::XMFLOAT3 position);
		
		void SetVisibility(bool val);
		void SetScale(float x, float y, float z);
		Object2D* m_ScreenObj = nullptr;
		FontTexID m_TexID = MENU_NORMAL0;
		std::string m_Text;
		std::vector<GUITexture>* m_Font = nullptr;
		GUITexture* m_Texture;

	};
};
