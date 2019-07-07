#pragma once
#include <unordered_map>
#include <d3d11.h>
#include "ThempD3D.h"

#define BASE_TEXTURE_PATH "../data/textures/"
#define BASE_SHADER_PATH "../data/shaders/"
#define BASE_MODEL_PATH "../data/models/"
#define BASE_MATERIAL_PATH "../data/materials/"


namespace Themp
{
	struct Vertex;
	struct Texture;
	class Object3D;
	class Mesh;
	class Material;
	class Resources
	{
	public:
		struct Buffer
		{
			void InitBuf(int data_size, int structSize, UINT bindFlags, D3D11_CPU_ACCESS_FLAG cpuType = (D3D11_CPU_ACCESS_FLAG)0, void* initData = nullptr)
			{
				D3D11_BUFFER_DESC desc{ 0 };
				desc.ByteWidth = data_size;
				desc.StructureByteStride = structSize;
				desc.BindFlags = bindFlags;
				desc.Usage = D3D11_USAGE_DEFAULT;
				desc.MiscFlags = (bindFlags & D3D11_BIND_UNORDERED_ACCESS || bindFlags & D3D11_BIND_SHADER_RESOURCE) ? D3D11_RESOURCE_MISC_BUFFER_STRUCTURED : 0;
				desc.CPUAccessFlags = cpuType;
				if (initData != nullptr)
				{
					D3D11_SUBRESOURCE_DATA data;
					data.pSysMem = initData;
					Themp::D3D::s_D3D->m_Device->CreateBuffer(&desc, &data, &buf);
				}
				else
				{
					Themp::D3D::s_D3D->m_Device->CreateBuffer(&desc, nullptr, &buf);
				}

			}
			void InitUAV()
			{
				if (!buf)
				{
					Themp::System::Print("Can't Init UAV, Buf wasn't initialized yet!");
					return;
				}

				D3D11_BUFFER_DESC bufDesc{ 0 };
				buf->GetDesc(&bufDesc);

				D3D11_UNORDERED_ACCESS_VIEW_DESC UAVdesc;
				ZeroMemory(&UAVdesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
				UAVdesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
				UAVdesc.Buffer.FirstElement = 0;
				UAVdesc.Format = DXGI_FORMAT_UNKNOWN;
				UAVdesc.Buffer.NumElements = bufDesc.ByteWidth / bufDesc.StructureByteStride;
				Themp::D3D::s_D3D->m_Device->CreateUnorderedAccessView(buf, &UAVdesc, &uav);
			}
			void InitSRV()
			{
				if (!buf)
				{
					Themp::System::Print("Can't Init SRV, Buf wasn't initialized yet!");
					return;
				}

				D3D11_BUFFER_DESC bufDesc{ 0 };
				buf->GetDesc(&bufDesc);

				D3D11_SHADER_RESOURCE_VIEW_DESC SRVdesc;
				ZeroMemory(&SRVdesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

				SRVdesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
				SRVdesc.Format = DXGI_FORMAT_UNKNOWN;
				SRVdesc.BufferEx.FirstElement = 0;
				SRVdesc.BufferEx.NumElements = bufDesc.ByteWidth / bufDesc.StructureByteStride;

				Themp::D3D::s_D3D->m_Device->CreateShaderResourceView(buf, &SRVdesc, &srv);
			}

			ID3D11Buffer* buf = nullptr;
			ID3D11ShaderResourceView* srv = nullptr;
			ID3D11UnorderedAccessView* uav = nullptr;
			size_t numElements = 0;
		};
		Resources();
		~Resources();
		void RemoveObject3D(std::string name, Object3D * m);
		Texture* GetTexture(std::string path,int d3d11_CPU_ACCESS_FLAG = 0);

		//Themp::Material * GetUniqueMaterial(std::string texture, std::string shaderPath);
		Themp::Material * GetUniqueMaterial(std::string texture, std::string shaderPath, D3D11_INPUT_ELEMENT_DESC * IED = nullptr, int IEDNumElements = 0);
		Themp::Material * GetMaterial(std::string materialName, std::string texture, std::string shaderPath, bool geometryShader, D3D11_INPUT_ELEMENT_DESC* nonDefaultIED = nullptr, int numElements = 0, bool multisample = false);
		Themp::Material * GetMaterial(std::string materialName, std::vector<std::string>& textures, std::vector<uint8_t>& textureTypes, std::string shaderPath, bool geometryShader, bool multisample = false);
		
		//uniqueMesh only has effect if the model you are loading was loaded in before (if true it will duplicate the mesh instead of reusing the existing one)
		Object3D* GetModel(std::string path, bool uniqueMesh = false);

		size_t CreateVertexBuffer(Vertex * vertices, size_t numVertices);
		bool EditVertexBuffer(int vertexBuffer, Vertex * vertices, size_t numVertices);
		size_t CreateIndexBuffer(uint32_t * indices, size_t numIndices);

		bool EditIndexBuffer(int indexBuffer, uint32_t * indices, size_t numIndices);
		
		std::unordered_map<std::string, ID3D11VertexShader*> m_VertexShaders;
		std::unordered_map<std::string, ID3D11PixelShader*> m_PixelShaders;
		std::unordered_map<std::string, ID3D11GeometryShader*> m_GeometryShaders;
		std::unordered_map<std::string, Texture*> m_Textures;
		std::unordered_map<std::string, Themp::Material*> m_Materials;
		std::unordered_map<std::string, Object3D*> m_Models;
		std::vector<Buffer> m_VertexBuffers;
		std::vector<Buffer> m_IndexBuffers;
		std::vector<Mesh*> m_Meshes;
		std::vector<Object3D*> m_3DObjects;
		ID3D10Blob* ReadToBlob(std::string path);
		ID3D11VertexShader* GetVertexShader(std::string name);
		ID3D11PixelShader* GetPixelShader(std::string name);
		ID3D11GeometryShader * GetGeometryShader(std::string name);

		static Resources* TRes;

	private:
		//shaderpath will detect shaders in a named pattern like "path_%type%" so as shader_VS, shader_PS, shader_GS
		Themp::Material* LoadMaterial(std::string materialName, std::string texture, std::string shaderPath, bool geometryShader, D3D11_INPUT_ELEMENT_DESC* nonDefaultIED = nullptr, int numElements = 0, bool multisample = false);
		//shaderpath will detect shaders in a named pattern like "path_%type%" so as shader_VS, shader_PS, shader_GS
		Themp::Material * LoadMaterial(std::string materialName, std::vector<std::string>& textures, std::vector<uint8_t>& textureTypes, std::string shaderPath, bool geometryShader, bool multisample = false);
		size_t currentUniqueMatIndex = 0;
		Object3D* LoadModel(std::string name);
	};
};