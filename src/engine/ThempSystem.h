#pragma once

#include <unordered_map>
#include <string>

#pragma warning( disable : 4996) //disables warning unsafe function: freopen() fopen() .. etc

constexpr auto PI = 3.14159265359f;
#define ReportLiveObjects (0)
#define LOG_MISSING_MATERIALS (0)
#define CLEAN(x) if(x){x->Release();x=nullptr;}


#define SVAR_WINDOWWIDTH "WindowSizeX"
#define SVAR_WINDOWHEIGHT "WindowSizeY"
#define SVAR_FULLSCREEN "Fullscreen"
#define SVAR_WINDOWPOSX "WindowPosX"
#define SVAR_WINDOWPOSY "WindowPosY"
#define SVAR_ANISOTROPIC_FILTERING "Anisotropic_Filtering"
#define SVAR_MULTISAMPLE "Multisample"

//The original game has a fluctuating turns per second depending on FPS, but the target is 20 turns, as we have the ability to use delta times we can work with this.
#define GAME_TURNS_PER_SECOND (20.0f)
#define GAME_TURNS_TO_SECOND(x) (((float)x) / GAME_TURNS_PER_SECOND)
#define SECOND_TO_GAME_TURNS(x) ((x) * GAME_TURNS_PER_SECOND)

namespace Themp
{
	float lerp(float x, float y, float t);
	float mod(float val, float mod);
	class Game;
	class D3D;
	class Resources;
	class GUI;
	class Audio;
	class System
	{
	public:
		static Themp::System* tSys;
		System() {}; 
		void Start();
		void Interrupt() {}; // Alt tab, lost focus etc...

		std::string m_BaseDir;
		bool m_Quitting = false;
		bool m_CursorShown = true;
		std::unordered_map<std::string, float> m_SVars;
		Themp::Game* m_Game = nullptr;
		Themp::D3D* m_D3D = nullptr;
		Themp::Resources* m_Resources = nullptr;
		Themp::GUI* m_GUI = nullptr;
		Themp::Audio* m_Audio = nullptr;
	};
};