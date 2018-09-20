#pragma once
#include <vector>
namespace Themp
{
	class Object3D;
	class Object2D;
	class GUIButton;
	class Camera;
	class Video;
	class Font;
	class MainMenu
	{
	public:
		MainMenu() {};
		void Start();
		void Update(double dt);
		void GoToSplash();
		void GoToCampaignSelect();
		void GoToContinue();
		void GoToLoadGame();
		void GoToMultiplayer();
		void GoToHighScore();
		void GoToOptions();
		void GoToLoading();
		void GoToIntro();
		void GoToMenu();
		void SetEverythingHidden();
		void Stop();
		enum class MenuState {Splash,Intro,Loading,Menu,CampaignSelect, LoadGame,Options,HighScore};
		MenuState m_State = MenuState::Splash;
		Object3D* m_VideoObject = nullptr;
		Object2D* m_LoadingScreenTexture = nullptr; //Contains Texture / sprite data
		Object2D* m_TortureTexture = nullptr;
		Object2D* m_MenuBackgroundTexture = nullptr;
		Object2D* m_SplashScreenTexture = nullptr;
		Object2D* m_MainMenuBar[3] = { nullptr,nullptr,nullptr };
		Object2D* m_Cursor = nullptr;
		Object2D* m_MapFiles[22];
		Object2D* m_MapWindowFiles[22];
		GUIButton* m_GUIButtons[7];
		GUIButton* m_LevelSelectFlag = nullptr;
		Font* m_TextObject;
		Video* m_Video = nullptr;
		bool m_IsDone = false;
		bool m_DoPlayVideo = true;
		bool m_IsLoaded = false;
		int m_CurrentLevel = 0;
	};
};
