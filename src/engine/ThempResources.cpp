#include "ThempSystem.h"
#include "ThempResources.h"
#include "ThempD3D.h"
#include "ThempObject3D.h"
#include "ThempMesh.h"
#include "ThempMaterial.h"
#include <iostream>
#include <fstream>
#include <algorithm>

#pragma warning( disable : 4996)

namespace Themp
{

	std::unordered_map<std::string,std::vector<BYTE>> ShaderFiles =
	{
#ifndef _DEBUG
#include "shaders\debugline_vs.h"
#include "shaders\debugline_ps.h"
#include "shaders\default_vs.h"
#include "shaders\default_ps.h"
#include "shaders\gui_vs.h"
#include "shaders\gui_ps.h"
#include "shaders\screenspace_ps.h"
#include "shaders\screenspace_vs.h"
#include "shaders\voxel_ps.h"
#include "shaders\voxel_vs.h"
#else
#include "shaders\debugline_vs_d.h"
#include "shaders\debugline_ps_d.h"
#include "shaders\default_vs_d.h"
#include "shaders\default_ps_d.h"
#include "shaders\gui_vs_d.h"
#include "shaders\gui_ps_d.h"
#include "shaders\screenspace_ps_d.h"
#include "shaders\screenspace_vs_d.h"
#include "shaders\voxel_ps_d.h"
#include "shaders\voxel_vs_d.h"
#endif
	};

	std::string SanitizeSlashes(std::string input);
	
	Resources* Resources::TRes = nullptr;
	Resources::Resources()
	{
		Resources::TRes = this;

		Buffer b;
		b.buffer = nullptr;
		b.numElements = 0;
		m_VertexBuffers.push_back(b);
		m_IndexBuffers.push_back(b);
	}
	Resources::~Resources()
	{
		for (auto i : m_VertexShaders)
		{
			i.second->Release();
			i.second = nullptr;
		}
		m_VertexShaders.clear();

		for (auto i : m_PixelShaders)
		{
			i.second->Release();
			i.second = nullptr;
		}
		m_PixelShaders.clear();

		for (auto i : m_GeometryShaders)
		{
			i.second->Release();
			i.second = nullptr;
		}
		m_GeometryShaders.clear();
		
		for (auto i : m_Textures)
		{
			delete i.second;
			i.second = nullptr;
		}
		m_Textures.clear();

		for (auto i : m_Materials)
		{
			delete i.second;
			i.second = nullptr;
		}
		m_Materials.clear();

		for (auto i : m_VertexBuffers)
		{
			if (i.buffer == 0)continue;
			i.buffer->Release();
			i.buffer = nullptr;
		}
		m_VertexBuffers.clear();

		for (auto i : m_IndexBuffers)
		{
			if (i.buffer == 0)continue;
			i.buffer->Release();
			i.buffer = nullptr;
		}
		m_IndexBuffers.clear();

		for (auto i : m_Meshes)
		{
			delete i;
			i = nullptr;
		}
		m_Meshes.clear();

		for (auto i : m_3DObjects)
		{
			delete i;
			i = nullptr;
		}
		m_3DObjects.clear();
		Resources::TRes = nullptr;
	}
	void Resources::RemoveObject3D(std::string name, Object3D* m)
	{
		//TODO: Safely be able to "delete" objects, requires manager rework
		assert(false);
		//for (auto i : m_3DObjects)
		//{
		//	if (m == i)
		//	{
		//		for (auto j : m_Meshes)
		//		{
		//			for (size_t k = 0; k < m->m_Meshes.size(); k++)
		//			{
		//				if (j == m->m_Meshes[k])
		//				{
		//					auto iBuffer = m_IndexBuffers[j->i_IndexBuffer];
		//					iBuffer->Release();
		//					m_IndexBuffers.erase(m_IndexBuffers.begin() + j->i_IndexBuffer);
		//
		//
		//					auto vBuffer = m_VertexBuffers[j->i_VertexBuffer];
		//					vBuffer->Release();
		//					m_VertexBuffers.erase(m_VertexBuffers.begin() + j->i_VertexBuffer);
		//
		//					delete j;
		//					j = nullptr;
		//				}
		//			}
		//		}
		//		delete i;
		//		i = nullptr;
		//		break;
		//	}
		//}
	}
	ID3D10Blob* Resources::ReadToBlob(std::string path)
	{
		ID3D10Blob* nBlob = nullptr;
		auto shaderData = ShaderFiles.find(path);
		if (shaderData != ShaderFiles.end())
		{
			D3D10CreateBlob(shaderData->second.size(), &nBlob);
			//memcpy
			memcpy(nBlob->GetBufferPointer(), shaderData->second.data(), shaderData->second.size());
			//ifs.read((char*)nBlob->GetBufferPointer(), length);
		}
		return nBlob;
	}
	Texture* Resources::GetTexture(std::string path, int d3d11_CPU_ACCESS_FLAG)
	{
		if (path == "")
		{
			std::unordered_map<std::string, Texture*>::iterator s = m_Textures.find(path);
			if (s != m_Textures.end()) return s->second;

			Texture* tex = new Texture();
			tex->Create(32, 32, DXGI_FORMAT_R8G8B8A8_UNORM);
			m_Textures[""] = tex;

			return tex;
		}
		std::string basePath = BASE_TEXTURE_PATH;
		basePath = basePath.append(path);
		std::unordered_map<std::string, Texture*>::iterator s = m_Textures.find(basePath);
		if (s != m_Textures.end()) return s->second;

		std::string wPath = std::string(basePath.begin(), basePath.end());
		FILE* t = fopen(wPath.c_str(), "r");
		if (t == nullptr)
		{
			return nullptr;
		}

		Texture* tex = new Texture();
		memset(tex, 0, sizeof(Texture));

		unsigned int width = 0;
		unsigned int height = 0;
		fread(&width, 4, 1, t);
		fread(&height,4, 1, t);

		unsigned int* pixels = (unsigned int*)malloc(width*height * 4);
		fread(pixels, width*height * 4, 1, t);

		tex->Create(width, height, DXGI_FORMAT_R8G8B8A8_UNORM, false, pixels);

		free(pixels);
		fclose(t);

		m_Textures[basePath] = tex;
		return tex;
	}

	Themp::Material* Resources::GetMaterial(std::string materialName, std::string texture, std::string shaderPath, bool geometryShader, D3D11_INPUT_ELEMENT_DESC* nonDefaultIED, int numElements, bool multisample)
	{
		std::unordered_map<std::string, Themp::Material*>::iterator s = m_Materials.find("_" + materialName);
		if (s != m_Materials.end()) return s->second;
		if (shaderPath == "")
		{
			shaderPath = "default";
		}
		return LoadMaterial("_" + materialName, texture, shaderPath, geometryShader, nonDefaultIED, numElements, multisample);
	}
	Themp::Material* Resources::GetMaterial(std::string materialName, std::vector<std::string>& textures, std::vector<uint8_t>& textureTypes, std::string shaderPath, bool geometryShader, bool multisample)
	{
		std::unordered_map<std::string, Themp::Material*>::iterator s = m_Materials.find("_" + materialName);
		if (s != m_Materials.end()) return s->second;
		if (shaderPath == "")
		{
			shaderPath = "default";
		}
		return LoadMaterial("_" + materialName, textures, textureTypes, shaderPath, geometryShader, multisample);
	}
	//Themp::Material * Resources::GetUniqueMaterial(std::string texture, std::string shaderPath)
	//{
	//	char buf[64] = { 0 };
	//	_ui64toa(currentUniqueMatIndex, buf, 10);
	//	std::string uniqueName = std::string("Unique") + buf;
	//	currentUniqueMatIndex++;
	//	std::unordered_map<std::string, Themp::Material*>::iterator s = m_Materials.find(uniqueName);
	//	if (s != m_Materials.end()) return s->second;
	//	if (shaderPath == "")
	//	{
	//		shaderPath = "default";
	//	}
	//	return LoadMaterial(uniqueName, texture, shaderPath, false);
	//}
	Themp::Material * Resources::GetUniqueMaterial(std::string texture, std::string shaderPath,D3D11_INPUT_ELEMENT_DESC* IED,int IEDNumElements)
	{
		char buf[64] = { 0 };
		_ui64toa(currentUniqueMatIndex, buf, 10);
		std::string uniqueName = std::string("Unique") + buf;
		currentUniqueMatIndex++;
		std::unordered_map<std::string, Themp::Material*>::iterator s = m_Materials.find(uniqueName);
		if (s != m_Materials.end()) return s->second;
		if (shaderPath == "")
		{
			shaderPath = "default";
		}
		return LoadMaterial(uniqueName, texture, shaderPath, false,IED,IEDNumElements);
	}
	Themp::Object3D* Resources::GetModel(std::string path, bool uniqueMesh)
	{
		std::unordered_map<std::string, Themp::Object3D*>::iterator s = m_Models.find(path);
		if (s != m_Models.end())
		{
			//clone the model
			Object3D* source = s->second;
			Object3D* clone = new Object3D(*source);
			clone->m_Position = XMFLOAT3(0, 0, 0);
			clone->m_Scale = XMFLOAT3(1,1,1);
			clone->m_Rotation = XMFLOAT3(0, 0, 0);
			clone->m_ConstantBuffer = nullptr;

			if (uniqueMesh)
			{
				clone->m_Meshes.clear();
				for (size_t i = 0; i < source->m_Meshes.size(); i++)
				{
					Mesh* srcMesh = source->m_Meshes[i];
					Mesh* newMesh = new Mesh(*source->m_Meshes[i]);
					m_Meshes.push_back(newMesh);
					newMesh->m_Indices = new uint32_t[srcMesh->m_NumIndices];
					memcpy(newMesh->m_Indices, srcMesh->m_Indices, sizeof(uint32_t)*srcMesh->m_NumIndices);
					newMesh->m_Vertices = new Vertex[srcMesh->m_NumVertices];
					memcpy(newMesh->m_Vertices, srcMesh->m_Vertices, sizeof(Vertex)*srcMesh->m_NumVertices);
					newMesh->m_NumIndices = srcMesh->m_NumIndices;
					newMesh->m_NumVertices = srcMesh->m_NumVertices;
					newMesh->ConstructVertexBuffer();
					clone->m_Meshes.push_back(newMesh);
				}
			}
			else
			{
				clone->m_Meshes = source->m_Meshes;
			}
			m_3DObjects.push_back(clone);
			return clone;
		}
		Object3D* newObject = LoadModel(path);
		m_3DObjects.push_back(newObject);
		return newObject;
	}
	size_t Resources::CreateVertexBuffer(Vertex* vertices, size_t numVertices)
	{
		Themp::D3D* d = Themp::System::tSys->m_D3D;
		D3D11_BUFFER_DESC bd;
		D3D11_MAPPED_SUBRESOURCE ms;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));

		//set up for vertices
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(Vertex) * numVertices;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		ID3D11Buffer* vertexBuffer;
		Buffer buf;
		HRESULT res = d->m_Device->CreateBuffer(&bd, NULL, &vertexBuffer);
		if (res == S_OK)
		{
			buf.buffer = vertexBuffer;
			buf.numElements = numVertices;
			d->m_DevCon->Map(vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
			memcpy(ms.pData, vertices, sizeof(Vertex)*numVertices);
			d->m_DevCon->Unmap(vertexBuffer, NULL);
			m_VertexBuffers.push_back(buf);
			return m_VertexBuffers.size() - 1;
		}
		System::Print("Could not create vertex buffer!");
		return 0;
	}
	bool Resources::EditVertexBuffer(int vertexBuffer, Vertex* vertices, size_t numVertices)
	{
		Themp::D3D* d = Themp::System::tSys->m_D3D; 
		Buffer& buf = m_VertexBuffers[vertexBuffer];
		D3D11_MAPPED_SUBRESOURCE ms;
		ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));
		if (buf.numElements >= numVertices) //re-use our buffer
		{
			if (d->m_DevCon->Map(buf.buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms) == S_OK)
			{
				memcpy(ms.pData, vertices, sizeof(Vertex)*numVertices);
				d->m_DevCon->Unmap(buf.buffer, NULL);
				return true;
			}
		}
		else
		{
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

			//set up for vertices
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(Vertex) * numVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			ID3D11Buffer* vBuffer;
			HRESULT res = d->m_Device->CreateBuffer(&bd, NULL, &vBuffer);
			if (res == S_OK)
			{
				if (d->m_DevCon->Map(vBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms) == S_OK)
				{
					memcpy(ms.pData, vertices, sizeof(Vertex)*numVertices);
					d->m_DevCon->Unmap(vBuffer, NULL);
					buf.buffer->Release(); //release old buffer
					buf.buffer = vBuffer;
					buf.numElements = numVertices;
					return true;
				}
			}
		}
		return false;
	}
	size_t Resources::CreateIndexBuffer(uint32_t* indices, size_t numIndices)
	{
		Themp::D3D* d = Themp::System::tSys->m_D3D;
		HRESULT res;

		D3D11_BUFFER_DESC bd;
		D3D11_MAPPED_SUBRESOURCE ms;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));
		//set up for indices
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(uint32_t) * numIndices;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		ID3D11Buffer* indexBuffer;
		Buffer buf;
		res = d->m_Device->CreateBuffer(&bd, NULL, &indexBuffer);
		if (res == S_OK)
		{
			buf.buffer = indexBuffer;
			buf.numElements = numIndices;
			d->m_DevCon->Map(indexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
			memcpy(ms.pData, indices, sizeof(uint32_t)*numIndices);
			d->m_DevCon->Unmap(indexBuffer, NULL);
			m_IndexBuffers.push_back(buf);
			return m_IndexBuffers.size() - 1;
		}
		System::Print("Could not create index buffer!");
		return 0;
	}
	bool Resources::EditIndexBuffer(int indexBuffer, uint32_t* indices, size_t numIndices)
	{
		Themp::D3D* d = Themp::System::tSys->m_D3D;
		Buffer& buf = m_IndexBuffers[indexBuffer];
		D3D11_MAPPED_SUBRESOURCE ms;
		ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));
		if (buf.numElements >= numIndices) //re-use our buffer
		{
			if (d->m_DevCon->Map(buf.buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms) == S_OK)
			{
				memcpy(ms.pData, indices, sizeof(uint32_t)*numIndices);
				d->m_DevCon->Unmap(buf.buffer, NULL);
				return true;
			}
		}
		else
		{
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

			//set up for vertices
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(uint32_t) * numIndices;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			ID3D11Buffer* iBuffer;
			HRESULT res = d->m_Device->CreateBuffer(&bd, NULL, &iBuffer);
			if (res == S_OK)
			{
				if (d->m_DevCon->Map(iBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms) == S_OK)
				{
					memcpy(ms.pData, indices, sizeof(uint32_t)*numIndices);
					d->m_DevCon->Unmap(iBuffer, NULL);
					buf.buffer->Release(); //release old buffer
					buf.buffer = iBuffer;
					buf.numElements = numIndices;
					return true;
				}
			}
		}
		return false;
	}
	ID3D11VertexShader * Resources::GetVertexShader(std::string name)
	{
		std::unordered_map<std::string, ID3D11VertexShader*>::iterator s = m_VertexShaders.find(name);
		if (s != m_VertexShaders.end()) return s->second;

		HRESULT result = 0;
		ID3D11VertexShader* vShader = nullptr;
		ID3D10Blob* VSRaw = ReadToBlob(name);
		result = System::tSys->m_D3D->m_Device->CreateVertexShader(VSRaw->GetBufferPointer(), VSRaw->GetBufferSize(), nullptr, &vShader);
		if (result != S_OK) { System::Print("Could not create Vertex shader from: %s", name.c_str()); CLEAN(VSRaw); return nullptr; }
		m_VertexShaders[name] = vShader;

		return vShader;
	}

	ID3D11PixelShader * Resources::GetPixelShader(std::string name)
	{
		std::unordered_map<std::string, ID3D11PixelShader*>::iterator s = m_PixelShaders.find(name);
		if (s != m_PixelShaders.end()) return s->second;
		
		HRESULT result = 0;
		ID3D11PixelShader* pShader = nullptr;
		ID3D10Blob* PSRaw = ReadToBlob(name);
		result = System::tSys->m_D3D->m_Device->CreatePixelShader(PSRaw->GetBufferPointer(), PSRaw->GetBufferSize(), nullptr, &pShader);
		if (result != S_OK) { System::Print("Could not create Pixel shader from: %s", name.c_str() ); CLEAN(PSRaw); return nullptr; }
		m_PixelShaders[name] = pShader;

		return pShader;
	}
	ID3D11GeometryShader * Resources::GetGeometryShader(std::string name)
	{
		std::unordered_map<std::string, ID3D11GeometryShader*>::iterator s = m_GeometryShaders.find(name);
		if (s != m_GeometryShaders.end()) return s->second;
		
		HRESULT result = 0;
		ID3D11GeometryShader* gShader = nullptr;
		ID3D10Blob* GSRaw = ReadToBlob(name);
		result = System::tSys->m_D3D->m_Device->CreateGeometryShader(GSRaw->GetBufferPointer(), GSRaw->GetBufferSize(), nullptr, &gShader);
		if (result != S_OK) { System::Print("Could not create Geometry shader from: %", name.c_str()); CLEAN(GSRaw); return nullptr; }
		m_GeometryShaders[name] = gShader;

		return gShader;
	}
	Themp::Material* Resources::LoadMaterial(std::string materialName, std::string texture, std::string shaderPath, bool geometryShader, D3D11_INPUT_ELEMENT_DESC* nonDefaultIED, int numElements, bool multisample)
	{
		std::transform(shaderPath.begin(), shaderPath.end(), shaderPath.begin(), ::tolower);
		std::string tempPath = shaderPath;
		std::unordered_map<std::string, Material*>::iterator s = m_Materials.find(materialName);
		if (s != m_Materials.end()) return s->second;

		Material* material = new Themp::Material();
		//VERTEX SHADER
		{
			HRESULT res;
#ifdef _DEBUG
			tempPath.append("_vs_d");
#else
			tempPath.append("_vs");
#endif
			//material = new Themp::Material();
			material->m_VertexShader = Resources::TRes->GetVertexShader(tempPath);
			if (!material->m_VertexShader)
			{
				System::Print("Couldn't find Vertex shader: %s" ,tempPath.c_str());
				delete material;
				return nullptr;
			}
			std::string blobPath = tempPath;
			ID3D10Blob* vsShaderBlob = ReadToBlob(blobPath);
			if (nonDefaultIED != nullptr)
			{
				res = Themp::System::tSys->m_D3D->m_Device->CreateInputLayout(nonDefaultIED, numElements, vsShaderBlob->GetBufferPointer(), vsShaderBlob->GetBufferSize(), &material->m_InputLayout);
			}
			else
			{
				res = Themp::System::tSys->m_D3D->m_Device->CreateInputLayout(Themp::D3D::DefaultInputLayoutDesc, Themp::D3D::DefaultInputLayoutNumElements, vsShaderBlob->GetBufferPointer(), vsShaderBlob->GetBufferSize(), &material->m_InputLayout);
			}
			CLEAN(vsShaderBlob);
			if (res != S_OK) 
			{
				System::Print("Could not create shader input layout"); 
				delete material;
				return nullptr; 
			}
		}
		//PIXEL SHADER
		{
			tempPath = shaderPath;
#ifdef _DEBUG
			tempPath.append("_ps_d");
#else
			tempPath.append("_ps");
#endif
			material->m_PixelShader = Resources::TRes->GetPixelShader(tempPath);
			if (!material->m_PixelShader)
			{
				System::Print("Couldn't find Pixel shader: %s", tempPath.c_str());
				delete material;
				return nullptr;
			}
		}
		if (geometryShader)
		{
			tempPath = shaderPath;
#ifdef _DEBUG
			tempPath.append("_gs_d");
#else
			tempPath.append("_gs");
#endif
			material->m_GeometryShader = Resources::TRes->GetGeometryShader(tempPath);
			if (!material->m_GeometryShader)System::Print("Couldn't find Geometry shader: %s", tempPath.c_str());
		}
		std::vector<std::string> textures = { texture };
		std::vector<std::uint8_t> textureTypes = { Material::DIFFUSE};
		material->ReadTextures(textures, textureTypes);
		m_Materials[materialName] = material;
		return material;
	}

	Themp::Material* Resources::LoadMaterial(std::string materialName, std::vector<std::string>& textures, std::vector<uint8_t>& textureTypes, std::string shaderPath, bool geometryShader, bool multisample)
	{

		std::transform(shaderPath.begin(), shaderPath.end(), shaderPath.begin(), ::tolower);
		std::string tempPath = shaderPath;

		std::unordered_map<std::string, Material*>::iterator s = m_Materials.find(materialName);
		if (s != m_Materials.end()) return s->second;

		Themp::Material* material = new Themp::Material();
		//VERTEX SHADER
		{
			HRESULT res;
#ifdef _DEBUG
			tempPath.append("_vs_d");
#else
			tempPath.append("_vs");
#endif
			
			material->m_VertexShader = Resources::TRes->GetVertexShader(tempPath);
			if (!material->m_VertexShader)
			{
				System::Print("Couldn't find Vertex shader: %s" ,tempPath.c_str());
				delete material;
				return nullptr;
			}

			std::string blobPath = tempPath;
			ID3D10Blob* vsShaderBlob = ReadToBlob(blobPath);
			res = Themp::System::tSys->m_D3D->m_Device->CreateInputLayout(Themp::D3D::DefaultInputLayoutDesc, Themp::D3D::DefaultInputLayoutNumElements, vsShaderBlob->GetBufferPointer(), vsShaderBlob->GetBufferSize(), &material->m_InputLayout);

			CLEAN(vsShaderBlob);
			if (res != S_OK) {System::Print( "Could not create shader input layout"); delete material;  return nullptr; }
		}
		//PIXEL SHADER
		{
			tempPath = shaderPath;
#ifdef _DEBUG
			tempPath.append("_ps_d");
#else
			tempPath.append("_ps");
#endif
			material->m_PixelShader = Resources::TRes->GetPixelShader(tempPath);
			if (!material->m_PixelShader)System::Print( "Couldn't find Pixel shader: %s" ,tempPath.c_str());
		}

		if (geometryShader)
		{
			tempPath = shaderPath;
#ifdef _DEBUG
			tempPath.append("_gs_d");
#else
			tempPath.append("_gs");
#endif
			material->m_GeometryShader = Resources::TRes->GetGeometryShader(tempPath);
			if (!material->m_GeometryShader)System::Print( "Couldn't find Geometry shader: %s" , tempPath.c_str());
		}

		material->ReadTextures(textures,textureTypes);
		m_Materials[materialName] = material;
		return material;
	}

	Themp::Object3D* Resources::LoadModel(std::string name)
	{
		return 0;
	}
	std::string SanitizeSlashes(std::string input)
	{
		std::string output = "";
		output.reserve(input.size());

		int64_t i = (int64_t)(input.size()-1);
		while (i >= 0)
		{
			//remove double (or more) slashes "//" or "\\" in filepaths
			if (i - 1 >= 0 && (input[i] == '/' || input[i] == '\\' || input[i] == 255) && (input[i - 1] == '/' || input[i - 1] == '\\'))
			{
				//mark for ignore
				input[i] = (unsigned char)255;
			}
			i--;
		}
		for (i = 0; i < input.size(); i++)
		{
			if (input[i] != (unsigned char)255)
			{
				output += input[i];
			}
		}

		return output;
	}
}