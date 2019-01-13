#include "ThempSystem.h"
#include "ThempRenderTexture.h"
#include "ThempD3D.h"
namespace Themp
{
	RenderTexture::RenderTexture(int width, int height, TextureType type, int multisample,int numTextures, DXGI_FORMAT depthFormat)
	{
		m_TextureType = type;
		m_ResolutionX = width;
		m_ResolutionY = height;
		ID3D11Device* m_Device = Themp::System::tSys->m_D3D->m_Device;
		if (multisample == 0) multisample = 1;
		m_Multisample = multisample;
		if (type == TextureType::RenderTex || type == TextureType::RenderTexArray)
		{
			HRESULT result;
			D3D11_TEXTURE2D_DESC renderTexDesc;
			memset(&renderTexDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));
			renderTexDesc.Usage = D3D11_USAGE_DEFAULT;
			renderTexDesc.ArraySize = type != RenderTexArray ? 1 : numTextures;
			renderTexDesc.MipLevels = 1;
			renderTexDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
			renderTexDesc.CPUAccessFlags = 0;
			renderTexDesc.SampleDesc.Count = multisample;
			renderTexDesc.SampleDesc.Quality = 0;
			renderTexDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
			renderTexDesc.Height = height;
			renderTexDesc.Width = width;

			result = m_Device->CreateTexture2D(&renderTexDesc, nullptr, &m_Texture);

			if (result != S_OK) { System::Print("Could not CreateTexture2D"); return; }
			// use the back buffer address to create the render target
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			memset(&srvDesc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
			srvDesc.Format = renderTexDesc.Format;
			srvDesc.Texture2D.MipLevels = 1;
			srvDesc.ViewDimension = type != RenderTexArray ? (multisample == 1 ? D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D : D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2DMS) : (multisample == 1 ? D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2DARRAY : D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY);

			result = m_Device->CreateShaderResourceView(m_Texture, &srvDesc, &m_ShaderResourceView);
			if (result != S_OK) { System::Print("Could not CreateShaderResourceView"); return; }

			result = m_Device->CreateRenderTargetView(m_Texture, nullptr, &m_RenderTarget);
			if (result != S_OK) { System::Print("Could not CreateRenderTargetView"); return; }

		}
		else if (type == TextureType::DepthTex || type == TextureType::DepthTexArray)
		{
			HRESULT result;

			D3D11_TEXTURE2D_DESC depthBufferDesc;
			D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
			D3D11_SHADER_RESOURCE_VIEW_DESC depthSRVDesc;
			memset(&depthBufferDesc, 0, sizeof(D3D11_TEXTURE2D_DESC));
			memset(&descDSV, 0, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
			memset(&depthSRVDesc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

			depthBufferDesc.SampleDesc.Count = multisample;
			depthBufferDesc.SampleDesc.Quality = 0;

			depthBufferDesc.Width = width;
			depthBufferDesc.Height = height;
			depthBufferDesc.MipLevels = 1;

			depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

			depthBufferDesc.Format = depthFormat; // DXGI_FORMAT_D16_UNORM;
			depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			depthBufferDesc.CPUAccessFlags = 0;
			
			depthBufferDesc.ArraySize = type != DepthTexArray ? 1 : numTextures;
			depthSRVDesc.ViewDimension = type != DepthTexArray ? (multisample == 1 ? D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D : D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2DMS) :(multisample == 1 ? D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2DARRAY : D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY);
			descDSV.ViewDimension = type != DepthTexArray ? (multisample == 1 ? D3D11_DSV_DIMENSION_TEXTURE2D : D3D11_DSV_DIMENSION_TEXTURE2DMS) : (multisample == 1 ? D3D11_DSV_DIMENSION_TEXTURE2DARRAY : D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY);

			
			result = m_Device->CreateTexture2D(&depthBufferDesc, NULL, &m_Texture);
			if (result != S_OK)
			{
				System::Print("Could not create Depthstencil texture");
				return;
			}
			if (depthFormat != DXGI_FORMAT_R32_TYPELESS && depthFormat != DXGI_FORMAT_R32G32_TYPELESS && depthFormat != DXGI_FORMAT_R32G32B32_TYPELESS && depthFormat != DXGI_FORMAT_R32G32B32A32_TYPELESS)
			{
				System::Print("Format needs to be of 32 bit type (with up to 4 channels), 16 bit or different is not supported");
			}
			descDSV.Format = DXGI_FORMAT_D32_FLOAT;
			

			//this stuff is unioned so I'm not entirely sure if this is the best way to do it since the second statement might overwrite a variable from the first
			descDSV.Texture2DArray.ArraySize = m_Multisample == 1 ? type != DepthTexArray ? 0 : numTextures : 0;
			descDSV.Texture2DMSArray.ArraySize = m_Multisample != 1 ? type != DepthTexArray ? 0 : numTextures : 0;

			// Create the depth stencil view
			result = m_Device->CreateDepthStencilView(m_Texture, // Depth stencil texture
				&descDSV, // Depth stencil desc
				&m_DepthStencilView);  // [out] Depth stencil view
			if (result != S_OK)
			{
				System::Print("Could not create Depthstencil view");
				return;
			}

			if (multisample == 1)
			{
				if (type == TextureType::DepthTex)
				{
					D3D11_TEX2D_SRV tex2D;
					tex2D.MipLevels = 1;
					tex2D.MostDetailedMip = 0;
					depthSRVDesc.Texture2D = tex2D;
				}
				else
				{
					D3D11_TEX2D_ARRAY_SRV tex2DArray;
					
					tex2DArray.ArraySize = descDSV.Texture2DArray.ArraySize;
					tex2DArray.MipLevels = 1;
					tex2DArray.FirstArraySlice = 0;
					tex2DArray.MostDetailedMip = 0;
					depthSRVDesc.Texture2DArray = tex2DArray;
				}
			}
			else
			{
				if (type == TextureType::DepthTex)
				{
					D3D11_TEX2D_SRV tex2D;
					tex2D.MipLevels = 1;
					tex2D.MostDetailedMip = 0;
					depthSRVDesc.Texture2D = tex2D;
				}
				else
				{
					depthSRVDesc.Texture2DMSArray.ArraySize = descDSV.Texture2DMSArray.ArraySize;
					depthSRVDesc.Texture2DMSArray.FirstArraySlice = 0;
				}
			}

			depthSRVDesc.Format =	depthFormat == DXGI_FORMAT_R32_TYPELESS ? DXGI_FORMAT_R32_FLOAT : depthFormat == DXGI_FORMAT_R32G32_TYPELESS ? DXGI_FORMAT_R32G32_FLOAT : 
									depthFormat == DXGI_FORMAT_R32G32B32_TYPELESS ? DXGI_FORMAT_R32G32B32_FLOAT : DXGI_FORMAT_R32G32B32A32_FLOAT;

			result = m_Device->CreateShaderResourceView(m_Texture, 	&depthSRVDesc, 	&m_ShaderResourceView);  
			if (result != S_OK)
			{
				System::Print("Could not create Depthstencil shader resource view");
				return;
			}
		}
	}
	RenderTexture::~RenderTexture()
	{
		CLEAN(m_RenderTarget);
		CLEAN(m_ShaderResourceView);
		CLEAN(m_Texture);
		CLEAN(m_DepthStencilView);
	}
}