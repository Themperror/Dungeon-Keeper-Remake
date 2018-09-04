#pragma once
#include <vector>
namespace Themp
{
	class Object3D;
	class Object2D;
	class Camera;
	class Video;
	class MainMenu
	{
	public:
		MainMenu() {};
		void Start();
		void Update(double dt);
		void Stop();
		enum class MenuState {Intro,Loading,Menu,CampaignSelect, LoadGame,Options,HighScore};
		MenuState m_State = MenuState::Intro;
		Object3D* m_VideoObject = nullptr;
		Object3D* m_LoadingScreen = nullptr; // actual geometry to render on
		Object2D* m_LoadingScreenTexture = nullptr; //Contains Texture / sprite data
		Object2D* m_TortureTexture = nullptr;
		Object2D* m_MapFiles[22];
		Video* m_Video = nullptr;
		bool m_IsDone = false;
		bool m_DoPlayVideo = true;
		bool m_IsLoaded = false;
	};
};
