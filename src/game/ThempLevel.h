#pragma once
#include <vector>
namespace Themp
{
	class D3D;
	class Object3D;
	class Object2D;
	struct Sprite;
	class Level
	{
	public:
		~Level();
		Level(int levelIndex);
		void Update(float delta);
		Object2D* m_Cursor;

	};
};
