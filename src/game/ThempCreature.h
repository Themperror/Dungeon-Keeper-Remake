#pragma once
#include <vector>
namespace Themp
{
	class D3D;
	class Object3D;
	struct Sprite;
	class Creature
	{
	public:
		~Creature();
		Creature(int spriteIndex);
		void NextFrame();
		void SetVisibility(bool val);

		Object3D* m_Renderable = nullptr;
		Sprite* m_Sprite = nullptr;

		int currentAnim = 0;
	};
};
