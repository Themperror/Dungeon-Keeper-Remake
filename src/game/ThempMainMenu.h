#pragma once
#include <vector>
namespace Themp
{
	class Object3D;
	class Object2D;
	class GUIButton;
	class Camera;
	class Video;
	class MainMenu
	{
	public:
		MainMenu() {};
		void Start();
		void Update(double dt);
		void Stop();
		enum class MenuState {Splash,Intro,Loading,Menu,CampaignSelect, LoadGame,Options,HighScore};
		MenuState m_State = MenuState::Splash;
		Object3D* m_VideoObject = nullptr;
		Object2D* m_LoadingScreenTexture = nullptr; //Contains Texture / sprite data
		Object2D* m_TortureTexture = nullptr;
		Object2D* m_MenuBackgroundTexture = nullptr;
		Object2D* m_SplashScreenTexture = nullptr;
		Object2D* m_MapFiles[22];
		GUIButton* m_GUIButtons[7];
		Video* m_Video = nullptr;
		bool m_IsDone = false;
		bool m_DoPlayVideo = true;
		bool m_IsLoaded = false;
	};
};
