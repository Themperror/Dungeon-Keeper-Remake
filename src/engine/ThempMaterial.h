#pragma once
#include <d3d11.h>
#include <string>

#define MAX_TEXTURES 4

namespace Themp
{
	struct Texture
	{
		ID3D11Resource* m_Resource = nullptr;
		ID3D11ShaderResourceView* m_View = nullptr;
		ID3D11SamplerState* m_SamplerState = nullptr;
		ID3D11Texture2D* m_Texture2D = nullptr;
		char* m_Data = nullptr;
		int m_Width = 0, m_Height = 0;
		~Texture()
		{
			if (m_Resource)
			{
				m_Resource->Release();
				m_Resource = nullptr;
			}
			if (m_View)
			{
				m_View->Release();
				m_View = nullptr;
			}
			if (m_Texture2D)
			{
				m_Texture2D->Release();
				m_Texture2D = nullptr;
			}
			m_SamplerState = nullptr;
			
			if (m_Data)delete m_Data, m_Data = nullptr;
		}
		
		void Create(int width, int height, DXGI_FORMAT format, bool keepCPUTexture = true, void* data = nullptr);
		void Load(void* data, int size);
	};
	class Material
	{
	public:
		~Material();
		Material();
		ID3D11VertexShader* m_VertexShader;
		ID3D11PixelShader* m_PixelShader;
		ID3D11GeometryShader* m_GeometryShader;
		ID3D11InputLayout* m_InputLayout;

		ID3D11ShaderResourceView* m_Views[MAX_TEXTURES];
		ID3D11SamplerState* m_SamplerStates[MAX_TEXTURES];
		Themp::Texture* m_Textures[MAX_TEXTURES];
		//TextureType* textureTypes;
		uint32_t* textureSlots = nullptr;
		uint32_t numTextures = 0;

		void ReadTexture(std::string path);
		void ReadTextures(std::vector<std::string>& textures, std::vector<uint8_t>& textureTypes);
		void SetTexture(Texture * tex, uint32_t TextureIndex = 0);
	};
}