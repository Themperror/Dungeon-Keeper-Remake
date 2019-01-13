#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
namespace Themp
{
	using namespace DirectX;
	class Mesh;
	class D3D;
	class Material;
	class Object3D
	{
	public:
		struct Object3DConstantBufferData
		{
			XMFLOAT4X4 worldMatrix;
			uint32_t misc0, misc1, misc2, misc3;
		};
		Object3D();
		~Object3D();
		void Update(float dt);
		void SetMaterial(Material* m, int MeshIndex = 0);
		//pass is a enum Themp::Mesh::DrawPass type;
		void Draw(D3D& d3d);
		void ForceBufferUpdate();
		void CreateCube(std::string shader,  bool geometryShader);
		void CreateTriangle(std::string shader, bool geometryShader);
		void CreateQuad(std::string shader, bool geometryShader);

		//Only need to call this when you're manually building a mesh, this function builds all vertex and index buffers for you
		void Construct();

		std::vector<Themp::Mesh*> m_Meshes;
		XMFLOAT3 m_Position, m_Rotation, m_Scale;

		void SetPosition(float x, float y, float z);
		void SetPosition(XMFLOAT3& pos);
		void SetScale(float x, float y, float z);
		void SetScale(XMFLOAT3& scale);
		void SetRotation(float x, float y, float z);
		void SetRotation(XMFLOAT3& rot);

		//axis aligned
		XMFLOAT3 m_BoundsMin, m_BoundsMax;
		ID3D11Buffer* m_ConstantBuffer = nullptr;
		Object3DConstantBufferData m_ConstantBufferData;
		bool isDirty = true;
		bool isVisible = true;
		bool m_BackfaceCull = true;
	};
}