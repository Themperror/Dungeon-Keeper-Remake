#pragma once
#include <vector>
#include <string>
namespace Themp
{
	class Object2D;
	struct Texture;
	class Font;
	class GUIButton
	{
		struct ButtonInfo
		{
			std::vector<int>* animation = nullptr;
			int normal;
			int hover;
			int click;
			float xOffset;
			float yOffset;
			float zOffset;
			float xScale;
			float yScale;
		};
	public:
		~GUIButton();
		GUIButton(int source, int textureIDNormal, int textureIDHover, int textureIDClick, bool hiRes);
		GUIButton(int source, int* textureIDNormal, int* textureIDHover, int* textureIDClick, float * xOffsets, float * yOffsets, float * zOffsets, int numIDs, bool hiRes = false, std::string text = "", int textFontNormal = 0,int texttFontHover = 0, int textFontDisabled = 0);
		
		void NextGUI();
		void SetSize(float W, float H, float textW, float textH, int index = 0);
		bool Update(float delta, float mouseX, float mouseY, int lmbuttonState, GUIButton * InitialClick);
		void SetVisibility(bool val);
		void SetPosition(float x, float y, float z = 0);
		Object2D* m_Object[8];
		ButtonInfo m_TexIDs[8];
		Font* m_FontObjects[3];
		float m_CenterPosX = 0;
		float m_CenterPosY = 0;

		int m_NumObjects = 0;
		int m_GuiIndex = 0;

		bool m_IsHiRes = false;
		bool m_IsDisabled = false;

		float m_AnimTime = 0;
		int m_CurrentAnimIndex = 0;
		bool m_Reversing = false;
		bool m_IsVisible = false;
	};
};
