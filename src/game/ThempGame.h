#pragma once
#include <vector>
namespace Themp
{
	class Object3D;
	class Creature;
	class Entity;
	class Camera;
	class MainMenu;
	class Level;
	class FileManager;
	//class Resources;
	//struct Texture;
	class Game
	{
	public:
		Game() {};
		void Start();
		void Update(double dt);
		void LoadLevel(int levelIndex);
		void Stop();
		void AddObject3D(Object3D* obj);
		void AddCreature(Creature* obj);
		void RemoveCreature(Creature * creature);
		void AddEntity(Entity * entity);
		static void TranslateMousePos(int inX, int inY, float & outX, float & outY);
		std::vector<Themp::Object3D*> m_Objects3D;
		std::vector<Themp::Creature*> m_Creatures;
		std::vector<Themp::Entity*> m_Entities;
		Camera* m_Camera = nullptr;
		FileManager* m_FileManager;
		MainMenu* m_MainMenu = nullptr;
		Level* m_CurrentLevel = nullptr;

		signed char m_Keys[258];//Keeping track of input;
		POINT m_CursorPos;
		float m_CursorWindowedX, m_CursorWindowedY;
		float m_CursorDeltaX, m_CursorDeltaY;
	};
};
