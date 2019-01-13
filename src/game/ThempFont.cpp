#include "ThempSystem.h"
#include "ThempFont.h"
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

Themp::Font::~Font()
{
	//for (size_t i = 0; i < m_Text.size(); i++)
	//{
	//	if (m_Renderables[i] == nullptr)continue;
	//	delete m_Renderables[i];
	//	m_Renderables[i] = nullptr;
	//}
	if (m_ScreenObj != nullptr)
	{
		delete m_ScreenObj; 
		m_ScreenObj = nullptr;
	}
	delete m_Texture->texture;
	delete m_Texture;
}

//Themp::Font::Font(std::string text, FontTexID fontTexture, bool hiRes, DirectX::XMFLOAT3 position)
//{
//	m_Position = position;
//	m_Font = FileManager::GetFont(fontTexture + (fontTexture == FontTexID::INGAME && hiRes ? 1 : 0));
//	m_Text = text;
//	m_Renderables = new Object2D*[text.size()];
//	float scrWidth = System::tSys->m_SVars[SVAR_SCREENWIDTH];
//	float scrHeight = System::tSys->m_SVars[SVAR_SCREENHEIGHT];
//	float lastX = position.x;
//	float lastY = position.y;
//	for (size_t i = 0; i < text.size(); i++)
//	{
//		m_Renderables[i] = nullptr;
//
//		int textID = (uint8_t)text[i] - 32;
//		if (text[i] == ' ')
//		{
//			lastX += 1.0 / 800.0 * 20.0;
//			continue;
//		}
//		else if (text[i] == '\n')
//		{
//			lastX = m_Position.x;
//			lastY -= 1.0 / 800 * 64.0f;
//			continue;
//		}
//		if (textID < 0)continue;
//		Object2D* obj = new Object2D();
//		m_Renderables[i] = obj;
//		obj->m_GUITex = &m_Font->at(textID);
//		obj->m_Material->SetTexture(obj->m_GUITex->texture);
//
//		obj->m_Renderable->m_Position.x = lastX;
//		obj->m_Renderable->m_Position.y = lastY;
//		obj->m_Renderable->m_Position.z = position.z - i*0.00001;
//
//		lastX += obj->m_GUITex->width / 800.0 + (1.0 / 800.0 * 10.0);
//		obj->m_Renderable->SetScale(obj->m_GUITex->width / 800.0, obj->m_GUITex->height / 600.0, 1);
//		obj->SetVisibility(true);
//		System::tSys->m_Game->AddObject3D(obj->m_Renderable);
//	}
//}
Themp::Font::Font(std::string text, FontTexID fontTexture, bool hiRes, DirectX::XMFLOAT3 position, int lineheight)
{
	m_Font = FileManager::GetFont(fontTexture + (fontTexture == FontTexID::INGAME && hiRes ? 1 : 0));
	m_Text = text;
	m_ScreenObj = new Object2D();
	m_ScreenObj->m_Renderable->SetPosition(position);
	System::tSys->m_Game->AddObject3D(m_ScreenObj->m_Renderable);
	
	ChangeText(text, lineheight);
}
//Font is not made for changing text.. this is gonna be a pain, make sure to minimize amount of calls to this function
void Themp::Font::ChangeText(std::string text,int lineheight)
{
	int dataWidth = 0;
	int dataHeight = 0;
	int numLines = 0;
	int biggestYTex = 0;

	int spacing = 0;
	int spaceSize = 10;
	for (size_t i = 0; i < text.size(); i++)
	{
		int textID = (uint8_t)text[i] - 32;
		if (text[i] == ' ')
		{
			dataWidth += spaceSize;
			continue;
		}
		else if (text[i] == '\n')
		{
			numLines++;
			continue;
		}
		if (textID < 0)continue;
		GUITexture* tex = &m_Font->at(textID);
		if (biggestYTex < tex->height)biggestYTex = tex->height;
		dataWidth += tex->width + spacing;
		if (dataHeight < biggestYTex + numLines * lineheight) dataHeight = biggestYTex + numLines * lineheight;
	}
	BYTE* imageData = (BYTE*)malloc(dataWidth * dataHeight * 4);
	memset(imageData, 0, dataWidth*dataHeight * 4);
	int lastX = 0;
	int lastY = 0;
	for (size_t i = 0; i < text.size(); i++)
	{
		int textID = (uint8_t)text[i] - 32;
		if (text[i] == ' ')
		{
			lastX += spaceSize;
			continue;
		}
		else if (text[i] == '\n')
		{
			lastX = 0;
			lastY += lineheight;
			continue;
		}
		if (textID < 0)continue;
		GUITexture* tex = &m_Font->at(textID);
		BYTE* texData = (BYTE*)tex->texture->m_Data;
		for (int y = 0; y < tex->height; y++)
		{
			for (int x = 0; x < tex->width; x++)
			{
				int posX = lastX + x;
				int posY = lastY + y;
				int writePos = posX + posY * dataWidth;
				imageData[(writePos * 4)] = texData[(x + y * tex->width) * 4];
				imageData[(writePos * 4 + 1)] = texData[(x + y * tex->width) * 4 + 1];
				imageData[(writePos * 4 + 2)] = texData[(x + y * tex->width) * 4 + 2];
				imageData[(writePos * 4 + 3)] = texData[(x + y * tex->width) * 4 + 3];
			}
		}

		lastX += tex->width + spacing;
	}

	//yeah this sucks..
	if (m_Texture)
	{
		delete m_Texture->texture;
		delete m_Texture;
	}
	m_Texture = new GUITexture();
	m_Texture->height = dataHeight;
	m_Texture->width = dataWidth;
	m_Texture->texture = new Texture();
	m_Texture->texture->Create(dataWidth, dataHeight, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, false, imageData);
	free(imageData);

	m_ScreenObj->m_GUITex = m_Texture;
	m_ScreenObj->m_Tex = m_Texture->texture;
	m_ScreenObj->m_Material->SetTexture(m_Texture->texture);
	SetScale(1, 1, 1);
}

void Themp::Font::SetVisibility(bool val)
{
	m_ScreenObj->SetVisibility(val);
}
void Themp::Font::SetScale(float x, float y, float z)
{
	Texture* t = m_Texture->texture;
	float originalW = (float)t->m_Width / 640.0f; //original screen width
	float originalH = (float)t->m_Height / 480.0f; //original screen height
	float newW = originalW * 2.0f;// *4.0 ;//* System::tSys->m_SVars[SVAR_SCREENWIDTH];
	float newH = originalH * 2.0f;// *4.0 ;//* System::tSys->m_SVars[SVAR_SCREENHEIGHT];

	m_ScreenObj->m_Renderable->SetScale(x * newW, y * newH, z);
}
