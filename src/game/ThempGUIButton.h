#pragma once
#include <vector>
#include <string>
namespace Themp
{
	class Object2D;
	struct Texture;
	class GUIButton
	{
		struct ButtonInfo
		{
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
		GUIButton(int source, int * textureIDNormal, int * textureIDHover, int * textureIDClick, float * xOffsets, float * yOffsets, float * zOffsets, int numIDs, bool hiRes);
		void NextGUI();
		//void SetSize(int W, int H, int screenWidth, int screenHeight);
		void SetSize(float W, float H, int index = 0);
		bool Update(float mouseX, float mouseY, int lmbuttonState, GUIButton * InitialClick);
		void SetVisibility(bool val);
		void SetPosition(float x, float y, float z = 0);
		Object2D* m_Object[8];
		ButtonInfo m_TexIDs[8];

		float m_CenterPosX = 0;
		float m_CenterPosY = 0;

		int numObjects = 0;
		int guiIndex = 0;

		bool isHiRes = false;
	};
};
