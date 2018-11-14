#include "ThempSystem.h"
#include "ThempMesh.h"
#include "ThempD3D.h"
#include "ThempResources.h"
#include "ThempMaterial.h"
#include <d3d10.h>
#include <d3d11.h>

using namespace DirectX;
namespace Themp
{
	Mesh::Mesh()
	{
		m_BoundsMin = XMFLOAT3(0, 0, 0);
		m_BoundsMax = XMFLOAT3(0, 0, 0);
		m_VertexSize = sizeof(Vertex);
	}

	Mesh::~Mesh()
	{
		if (m_Vertices)
		{
			delete[] m_Vertices;
			m_Vertices = nullptr;
		}
		if (m_Indices)
		{
			delete[] m_Indices;
			m_Indices = nullptr;
		}
		m_Material = nullptr;
		m_VertexBuffer = nullptr;
		m_IndexBuffer = nullptr;
	}
	void Mesh::ConstructVertexBuffer()
	{
		i_VertexBuffer = Themp::Resources::TRes->CreateVertexBuffer(m_Vertices, m_NumVertices);
		i_IndexBuffer = Themp::Resources::TRes->CreateIndexBuffer(m_Indices, m_NumIndices);
		m_VertexBuffer = Themp::Resources::TRes->m_VertexBuffers[i_VertexBuffer].buffer;
		m_IndexBuffer = Themp::Resources::TRes->m_IndexBuffers[i_VertexBuffer].buffer;

		//m_BoundsMin = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
		//m_BoundsMax = XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		//for (size_t i = 0; i < m_NumVertices; i++)
		//{
		//	Vertex& v = m_Vertices[i];
		//	if (v.x < m_BoundsMin.x) m_BoundsMin.x = v.x;
		//	if (v.x > m_BoundsMax.x) m_BoundsMax.x = v.x;
		//	if (v.y < m_BoundsMin.y) m_BoundsMin.y = v.y;
		//	if (v.y > m_BoundsMax.y) m_BoundsMax.y = v.y;
		//	if (v.z < m_BoundsMin.z) m_BoundsMin.z = v.z;
		//	if (v.z > m_BoundsMax.z) m_BoundsMax.z = v.z;
		//}
	}
	
	void Mesh::Draw(Themp::D3D& d3d)
	{
		uint32_t stride[] = { (uint32_t)m_VertexSize };
		uint32_t offset[] = { 0 };
		//set vertex/index buffers
		d3d.m_DevCon->IASetVertexBuffers(0, 1, &m_VertexBuffer, stride, offset);
		d3d.m_DevCon->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		d3d.m_DevCon->IASetInputLayout(m_Material->m_InputLayout);

		//set resources for shader pipeline
		
		d3d.m_DevCon->PSSetSamplers(0, 4, m_Material->m_SamplerStates);
		d3d.m_DevCon->PSSetShaderResources(0, 4, m_Material->m_Views);
		d3d.m_DevCon->VSSetShader(m_Material->m_VertexShader, 0, 0);
		d3d.m_DevCon->PSSetShader(m_Material->m_PixelShader, 0, 0);
		d3d.m_DevCon->GSSetShader(m_Material->m_GeometryShader, 0, 0);

		d3d.SetMaterialConstantBuffer(m_ConstantBuffer);
		d3d.PSUploadConstantBuffersToGPU();

		d3d.m_DevCon->DrawIndexed(m_NumIndices, 0, 0);
	}
}