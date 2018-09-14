#pragma once
#include <vector>
namespace Themp
{
	class Object3D;
	class Camera;
	class MainMenu;
	class FileManager;
	//class Resources;
	//struct Texture;
	class Game
	{
	public:
		Game() {};
		void Start();
		void Update(double dt);
		void Stop();
		void AddObject3D(Object3D* obj);
		std::vector<Themp::Object3D*> m_Objects3D;
		Camera* m_Camera = nullptr;
		FileManager* m_FileManager;
		MainMenu* m_MainMenu = nullptr;

		signed char m_Keys[258];//Keeping track of input;
		POINT m_CursorPos;
		float m_CursorDeltaX, m_CursorDeltaY;
	};
};
