#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>
namespace Themp
{
	class Object2D;
	struct GUITexture;
	struct Texture;
	class Font;
	class GUIButton
	{
	public:
		struct ButtonAnim
		{
			ButtonAnim(std::vector<int> anim,bool doPingPong,int numFPS )
			{
				pingpong = doPingPong;
				animation = anim;
				fps = numFPS;
			}
			std::vector<int> animation;
			bool pingpong;
			int fps;
		};
		struct ButtonInfo
		{
			ButtonAnim* normalAnimation = nullptr;
			ButtonAnim* hoverAnimation = nullptr;
			int normal = 0;
			int hover = 0;
			int click = 0;
			int currentAnimIndex = 0;
			float currentNormalTime = 0.0f;
			float currentHoverTime = 0.0f;
			float xOffset = 0.0f;
			float yOffset = 0.0f;
			float zOffset = 0.0f;
			float xScale = 0.0f;
			float yScale = 0.0f;
			bool reversing = false;
			
		};
		~GUIButton();
		GUIButton(int source, int textureIDNormal, int textureIDHover, int textureIDClick, bool hiRes);
		GUIButton(int source, int* textureIDNormal, int* textureIDHover, int* textureIDClick, float * xOffsets, float * yOffsets, float * zOffsets, int numIDs, bool hiRes = false, std::string text = "", int textFontNormal = 0,int texttFontHover = 0, int textFontDisabled = 0);
		
		void SetSize(float W, float H, float textW, float textH, int index = 0);
		bool Update(float delta, float mouseX, float mouseY, int lmbuttonState, GUIButton * InitialClick);
		void SetVisibility(bool val);
		void SetPosition(const DirectX::XMFLOAT3 & pos);
		void SetPosition(float x, float y, float z = 0);

		std::vector<Object2D*> m_Objects;
		std::vector<ButtonInfo> m_TexIDs;
		std::vector<Font*> m_FontObjects;
		float m_CenterPosX = 0;
		float m_CenterPosY = 0;

		int m_Source = 0;

		bool m_IsHiRes = false;
		bool m_IsDisabled = false;
		bool m_IsColliding = false;
		float m_AnimTime = 0;
		bool m_IsVisible = false;
	};
};
