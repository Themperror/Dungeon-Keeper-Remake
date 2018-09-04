#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
namespace Themp
{
	using namespace DirectX;
	class Material;

	class DebugDraw
	{
		struct LineData
		{
			XMFLOAT3 x1, x2;
			XMFLOAT3 color;
			float time;
		};
	public:
		static void Destroy();
		static void Line(XMFLOAT3 p1, XMFLOAT3 p2, float time = 0.0f, XMFLOAT3 color = XMFLOAT3(1, 1, 1));
		static void Update(float dt);
		static void Draw(ID3D11Device* dev, ID3D11DeviceContext* devcon);

		static D3D11_INPUT_ELEMENT_DESC DefaultLineInputLayoutDesc[];
		static std::vector<LineData> lines;
		static Material* DefaultLineMaterial;
		static ID3D11Buffer* vertexBuffer;
	};
}