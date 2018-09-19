#pragma once
#include <vector>
#include <DirectXMath.h>
namespace Themp
{
	class D3D;
	class Object3D;
	class Material;
	struct Texture;
	struct GUITexture;
	class Object2D
	{
	public:
		enum Source{sFILE,sLEVEL_MISC,sLEVEL_PANE,sMENU_MAIN,sMENU_CURSOR,sCUSTOM};
		~Object2D();
		Object2D();
		Object2D(std::wstring path, std::wstring ext);
		Object2D(Source textureSource, int textureIndex, bool hiRes);
		
		void SetVisibility(bool val);
		void SetTexture(GUITexture * tex);
		void SetScale(float W, float H);
		DirectX::XMFLOAT2 GetSizeinScreenPercentage(int screenWidth, int screenHeight);
		//void SetSprite(int offX, int offY, int W, int H, int MAXW, int MAXH);
		Texture* m_Tex = nullptr;
		Material* m_Material = nullptr;
		GUITexture* m_GUITex = nullptr;
		Object3D* m_Renderable = nullptr;
		Source m_Source = sFILE;

	};
};
