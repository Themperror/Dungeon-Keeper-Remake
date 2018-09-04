#pragma once
#include <d3d11.h>
struct ImDrawData;
namespace Themp
{
	class Material;
	class GUI
	{
		struct GUIConstantBufferData
		{
			float mvp[4][4];
		};
		struct GUIVertex
		{
			float x, y, u, v;
			uint8_t r, g, b, a;
		};
	public:
		GUI(HWND handle);
		~GUI();
		void Init();
		void PrepareDraw(ImDrawData* draw_data);
		void Draw(ImDrawData* draw_data);
		void EndDraw();
		void CreateFontsTexture();
		static GUI* gui;
	private:
		Material* m_Material = nullptr;
		ID3D11Buffer* m_GUIConstantBuffer = nullptr;
		ID3D11BlendState* m_BlendState = nullptr;
		ID3D11RasterizerState* m_RasterizerState = nullptr;
		ID3D11SamplerState* m_SamplerState = nullptr;
		ID3D11DepthStencilState* m_DepthStencilState = nullptr;
		ID3D11ShaderResourceView* m_ShaderResourceView = nullptr;
		ID3D11Buffer* m_VertexBuffer = nullptr;
		ID3D11Buffer* m_IndexBuffer = nullptr;
		int m_VertexBufferSize = 0, m_IndexBufferSize = 0;
		HWND m_WindowHandle = 0;

	};
};