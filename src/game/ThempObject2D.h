#pragma once
#include <vector>
namespace Themp
{
	class Object3D;
	struct Texture;
	class Object2D
	{
	public:
		~Object2D();
		Object2D(std::wstring path, bool PALWithRaw);
		Texture* m_Tex = nullptr;
	};
};
