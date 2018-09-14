#pragma once
#include <vector>
namespace Themp
{
	class D3D;
	class Object3D;
	struct Texture;
	struct GUITexture;
	class Object2D
	{
	public:
		enum Source{FILE,LEVEL_MISC,LEVEL_PANE,MENU_MAIN,MENU_CURSOR};
		~Object2D();
		Object2D(std::wstring path, std::wstring ext);
		Object2D(Source textureSource, int textureIndex, bool hiRes);
		
		void SetVisibility(bool val);
		void SetSprite(int offX, int offY, int W, int H, int MAXW, int MAXH);
		Texture* m_Tex = nullptr;
		GUITexture* m_GUITex = nullptr;
		Object3D* m_Renderable = nullptr;
		Source m_Source = FILE;

	};
};
