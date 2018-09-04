#pragma once

#include <d3d11.h>
namespace Themp
{
	class RenderTexture
	{
	public:
		enum TextureType{RenderTex, DepthTex,RenderTexArray,DepthTexArray};
		
		RenderTexture(int width, int height, TextureType type, int multisample, int numTextures = 1, DXGI_FORMAT depthFormat = DXGI_FORMAT_R32_TYPELESS);
		~RenderTexture();

		int m_ResolutionX = 0;
		int m_ResolutionY = 0;
		TextureType m_TextureType;
		int m_Multisample = 1;
		ID3D11RenderTargetView* m_RenderTarget = nullptr;
		ID3D11DepthStencilView* m_DepthStencilView = nullptr;
		ID3D11ShaderResourceView* m_ShaderResourceView;
		ID3D11Texture2D* m_Texture;
	};
};