#include "ThempSystem.h"
#include "ThempGUIButton.h"
#include "ThempFileManager.h"
#include "ThempResources.h"
#include "ThempObject2D.h"
#include "ThempGame.h"
#include "../Engine/ThempObject3D.h"
#include "../Engine/ThempMesh.h"
#include "../Engine/ThempMaterial.h"
#include "../Engine/ThempD3D.h"
#include <DirectXMath.h>

using namespace Themp;

struct TextureInfo
{
	short w, h;
};

Themp::GUIButton::~GUIButton()
{
	for (size_t i = 0; i < 8; i++)
	{
		if (m_Object[i] != nullptr)
		{
			delete m_Object[i];
		}
	}
}

//source is Object2D::Source
GUIButton::GUIButton(int source, int textureIDNormal, int textureIDHover, int textureIDClick, bool hiRes)
{
	for (size_t i = 0; i < 8; i++)
	{
		m_Object[i] = nullptr;
	}
	numObjects = 1;
	m_Object[0] = new Object2D((Object2D::Source)source, textureIDNormal, hiRes);
	Themp::System::tSys->m_Game->AddObject3D(m_Object[0]->m_Renderable);
	m_TexIDs[0].click = textureIDClick;
	m_TexIDs[0].hover = textureIDHover;
	m_TexIDs[0].normal = textureIDNormal;
	m_TexIDs[0].xOffset = 0;
	m_TexIDs[0].yOffset = 0;
}

//source is Object2D::Source
GUIButton::GUIButton(int source, int* textureIDNormal, int* textureIDHover, int* textureIDClick, float* xOffsets, float* yOffsets, float* zOffsets, int numIDs, bool hiRes)
{
	for (size_t i = 0; i < 8; i++)
	{
		m_Object[i] = nullptr;
	}
	for (size_t i = 0; i < numIDs; i++)
	{
		m_Object[i] = new Object2D((Object2D::Source)source, textureIDNormal[i], hiRes);
		Themp::System::tSys->m_Game->AddObject3D(m_Object[i]->m_Renderable);
		m_TexIDs[i].click = textureIDClick[i];
		m_TexIDs[i].hover = textureIDHover[i];
		m_TexIDs[i].normal = textureIDNormal[i];
		m_TexIDs[i].xOffset = xOffsets[i];
		m_TexIDs[i].yOffset = yOffsets[i];
		m_TexIDs[i].zOffset = zOffsets[i];
	}
	numObjects = numIDs;
}

void GUIButton::NextGUI()
{
	m_Object[0]->m_GUITex = FileManager::GetMenuGUITexture(guiIndex++);
	m_Object[0]->m_Tex = m_Object[0]->m_GUITex->texture;
	m_Object[0]->m_Renderable->m_Meshes[0]->m_Material->SetTexture(m_Object[0]->m_Tex);
}

//size in pixels based on resolution
//void GUIButton::SetSize(int W, int H, int screenWidth, int screenHeight)
//{
//	//Width == 100
//	//W == 10
//	//1.0 =  = width
//	//1.0/width = 1 pixel == 0.01
//	//W / width == U == 0.1 == 10 pixels
//	m_Object[0]->m_Renderable->m_Scale.x = (float)W / (float)screenWidth;
//	m_Object[0]->m_Renderable->m_Scale.y = (float)H / (float)screenHeight;
//}
//size in UV
void GUIButton::SetSize(float W, float H, int index)
{
	m_TexIDs[index].xScale = W * 2;
	m_TexIDs[index].yScale = H * 2;
	m_Object[index]->m_Renderable->m_Scale.x = W * 2.0;
	m_Object[index]->m_Renderable->m_Scale.y = H * 2.0;
}

void Themp::GUIButton::SetVisibility(bool val)
{
	for (int i = 0; i < numObjects; i++)
	{
		m_Object[i]->SetVisibility(val);
	}
}
bool GUIButton::Update(float mouseX, float mouseY, int lmbuttonState, GUIButton* InitialClick)
{
	bool isColliding = false;
	for (int i = 0; i < numObjects; i++)
	{
		Object2D* obj = m_Object[i];
		if (mouseX < m_CenterPosX + m_TexIDs[i].xOffset + (m_TexIDs[i].xScale) && mouseX > m_CenterPosX + m_TexIDs[i].xOffset - (m_TexIDs[i].xScale))
		{
			if (mouseY < m_CenterPosY + m_TexIDs[i].yOffset + (m_TexIDs[i].yScale) && mouseY > m_CenterPosY + m_TexIDs[i].yOffset - (m_TexIDs[i].yScale))
			{
				isColliding = true;
			}
		}
	}
	if (isColliding)
	{
		if(InitialClick == this && lmbuttonState == 1)
		{
			for (int i = 0; i < numObjects; i++)
			{
				Object2D* obj = m_Object[i];
				obj->m_GUITex = FileManager::GetMenuGUITexture(m_TexIDs[i].click);
				obj->m_Tex = obj->m_GUITex->texture;
				obj->m_Renderable->m_Meshes[0]->m_Material->SetTexture(obj->m_Tex);
			}
		}
		else
		{
			for (int i = 0; i < numObjects; i++)
			{
				Object2D* obj = m_Object[i];
				obj->m_GUITex = FileManager::GetMenuGUITexture(m_TexIDs[i].hover);
				obj->m_Tex = obj->m_GUITex->texture;
				obj->m_Renderable->m_Meshes[0]->m_Material->SetTexture(obj->m_Tex);
			}
		}
	}
	else
	{
		for (int i = 0; i < numObjects; i++)
		{
			Object2D* obj = m_Object[i];
			obj->m_GUITex = FileManager::GetMenuGUITexture(m_TexIDs[i].normal);
			obj->m_Tex = obj->m_GUITex->texture;
			obj->m_Renderable->m_Meshes[0]->m_Material->SetTexture(obj->m_Tex);
		}
	}
	return isColliding;
}
void Themp::GUIButton::SetPosition(float x, float y, float z)
{
	m_CenterPosX = x;
	m_CenterPosY = y;
	for (int i = 0; i < numObjects; i++)
	{
		m_Object[i]->m_Renderable->m_Position.x = m_TexIDs[i].xOffset + x;
		m_Object[i]->m_Renderable->m_Position.y = m_TexIDs[i].yOffset + y;
		m_Object[i]->m_Renderable->m_Position.z = m_TexIDs[i].zOffset + z;
	}
}
