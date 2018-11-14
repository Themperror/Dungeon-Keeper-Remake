#include "ThempSystem.h"
#include "ThempMaterial.h"
#include "ThempD3D.h"
#include "ThempResources.h"
#include <d3d10.h>
#include <istream>
#include <fstream>
#include <iostream>
#include <sstream>
namespace Themp
{
	Material::Material()
	{
		numTextures = 0;
		for (size_t i = 0; i < MAX_TEXTURES; i++)
		{
			m_Textures[i] = nullptr;
			m_SamplerStates[i] = nullptr;
			m_Views[i] = nullptr;
		}
	}

	Material::~Material()
	{
		for (size_t i = 0; i < MAX_TEXTURES; i++)
		{
			m_SamplerStates[i] = nullptr;
			m_Views[i] = nullptr;
		}
		if (m_InputLayout)
		{
			m_InputLayout->Release();
			m_InputLayout = nullptr;
		}
		if (m_GeometryShader)
		{
			m_GeometryShader = nullptr;
		}
		if (m_VertexShader)
		{
			m_VertexShader = nullptr;
		}
		if (m_PixelShader)
		{
			m_PixelShader = nullptr;
		}
		if(textureSlots)
			delete[] textureSlots;
	}
	void Material::ReadTexture(std::string path)
	{
		std::vector<std::string> defaultTextures = {
			"",
			"",
			"",
			"",
		};
		std::vector<uint8_t> defaultTypes = { 1,((uint8_t)(-1)),((uint8_t)(-1)),((uint8_t)(-1)) };
		defaultTextures[0] = path;
		ReadTextures(defaultTextures, defaultTypes);
		this->numTextures = 1;
		
	}
	void Material::ReadTextures(std::vector<std::string>& textures, std::vector<uint8_t>& textureTypes)
	{
		numTextures = (uint32_t)textures.size();
		if (textures.size() > MAX_TEXTURES)
		{
			System::Print("Found more than %i textures for a material, this is not supported yet!", MAX_TEXTURES);
			numTextures = MAX_TEXTURES;
		}
		std::string defaultTextures[4] = {
			"",
			"",
			"",
			"",
		};

		textureSlots = new uint32_t[MAX_TEXTURES];
		
		for (size_t i = 0; i < MAX_TEXTURES; i++)
		{
			//CAN RETURN NULLPTR, This is valid as not all texture slots might be used!!
			Themp::Texture* tex = Resources::TRes->GetTexture(defaultTextures[i],0);

			//this is so I can map all MAX_TEXTURES states and views through a single call
			if (tex != nullptr)
			{
				m_Views[i] = tex->m_View;
				tex->m_SamplerState = D3D::DefaultTextureSampler;
				m_SamplerStates[i] = tex->m_SamplerState;
			}
			else
			{
				m_Views[i] = nullptr;
				m_SamplerStates[i] = D3D::DefaultTextureSampler;
			}
			m_Textures[i] = tex;

		}
	}
	void Material::SetTexture(Texture * tex, uint32_t TextureIndex)
	{
		if (TextureIndex < MAX_TEXTURES)
		{
			m_Views[TextureIndex] = tex->m_View;
			m_Textures[TextureIndex] = tex;
			numTextures = TextureIndex+1 > numTextures ? TextureIndex+1 : numTextures;
		}
	}
	//void Material::UpdateBuffer()
	//{
	//	if (!m_MaterialConstantBuffer)
	//	{
	//		// Fill in a buffer description.
	//		D3D11_BUFFER_DESC cbDesc;
	//		cbDesc.ByteWidth = sizeof(MaterialConstantBuffer);
	//		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	//		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//		cbDesc.MiscFlags = 0;
	//		cbDesc.StructureByteStride = 0;
	//
	//		// Fill in the subresource data.
	//		D3D11_SUBRESOURCE_DATA InitData;
	//		InitData.pSysMem = &m_MaterialConstantBufferData;
	//		InitData.SysMemPitch = 0;
	//		InitData.SysMemSlicePitch = 0;
	//
	//		// Create the buffer.
	//		Themp::System::tSys->m_D3D->m_Device->CreateBuffer(&cbDesc, &InitData, &m_MaterialConstantBuffer);
	//	}
	//	else
	//	{
	//		D3D11_MAPPED_SUBRESOURCE ms;
	//		Themp::System::tSys->m_D3D->m_DevCon->Map(m_MaterialConstantBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	//		memcpy(ms.pData, &m_MaterialConstantBufferData, sizeof(MaterialConstantBuffer));
	//		Themp::System::tSys->m_D3D->m_DevCon->Unmap(m_MaterialConstantBuffer, NULL);
	//	}
	//}
	void Texture::Create(int width, int height, DXGI_FORMAT format, bool keepCPUTexture, void* data)
	{
		m_Width = width;
		m_Height = height;
		if (keepCPUTexture)
		{
			m_Data = new char[width*height * 4];//BUG: will not support formats more than 32 bits per pixel, less SHOULD be ok
			if (data)
			{
				memcpy(m_Data, data, width*height * 4);
			}
		}

		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = format;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

	
		D3D11_SUBRESOURCE_DATA subResource;
		subResource.pSysMem = keepCPUTexture ? m_Data : data;
		subResource.SysMemPitch = desc.Width * 4;
		subResource.SysMemSlicePitch = 0;
		HRESULT res = Themp::System::tSys->m_D3D->m_Device->CreateTexture2D(&desc, &subResource, &m_Texture2D);

		// Create texture view
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		if (m_Texture2D != nullptr)
		{
			res = Themp::System::tSys->m_D3D->m_Device->CreateShaderResourceView(m_Texture2D, &srvDesc, &m_View);
			if (!keepCPUTexture)
			{
				m_Texture2D->Release();
				m_Texture2D = nullptr;
			}
		}
		else
		{
			System::Print("Could not load texture");
		}
	}
	void Texture::Load(void* data, int size)
	{
		assert(m_Data && size <= m_Width * m_Height * 4);
		if (m_Data && size <= m_Width*m_Height*4)
		{
			memcpy(m_Data, data, size);
			D3D11_MAPPED_SUBRESOURCE ms{ 0 };
			
			Themp::System::tSys->m_D3D->m_DevCon->Map(m_Texture2D, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &ms);
			memcpy(ms.pData, m_Data, size);
			Themp::System::tSys->m_D3D->m_DevCon->Unmap(m_Texture2D, 0);
		}
	}
}