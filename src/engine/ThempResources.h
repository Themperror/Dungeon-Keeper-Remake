#pragma once
#include <unordered_map>
#include <d3d11.h>

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
		Resources();
		~Resources();
		void RemoveObject3D(std::string name, Object3D * m);
		Texture* GetTexture(std::string path,int d3d11_CPU_ACCESS_FLAG = 0);

		Themp::Material * GetUniqueMaterial(std::string texture, std::string shaderPath);
		Themp::Material * GetMaterial(std::string materialName, std::string texture, std::string shaderPath, bool geometryShader, D3D11_INPUT_ELEMENT_DESC* nonDefaultIED = nullptr, int numElements = 0, bool multisample = false);
		Themp::Material * GetMaterial(std::string materialName, std::vector<std::string>& textures, std::vector<uint8_t>& textureTypes, std::string shaderPath, bool geometryShader, bool multisample = false);
		
		//uniqueMesh only has effect if the model you are loading was loaded in before (if true it will duplicate the mesh instead of reusing the existing one)
		Object3D* GetModel(std::string path, bool uniqueMesh = false);

		size_t CreateVertexBuffer(Vertex * vertices, size_t numVertices);
		bool EditVertexBuffer(Vertex * vertices, size_t numVertices);
		size_t CreateIndexBuffer(uint32_t * indices, size_t numIndices);
		
		std::unordered_map<std::string, ID3D11VertexShader*> m_VertexShaders;
		std::unordered_map<std::string, ID3D11PixelShader*> m_PixelShaders;
		std::unordered_map<std::string, ID3D11GeometryShader*> m_GeometryShaders;
		std::unordered_map<std::string, Texture*> m_Textures;
		std::unordered_map<std::string, Themp::Material*> m_Materials;
		std::unordered_map<std::string, Object3D*> m_Models;
		std::vector<ID3D11Buffer*> m_VertexBuffers;
		std::vector<ID3D11Buffer*> m_IndexBuffers;
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