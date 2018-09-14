#include "ThempSystem.h"
#include "ThempDebugDraw.h"
#include "ThempD3D.h"
#include "ThempRenderTexture.h"
#include "ThempMaterial.h"
namespace Themp
{
	D3D11_INPUT_ELEMENT_DESC DebugDraw::DefaultLineInputLayoutDesc[] =
	{
		{ "POSITION" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	std::vector<DebugDraw::LineData> DebugDraw::lines;
	Material* DebugDraw::DefaultLineMaterial = nullptr;
	ID3D11Buffer* DebugDraw::vertexBuffer = nullptr;

	struct DebugLineVertex
	{
		XMFLOAT3 p, c;
	};
	std::vector<DebugLineVertex> lineVertices;

	void DebugDraw::Destroy()
	{
		CLEAN(vertexBuffer);
	}
	void DebugDraw::Line(XMFLOAT3 p1, XMFLOAT3 p2, float time, XMFLOAT3 color)
	{
#ifdef _DEBUG
		lines.push_back({ { p1.x,p1.y,p1.z },{ p2.x,p2.y,p2.z },{ color.x,color.y,color.z },time });
#endif
	}
	void DebugDraw::Update(float dt)
	{
#ifdef _DEBUG
		for (int i = lines.size()-1; i >= 0; i--)
		{
			if (lines[i].time > 0.0f)
			{
				lines[i].time -= dt;
				if (lines[i].time < 0.0)
				{
					lines.erase(lines.begin() + i);
				}
			}
		}
#endif
	}
	int LineBufferSize = 0;
	void DebugDraw::Draw(ID3D11Device* dev, ID3D11DeviceContext* devcon)
	{
#ifdef _DEBUG
		//if theres no lines to draw, skip everything
		if (lines.size() == 0) return;
		//if our buffer size is smaller than the number of vertices we'll put in, destroy the old buffer, create a bigger one.
		if (LineBufferSize <= lines.size() * 2 * sizeof(DebugLineVertex))
		{
			CLEAN(vertexBuffer);

			//create a buffer twice as big as what we're drawing;
			LineBufferSize = lines.size() * 2 * sizeof(DebugLineVertex) * 2;

			//create the line vertex buffer
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

			//set up for vertices
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(DebugLineVertex) * lines.size() * 2 * 2;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			HRESULT res = dev->CreateBuffer(&bd, NULL, &vertexBuffer);
		}

		D3D11_MAPPED_SUBRESOURCE ms;
		ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));

		//copy all the data
		lineVertices.clear();
		lineVertices.reserve(lines.size() * 2);
		for (int i = lines.size() - 1; i >= 0; i--)
		{
			lineVertices.push_back({ lines[i].x1,lines[i].color });
			lineVertices.push_back({ lines[i].x2,lines[i].color });
			if (lines[i].time == 0.0f)
			{
				lines.erase(lines.begin() + i);
			}
		}

		devcon->Map(vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, lineVertices.data(), sizeof(DebugLineVertex) * lineVertices.size());
		devcon->Unmap(vertexBuffer, NULL);

		//start preparing to draw our lines
		devcon->RSSetState(D3D::s_D3D->m_RasterizerState);
		devcon->IASetInputLayout(DefaultLineMaterial->m_InputLayout);
		devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		devcon->OMSetDepthStencilState(D3D::s_D3D->m_DepthStencilState, 1);
		devcon->OMSetRenderTargets(1, &D3D::s_D3D->m_BackBuffer, D3D::s_D3D->m_DepthStencilView);

		devcon->PSSetShaderResources(0, 0, nullptr);
		devcon->PSSetShader(DefaultLineMaterial->m_PixelShader, 0, 0);
		devcon->VSSetShader(DefaultLineMaterial->m_VertexShader, 0, 0);
		devcon->GSSetShader(nullptr, 0, 0);

		const uint32_t stride[] = { sizeof(DebugLineVertex) };
		const uint32_t offset[] = { 0 };
		devcon->IASetVertexBuffers(0, 1, &vertexBuffer, stride, offset);
		devcon->Draw(lineVertices.size(), 0);

#endif
	}
}