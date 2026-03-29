#pragma once
#include <unordered_map>
#include <d3dfwd.h>
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
			void InitBuf(int data_size, int structSize, uint32_t bindFlags, D3D11_CPU_ACCESS_FLAG cpuType = (D3D11_CPU_ACCESS_FLAG)0, void* initData = nullptr);
			void InitUAV();
			void InitSRV();

			ID3D11Buffer* buf = nullptr;
			ID3D11ShaderResourceView* srv = nullptr;
			ID3D11UnorderedAccessView* uav = nullptr;
			size_t numElements = 0;
		};
		Resources();
		~Resources();
		void RemoveObject3D(const std::string& name, Object3D * m);
		Texture* GetTexture(const std::string& path,int d3d11_CPU_ACCESS_FLAG = 0);

		//Themp::Material * GetUniqueMaterial(std::string texture, std::string shaderPath);
		Themp::Material * GetUniqueMaterial(const std::string& texture, const std::string& shaderPath, D3D11_INPUT_ELEMENT_DESC * IED = nullptr, int IEDNumElements = 0);
		Themp::Material * GetMaterial(const std::string& materialName, const std::string& texture, const std::string& shaderPath, bool geometryShader, D3D11_INPUT_ELEMENT_DESC* nonDefaultIED = nullptr, int numElements = 0, bool multisample = false);
		Themp::Material * GetMaterial(const std::string& materialName, std::vector<std::string>& textures, std::vector<uint8_t>& textureTypes, const std::string& shaderPath, bool geometryShader, bool multisample = false);
		
		//uniqueMesh only has effect if the model you are loading was loaded in before (if true it will duplicate the mesh instead of reusing the existing one)
		Object3D* GetModel(const std::string& path, bool uniqueMesh = false);

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
		ID3D10Blob* ReadToBlob(const std::string& path);
		ID3D11VertexShader* GetVertexShader(const std::string& name);
		ID3D11PixelShader* GetPixelShader(const std::string& name);
		ID3D11GeometryShader * GetGeometryShader(const std::string& name);

		static Resources* TRes;

	private:
		//shaderpath will detect shaders in a named pattern like "path_%type%" so as shader_VS, shader_PS, shader_GS
		Themp::Material* LoadMaterial(const std::string& materialName, const std::string& texture, const std::string& shaderPath, bool geometryShader, D3D11_INPUT_ELEMENT_DESC* nonDefaultIED = nullptr, int numElements = 0, bool multisample = false);
		//shaderpath will detect shaders in a named pattern like "path_%type%" so as shader_VS, shader_PS, shader_GS
		Themp::Material * LoadMaterial(const std::string& materialName, std::vector<std::string>& textures, std::vector<uint8_t>& textureTypes, const std::string& shaderPath, bool geometryShader, bool multisample = false);
		size_t currentUniqueMatIndex = 0;
		Object3D* LoadModel(const std::string& name);
	};
};