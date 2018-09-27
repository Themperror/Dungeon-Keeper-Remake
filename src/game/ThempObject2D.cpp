#include "ThempSystem.h"
#include "ThempObject2D.h"
#include "ThempGame.h"
#include "ThempFileManager.h"
#include "ThempResources.h"
#include "../Library/imgui.h"
#include "../Engine/ThempCamera.h"
#include "../Engine/ThempObject3D.h"
#include "../Engine/ThempMesh.h"
#include "../Engine/ThempMaterial.h"
#include "../Engine/ThempD3D.h"
#include "../Engine/ThempFunctions.h"
#include "../Engine/ThempDebugDraw.h"
#include "../Engine/ThempVideo.h"
#include <DirectXMath.h>

using namespace Themp;

struct TextureInfo
{
	short w, h;
	bool special;
	std::wstring palFile;
};
std::unordered_map<std::wstring, TextureInfo> TextureSizeTable =
{
	{ std::wstring(L"DATA\\FRONTEND"),{ 320 , 200,false,L"DATA\\MAIN.PAL" } }, //Dungeon keeper Splash Screen
	{ std::wstring(L"LDATA\\FRONT")  ,{ 640 , 480,false,L"DATA\\MAIN.PAL" } }, //Dungeon keeper Splash Screen
	{ std::wstring(L"DATA\\GMAP"	),{ 320 , 200,false,L"DATA\\MAIN.PAL" } }, //Map Screen (ingame)
	{ std::wstring(L"DATA\\GMAPHI")  ,{ 640 , 400,false,L"DATA\\MAIN.PAL" } }, //Map Screen (ingame)
	{ std::wstring(L"DATA\\NOCD"    ),{ 320 , 200,false,L"DATA\\MAIN.PAL" } }, //NO CD inserted
	{ std::wstring(L"DATA\\LOADING" ),{ 320 , 200,false,L"DATA\\MAIN.PAL" } }, //LOADING screen
	{ std::wstring(L"DATA\\LEGAL"   ),{ 640 , 400,false,L"DATA\\MAIN.PAL" } }, //some copyright stuff. (unused?)
	{ std::wstring(L"LDATA\\LOADING"),{ 320 , 200,false,L"DATA\\MAIN.PAL" } },  //LOADING screen (same as other)
	{ std::wstring(L"LDATA\\TORTURE"),{ 640 , 400,false,L"DATA\\MAIN.PAL" } }, //Easter Egg screen
	{ std::wstring(L"LDATA\\DKMAP00"),{ 1280, 800,false,L"DATA\\MAIN.PAL" } }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP01"),{ 1280, 800,false,L"DATA\\MAIN.PAL" } }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP02"),{ 1280, 800,false,L"DATA\\MAIN.PAL" } }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP03"),{ 1280, 800,false,L"DATA\\MAIN.PAL" } }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP04"),{ 1280, 800,false,L"DATA\\MAIN.PAL" } }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP05"),{ 1280, 800,false,L"DATA\\MAIN.PAL" } }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP06"),{ 1280, 800,false,L"DATA\\MAIN.PAL" } }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP07"),{ 1280, 800,false,L"DATA\\MAIN.PAL" } }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP08"),{ 1280, 800,false,L"DATA\\MAIN.PAL" } }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP09"),{ 1280, 800,false,L"DATA\\MAIN.PAL" } }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP10"),{ 1280, 800,false,L"DATA\\MAIN.PAL" } }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP11"),{ 1280, 800,false,L"DATA\\MAIN.PAL" } }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP12"),{ 1280, 800,false,L"DATA\\MAIN.PAL" } }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP13"),{ 1280, 800,false,L"DATA\\MAIN.PAL" } }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP14"),{ 1280, 800,false,L"DATA\\MAIN.PAL" } }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP15"),{ 1280, 800,false,L"DATA\\MAIN.PAL" } }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP16"),{ 1280, 800,false,L"DATA\\MAIN.PAL" } }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP17"),{ 1280, 800,false,L"DATA\\MAIN.PAL" } }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP18"),{ 1280, 800,false,L"DATA\\MAIN.PAL" } }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP19"),{ 1280, 800,false,L"DATA\\MAIN.PAL" } }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP20"),{ 1280, 800,false,L"DATA\\MAIN.PAL" } }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP21"),{ 1280, 800,false,L"DATA\\MAIN.PAL" } },//Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKWIND00"),{ 960, 720,true,L"LDATA\\DKMAP00.PAL" } }, //Level select border, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKWIND01"),{ 960, 720,true,L"LDATA\\DKMAP01.PAL" } }, //Level select border, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKWIND02"),{ 960, 720,true,L"LDATA\\DKMAP02.PAL" } }, //Level select border, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKWIND03"),{ 960, 720,true,L"LDATA\\DKMAP03.PAL" } }, //Level select border, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKWIND04"),{ 960, 720,true,L"LDATA\\DKMAP04.PAL" } }, //Level select border, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKWIND05"),{ 960, 720,true,L"LDATA\\DKMAP05.PAL" } }, //Level select border, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKWIND06"),{ 960, 720,true,L"LDATA\\DKMAP06.PAL" } }, //Level select border, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKWIND07"),{ 960, 720,true,L"LDATA\\DKMAP07.PAL" } }, //Level select border, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKWIND08"),{ 960, 720,true,L"LDATA\\DKMAP08.PAL" } }, //Level select border, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKWIND09"),{ 960, 720,true,L"LDATA\\DKMAP09.PAL" } }, //Level select border, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKWIND10"),{ 960, 720,true,L"LDATA\\DKMAP10.PAL" } }, //Level select border, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKWIND11"),{ 960, 720,true,L"LDATA\\DKMAP11.PAL" } }, //Level select border, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKWIND12"),{ 960, 720,true,L"LDATA\\DKMAP12.PAL" } }, //Level select border, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKWIND13"),{ 960, 720,true,L"LDATA\\DKMAP13.PAL" } }, //Level select border, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKWIND14"),{ 960, 720,true,L"LDATA\\DKMAP14.PAL" } }, //Level select border, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKWIND15"),{ 960, 720,true,L"LDATA\\DKMAP15.PAL" } }, //Level select border, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKWIND16"),{ 960, 720,true,L"LDATA\\DKMAP16.PAL" } }, //Level select border, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKWIND17"),{ 960, 720,true,L"LDATA\\DKMAP17.PAL" } }, //Level select border, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKWIND18"),{ 960, 720,true,L"LDATA\\DKMAP18.PAL" } }, //Level select border, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKWIND19"),{ 960, 720,true,L"LDATA\\DKMAP19.PAL" } }, //Level select border, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKWIND20"),{ 960, 720,true,L"LDATA\\DKMAP20.PAL" } }, //Level select border, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKWIND21"),{ 960, 720,true,L"LDATA\\DKMAP21.PAL" } }, //Level select border, "bloodier" depending on level.
};

Themp::Object2D::~Object2D()
{
	if (m_Source == sFILE) //we created our own texture so delete it
	{
		if (m_Tex)
		{
			delete m_Tex;
		}
	}
}

Themp::Object2D::Object2D()
{
	m_Source = sCUSTOM;
	m_Renderable = new Object3D();
	m_Renderable->CreateQuad("ScreenSpace", false);
	m_Material = Resources::TRes->GetUniqueMaterial("", "ScreenSpace");
	//material->SetTexture(m_Tex);
	m_Renderable->SetMaterial(m_Material);
	SetScale(1, 1);
}

Object2D::Object2D(std::wstring path, std::wstring ext)
{
	m_Source = sFILE;
	FileData mainData = FileManager::GetFileData(path + ext);
	FileData coData = { 0 };
	bool hasPalette = false;
	if (ext == L".RAW")
	{
		coData = FileManager::GetFileData(path + L".PAL");
		if (coData.size > 0)
		{
			hasPalette = true;
		}
	}
	if (mainData.size == 0)
	{
		System::Print("Did not find %S%S", path.c_str(), ext.c_str());
		return;
	}

	int width = 0;
	int height = 0;
	bool specialDecoding = false;
	std::wstring usingPalFile = L"DATA\\MAIN.PAL";
	if (TextureSizeTable.find(path) != TextureSizeTable.end())
	{
		width = TextureSizeTable[path].w;
		height = TextureSizeTable[path].h;
		usingPalFile = TextureSizeTable[path].palFile;
		specialDecoding = TextureSizeTable[path].special;
	}
	else
	{
		width = sqrt(mainData.size);
		height = width;
	}

	BYTE* imageData = nullptr;
	if (hasPalette) //This file has a palette along it
	{
		imageData = (BYTE*)malloc(mainData.size * 4); //points to a single pixel, so the image itself is rawFileSize * 4 (R G B A) (well RGB only really if alpha is keyed, but we use the alpha value seperately in our shaders)

		//read the image data from palette
		for (size_t i = 0; i < mainData.size; i++)
		{
			imageData[(i * 4)] = coData.data[mainData.data[i] * 3] * 4;
			imageData[(i * 4 + 1)] = coData.data[mainData.data[i] * 3 + 1] * 4;
			imageData[(i * 4 + 2)] = coData.data[mainData.data[i] * 3 + 2] * 4;
			imageData[(i * 4 + 3)] = 255; //TODO add alpha key check
		}
		m_Tex = new Texture();
		m_Tex->Create(width, height, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, false, imageData);
		free(imageData);
	}
	else
	{
		coData = FileManager::GetFileData(usingPalFile);
		if (specialDecoding)
		{
			imageData = (BYTE*)malloc(width * height * 4);
			memset(imageData, 0, width * height * 4);
			BYTE* startOff = (mainData.data + 0xB40); //These files have offset table, not sure if we need to use it yet
			int srcIndex = 0;
			int writtenBytes = 0;
			while(srcIndex < mainData.size - 0xB40)
			{ 
				//num Opaque pixels
				uint32_t numOpaque = startOff[srcIndex++];
				numOpaque += startOff[srcIndex++] << 8;
				numOpaque += startOff[srcIndex++] << 16;
				numOpaque += startOff[srcIndex++] << 24;
				for (size_t z = 0; z < numOpaque; z++)
				{
					imageData[(writtenBytes) * 4]	   = coData.data[startOff[srcIndex] * 3] * 4;
					imageData[(writtenBytes) * 4 + 1] = coData.data[startOff[srcIndex] * 3 + 1] * 4;
					imageData[(writtenBytes) * 4 + 2] = coData.data[startOff[srcIndex] * 3 + 2] * 4;
					imageData[(writtenBytes) * 4 + 3] = 255;
					writtenBytes++;
					srcIndex++;
				}
				uint32_t numAlpha = startOff[srcIndex++];
				numAlpha += startOff[srcIndex++] << 8;
				numAlpha += startOff[srcIndex++] << 16;
				numAlpha += startOff[srcIndex++] << 24;
				for (size_t z = 0; z < numAlpha; z++)
				{
					imageData[(writtenBytes) * 4] = 0;
					imageData[(writtenBytes) * 4 + 1] = 0;
					imageData[(writtenBytes) * 4 + 2] = 0;
					imageData[(writtenBytes) * 4 + 3]  =0;
					writtenBytes++;
				}
			}
		}
		else
		{
			imageData = (BYTE*)malloc(mainData.size * 4); //points to a single pixel, so the image itself is rawFileSize * 4 (R G B A) (well RGB only really if alpha is keyed, but we use the alpha value seperately in our shaders)

			//read the image data from palette
			for (size_t i = 0; i < mainData.size; i++)
			{
				imageData[(i * 4)] = coData.data[mainData.data[i] * 3] * 4;
				imageData[(i * 4 + 1)] = coData.data[mainData.data[i] * 3 + 1] * 4;
				imageData[(i * 4 + 2)] = coData.data[mainData.data[i] * 3 + 2] * 4;
				imageData[(i * 4 + 3)] = 255; //TODO add alpha key check
			}

		}
		m_Tex = new Texture();
		m_Tex->Create(width, height, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, false, imageData);
		free(imageData);
	}
	if (m_Tex)
	{
		m_Renderable = new Object3D();
		m_Renderable->CreateQuad("ScreenSpace", false);
		m_Material = Resources::TRes->GetUniqueMaterial("", "ScreenSpace");
		m_Material->SetTexture(m_Tex);
		m_Renderable->SetMaterial(m_Material);
		SetScale(1, 1);
	}
}
Themp::Object2D::Object2D(Source textureSource, int textureIndex, bool hiRes)
{
	m_IsHiRes = hiRes;
	m_Source = textureSource;
	m_GUITex = GetTexture(textureSource, textureIndex);
	m_Tex = m_GUITex->texture;
	if (m_Tex)
	{
		m_Renderable = new Object3D();
		m_Renderable->CreateQuad("ScreenSpace", false);
		m_Material = Resources::TRes->GetUniqueMaterial("", "ScreenSpace");
		m_Material->SetTexture(m_Tex);
		m_Renderable->SetMaterial(m_Material);
		SetScale(1, 1);
	}
}

void Object2D::SetVisibility(bool val)
{
	m_Renderable->isVisible = val;
}
void Object2D::SetTexture(GUITexture* tex)
{
	m_Tex = tex->texture;
	m_GUITex = tex;
	m_Renderable->m_Meshes[0]->m_Material->SetTexture(m_Tex);
}
void Object2D::SetTexture(Texture* tex)
{
	m_Tex = tex;
	m_Renderable->m_Meshes[0]->m_Material->SetTexture(m_Tex);
}
void Object2D::SetScale(float W, float H)
{
	if (m_Tex)
	{
		Texture* t = m_Tex;
		float originalW = (float)t->m_Width / 640.0; //original screen width
		float originalH = (float)t->m_Height / 480.0; //original screen height
		float newW = originalW * 2.0;// *4.0 ;//* System::tSys->m_SVars[SVAR_SCREENWIDTH];
		float newH = originalH * 2.0;// *4.0 ;//* System::tSys->m_SVars[SVAR_SCREENHEIGHT];
		m_Renderable->SetScale(W * newW, H * newH, 1);
	}
}
GUITexture* Object2D::GetTexture(Object2D::Source source, int index)
{
	switch (m_Source)
	{
	case sLEVEL_MISC:
		return FileManager::GetLevelMiscGUITexture(index, m_IsHiRes);
		break;
	case sLEVEL_PANE:
		return FileManager::GetLevelPaneGUITexture(index, m_IsHiRes);
		break;
	case sMENU_MAIN:
		return FileManager::GetMenuGUITexture(index);
		break;
	case sMENU_CURSOR:
		return FileManager::GetMenuCursorTexture(index);
		break;
	case sMENU_LEVELFLAG:
		return FileManager::GetLevelFlagTexture(index);
		break;
	default: assert(false); break;
	}
	return nullptr;
}
DirectX::XMFLOAT2 Object2D::GetSizeinScreenPercentage(int screenWidth, int screenHeight)
{
	XMFLOAT2 res; //result
	XMFLOAT3& cScale = m_Renderable->m_Scale;
	res.x = 100.0 * cScale.x / 2.0;
	res.y = 100.0 * cScale.y / 2.0;
	return res;
}
//void Object2D::SetSprite(int offX, int offY, int W, int H, int MAXW, int MAXH)
//{
//	float uUnit = (float)1.0 / (float)MAXW;
//	float vUnit = (float)1.0 / (float)MAXH;
//	float startU = offX *W* uUnit;
//	float startV = offY *H*vUnit;
//	float endU = startU + (W * uUnit);
//	float endV = startV + (H * vUnit);
//
//	Themp::Mesh* m = m_Renderable->m_Meshes[0];
//
//	m->m_Vertices[0].u = startU;
//	m->m_Vertices[0].v = startV;
//
//	m->m_Vertices[1].u = startU;
//	m->m_Vertices[1].v = endV;
//
//	m->m_Vertices[2].u = endU;
//	m->m_Vertices[2].v = endV;
//
//	m->m_Vertices[3].u = endU;
//	m->m_Vertices[3].v = startV;
//
//
//	D3D11_MAPPED_SUBRESOURCE ms;
//	ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));
//
//	Themp::D3D::s_D3D->m_DevCon->Map(m->m_VertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
//	memcpy(ms.pData, m->m_Vertices, sizeof(Vertex)*m->m_NumVertices);
//	Themp::D3D::s_D3D->m_DevCon->Unmap(m->m_VertexBuffer, NULL);
//}
