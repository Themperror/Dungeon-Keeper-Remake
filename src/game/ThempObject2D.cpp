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
};
std::unordered_map<std::wstring, TextureInfo> TextureSizeTable =
{
	{ std::wstring(L"DATA\\FRONTEND"),{ 320 , 200 } }, //Dungeon keeper Splash Screen
	{ std::wstring(L"LDATA\\FRONT"),{ 640 , 480 } }, //Dungeon keeper Splash Screen
	{ std::wstring(L"DATA\\GMAP"	),{ 320 , 200 } }, //Map Screen (ingame)
	{ std::wstring(L"DATA\\GMAPHI"),{ 640 , 400 } }, //Map Screen (ingame)
	{ std::wstring(L"DATA\\NOCD"    ),{ 320 , 200} }, //NO CD inserted
	{ std::wstring(L"DATA\\LOADING" ),{ 320 , 200} }, //LOADING screen
	{ std::wstring(L"DATA\\LEGAL"   ),{ 640 , 400} }, //some copyright stuff. (unused?)
	{ std::wstring(L"LDATA\\LOADING"),{ 320 , 200} },  //LOADING screen (same as other)
	{ std::wstring(L"LDATA\\TORTURE"),{ 640 , 400 } }, //Easter Egg screen
	{ std::wstring(L"LDATA\\DKWIND17"),{960 , 720 } }, //Level select border
	{ std::wstring(L"LDATA\\DKMAP00"),{ 1280, 800} }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP01"),{ 1280, 800} }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP02"),{ 1280, 800} }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP03"),{ 1280, 800} }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP04"),{ 1280, 800} }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP05"),{ 1280, 800} }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP06"),{ 1280, 800} }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP07"),{ 1280, 800} }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP08"),{ 1280, 800} }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP09"),{ 1280, 800} }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP10"),{ 1280, 800} }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP11"),{ 1280, 800} }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP12"),{ 1280, 800} }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP13"),{ 1280, 800} }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP14"),{ 1280, 800} }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP15"),{ 1280, 800} }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP16"),{ 1280, 800} }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP17"),{ 1280, 800} }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP18"),{ 1280, 800} }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP19"),{ 1280, 800} }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP20"),{ 1280, 800} }, //Level select map, "bloodier" depending on level.
	{ std::wstring(L"LDATA\\DKMAP21"),{ 1280, 800 } },//Level select map, "bloodier" depending on level.
};

Themp::Object2D::~Object2D()
{
	if (m_Source == FILE) //we created our own texture so delete it
	{
		if (m_Tex)
		{
			delete m_Tex;
		}
	}
}

Object2D::Object2D(std::wstring path, std::wstring ext)
{
	m_Source = FILE;
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
	if (TextureSizeTable.find(path) != TextureSizeTable.end())
	{
		width = TextureSizeTable[path].w;
		height = TextureSizeTable[path].h;
	}
	else
	{
		width = sqrt(mainData.size);
		height = width;
	}

	BYTE* imageData = nullptr;
	if (hasPalette) //This file likely has a palette along it
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
		//assume MAIN PAL
		coData = FileManager::GetFileData(L"DATA\\MAIN.PAL");
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
	if (m_Tex)
	{
		m_Renderable = new Object3D();
		m_Renderable->CreateQuad("ScreenSpace", false);
		size_t uniqueMatID = (size_t)this;
		char buf[16] = { 0 };
		_i64toa(uniqueMatID, buf, 16);
		Material* material = Resources::TRes->GetMaterial(buf, "", "ScreenSpace", false);
		material->SetTexture(m_Tex);
		m_Renderable->SetMaterial(material);
	}
}
Themp::Object2D::Object2D(Source textureSource, int textureIndex, bool hiRes)
{
	m_Source = textureSource;
	switch (m_Source)
	{
	case LEVEL_MISC:
		m_GUITex = FileManager::GetLevelMiscGUITexture(textureIndex, hiRes);
	break;
	case LEVEL_PANE:
		m_GUITex = FileManager::GetLevelPaneGUITexture(textureIndex,hiRes);
	break;
	case MENU_MAIN:
		m_GUITex = FileManager::GetMenuGUITexture(textureIndex);
		break;
	case MENU_CURSOR:
		m_GUITex = FileManager::GetMenuCursorTexture(textureIndex);
	break;
	default: assert(false); assert(true); break; //I can never remember if it asserts on true or false.. So why not both ¯\_(O_O)_/¯
	}
	m_Tex = m_GUITex->texture;
	if (m_Tex)
	{
		m_Renderable = new Object3D();
		m_Renderable->CreateQuad("ScreenSpace", false);
		size_t uniqueMatID = (size_t)this;
		char buf[16] = { 0 };
		_i64toa(uniqueMatID, buf, 16);
		Material* material = Resources::TRes->GetMaterial(buf, "", "ScreenSpace", false);
		material->SetTexture(m_Tex);
		m_Renderable->SetMaterial(material);
	}
}

void Object2D::SetVisibility(bool val)
{
	m_Renderable->isVisible = val;
}
void Object2D::SetSprite(int offX, int offY, int W, int H, int MAXW, int MAXH)
{
	float uUnit = (float)1.0 / (float)MAXW;
	float vUnit = (float)1.0 / (float)MAXH;
	float startU = offX *W* uUnit;
	float startV = offY *H*vUnit;
	float endU = startU + (W * uUnit);
	float endV = startV + (H * vUnit);

	Themp::Mesh* m = m_Renderable->m_Meshes[0];

	m->m_Vertices[0].u = startU;
	m->m_Vertices[0].v = startV;

	m->m_Vertices[1].u = startU;
	m->m_Vertices[1].v = endV;

	m->m_Vertices[2].u = endU;
	m->m_Vertices[2].v = endV;

	m->m_Vertices[3].u = endU;
	m->m_Vertices[3].v = startV;


	D3D11_MAPPED_SUBRESOURCE ms;
	ZeroMemory(&ms, sizeof(D3D11_MAPPED_SUBRESOURCE));

	Themp::D3D::s_D3D->m_DevCon->Map(m->m_VertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, m->m_Vertices, sizeof(Vertex)*m->m_NumVertices);
	Themp::D3D::s_D3D->m_DevCon->Unmap(m->m_VertexBuffer, NULL);
}
