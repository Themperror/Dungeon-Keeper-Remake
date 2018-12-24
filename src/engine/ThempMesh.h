#pragma once
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
namespace Themp
{
	struct Vertex
	{
		float x, y, z;
		float nx, ny, nz;
		float u, v;
	};
	struct VoxelVertex
	{
		float x, y, z;
		float nx, ny, nz;
		float u, v, visible;
		uint32_t doAnimate;
	};
	class Material;
	class Mesh
	{
	public:
		Mesh();
		~Mesh();
		enum DrawPass{GBUFFER,SHADOW,LIGHT};
		void ConstructVertexBuffer();
		void Draw(Themp::D3D& d3d);
		Material* m_Material;
		ID3D11Buffer* m_ConstantBuffer = nullptr;
		Vertex* m_Vertices;
		uint32_t m_NumVertices;
		uint32_t* m_Indices;
		uint32_t m_NumIndices;
		size_t i_VertexBuffer; //index to the vertexbuffer in resourcemanager
		size_t i_IndexBuffer; // index to the indexbuffer in resourcemanager
		DirectX::XMFLOAT3 m_BoundsMin;
		DirectX::XMFLOAT3 m_BoundsMax;


		//do not change manually
		ID3D11Buffer* m_VertexBuffer;
		size_t m_VertexSize = 0;
		//do not change manually
		ID3D11Buffer* m_IndexBuffer;
	};
}