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
	{ std::wstring(L"DATA\\FRONTEND"),{ 320 , 200} },
	{ std::wstring(L"DATA\\NOCD"    ),{ 320 , 200} },
	{ std::wstring(L"DATA\\LOADING" ),{ 320 , 200} },
	{ std::wstring(L"DATA\\LEGAL"   ),{ 640 , 400} },
	{ std::wstring(L"LDATA\\LOADING"),{ 320 , 200} },
	{ std::wstring(L"LDATA\\TORTURE"),{ 640 , 400} },
	{ std::wstring(L"LDATA\\DKMAP00"),{ 1280, 800} },
	{ std::wstring(L"LDATA\\DKMAP01"),{ 1280, 800} },
	{ std::wstring(L"LDATA\\DKMAP02"),{ 1280, 800} },
	{ std::wstring(L"LDATA\\DKMAP03"),{ 1280, 800} },
	{ std::wstring(L"LDATA\\DKMAP04"),{ 1280, 800} },
	{ std::wstring(L"LDATA\\DKMAP05"),{ 1280, 800} },
	{ std::wstring(L"LDATA\\DKMAP06"),{ 1280, 800} },
	{ std::wstring(L"LDATA\\DKMAP07"),{ 1280, 800} },
	{ std::wstring(L"LDATA\\DKMAP08"),{ 1280, 800} },
	{ std::wstring(L"LDATA\\DKMAP09"),{ 1280, 800} },
	{ std::wstring(L"LDATA\\DKMAP10"),{ 1280, 800} },
	{ std::wstring(L"LDATA\\DKMAP11"),{ 1280, 800} },
	{ std::wstring(L"LDATA\\DKMAP12"),{ 1280, 800} },
	{ std::wstring(L"LDATA\\DKMAP13"),{ 1280, 800} },
	{ std::wstring(L"LDATA\\DKMAP14"),{ 1280, 800} },
	{ std::wstring(L"LDATA\\DKMAP15"),{ 1280, 800} },
	{ std::wstring(L"LDATA\\DKMAP16"),{ 1280, 800} },
	{ std::wstring(L"LDATA\\DKMAP17"),{ 1280, 800} },
	{ std::wstring(L"LDATA\\DKMAP18"),{ 1280, 800} },
	{ std::wstring(L"LDATA\\DKMAP19"),{ 1280, 800} },
	{ std::wstring(L"LDATA\\DKMAP20"),{ 1280, 800} },
	{ std::wstring(L"LDATA\\DKMAP21"),{ 1280, 800} },
};

Themp::Object2D::~Object2D()
{
	if (m_Tex)
	{
		delete m_Tex;
	}
}

Object2D::Object2D(std::wstring path, bool PALWithRaw)
{
	if (PALWithRaw)
	{
		FileManager::FileData rawData = FileManager::GetFileData(path + L".RAW");
		FileManager::FileData palData = FileManager::GetFileData(path + L".PAL");

		int width = 0;
		int height = 0;
		if (TextureSizeTable.find(path) != TextureSizeTable.end())
		{
			width = TextureSizeTable[path].w;
			height = TextureSizeTable[path].h;
		}
		else
		{
			width = sqrt(rawData.size);
			height = width;
		}
		
		BYTE* imageData = (BYTE*)malloc(rawData.size * 4); //points to a single pixel, so the image itself is rawFileSize * 4 (R G B A) (well RGB only really if alpha is keyed, but we use the alpha value seperately in our shaders)

		//read the image data from palette
		for (size_t i = 0; i < rawData.size; i++)
		{
			imageData[(i * 4)] = palData.data[rawData.data[i] * 3];
			imageData[(i * 4 + 1)] = palData.data[rawData.data[i] * 3 + 1];
			imageData[(i * 4 + 2)] = palData.data[rawData.data[i] * 3 + 2];
			imageData[(i * 4 + 3)] = 255; //TODO add alpha key check
		}
		m_Tex = new Texture();
		m_Tex->Create(width, height, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, false, imageData);
		free(imageData);
	}
}