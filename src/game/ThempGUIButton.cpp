#include "ThempSystem.h"
#include "ThempGUIButton.h"
#include "ThempFont.h"
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

std::unordered_map<int, std::vector<int>> buttonAnimations =
{
	{ 1,{ 13,10,7,4,1 } },
	{ 2,{ 14,11,8,5,2 } },
	{ 3,{ 15,12,9,6,3 } },
	{15,{ 17,19 }},
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
	for (size_t i = 0; i < 3; i++)
	{
		delete m_FontObjects[i];
	}
}

//source is Object2D::Source
GUIButton::GUIButton(int source, int textureIDNormal, int textureIDHover, int textureIDClick, bool hiRes)
{
	for (size_t i = 0; i < 8; i++)
	{
		m_Object[i] = nullptr;
	}
	m_NumObjects = 1;
	m_Object[0] = new Object2D((Object2D::Source)source, textureIDNormal, hiRes);
	Themp::System::tSys->m_Game->AddObject3D(m_Object[0]->m_Renderable);
	m_TexIDs[0].click = textureIDClick;
	m_TexIDs[0].hover = textureIDHover;
	m_TexIDs[0].normal = textureIDNormal;
	m_TexIDs[0].xOffset = 0;
	m_TexIDs[0].yOffset = 0;
}

Themp::GUIButton::GUIButton(int source, int* textureIDNormal, int* textureIDHover, int* textureIDClick, float * xOffsets, float * yOffsets, float * zOffsets, int numIDs, bool hiRes, std::string text, int textFontNormal, int texttFontHover, int textFontDisabled)
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

		auto&& it = buttonAnimations.find(textureIDHover[i]);
		if (it != buttonAnimations.end())
		{
			m_TexIDs[i].animation = &it->second;
		}
		else
		{
			m_TexIDs[i].animation = nullptr;
		}
	}
	m_NumObjects = numIDs;
	if (text.size() > 0)
	{
		m_FontObjects[0] = new Themp::Font(text, (Font::FontTexID)textFontNormal, false, DirectX::XMFLOAT3(m_CenterPosX, m_CenterPosY, zOffsets[0]));
		m_FontObjects[1] = new Themp::Font(text, (Font::FontTexID)texttFontHover, false, DirectX::XMFLOAT3(m_CenterPosX, m_CenterPosY, zOffsets[0]));
		m_FontObjects[2] = new Themp::Font(text, (Font::FontTexID)textFontDisabled, false, DirectX::XMFLOAT3(m_CenterPosX, m_CenterPosY, zOffsets[0]));
	}
}

void GUIButton::NextGUI()
{
	m_Object[0]->m_GUITex = FileManager::GetMenuGUITexture(m_GuiIndex++);
	m_Object[0]->m_Tex = m_Object[0]->m_GUITex->texture;
	m_Object[0]->m_Renderable->m_Meshes[0]->m_Material->SetTexture(m_Object[0]->m_Tex);
}


//size in multiplier
void GUIButton::SetSize(float W,float H, float textW, float textH, int index)
{
	Texture* t = m_Object[index]->m_Tex;
	float originalW = (float)t->m_Width  / 640.0; //original screen width
	float originalH = (float)t->m_Height / 480.0; //original screen height
	float newW = originalW*2.0;// *4.0 ;//* System::tSys->m_SVars[SVAR_SCREENWIDTH];
	float newH = originalH*2.0;// *4.0 ;//* System::tSys->m_SVars[SVAR_SCREENHEIGHT];

	m_Object[index]->m_Renderable->SetScale(W * newW,H * newH,1);

	m_TexIDs[index].xScale = newW * W;
	m_TexIDs[index].yScale = newH * H;

	for (size_t i = 0; i < 3; i++)
	{
		m_FontObjects[i]->SetScale(textW ,textH, 1);
	}

	//m_Object[index]->m_Renderable->m_Scale.x = W * 2.0;
	//m_Object[index]->m_Renderable->m_Scale.y = H * 2.0;
	//m_FontObjects[index]->SetScale(W, H, 1);
}

void Themp::GUIButton::SetVisibility(bool val)
{
	for (int i = 0; i < m_NumObjects; i++)
	{
		m_Object[i]->SetVisibility(val);
	}
	for (size_t i = 0; i < 3; i++)
	{
		m_FontObjects[i]->SetVisibility(val);
	}
	m_IsVisible = val;
}

bool GUIButton::Update(float delta, float mouseX, float mouseY, int lmbuttonState, GUIButton* InitialClick)
{
	bool isColliding = false;
	for (int i = 0; i < m_NumObjects; i++)
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
		m_AnimTime += delta;
		if (m_AnimTime > 1.0 / 15.0)
		{
			m_AnimTime -= 1.0 / 15.0;
			if (m_Reversing) //pingpong animation
			{
				m_CurrentAnimIndex--;
				if (m_CurrentAnimIndex < 0)
				{
					m_Reversing = false;
					m_CurrentAnimIndex = 1;
				}
				for (int i = 0; i < m_NumObjects; i++)
				{
					//Textures with Hover but no animation
					if (m_TexIDs[i].animation == nullptr)
					{
						Object2D* obj = m_Object[i];
						if (m_TexIDs[i].hover != -1) //has hover
						{
							obj->m_GUITex = FileManager::GetMenuGUITexture(m_TexIDs[i].hover);
							obj->m_Tex = obj->m_GUITex->texture;
							obj->m_Renderable->m_Meshes[0]->m_Material->SetTexture(obj->m_Tex);
						}//else no hover, just continue
						continue;
					}
					//hover and animation
					Object2D* obj = m_Object[i];
					obj->m_GUITex = FileManager::GetMenuGUITexture(m_TexIDs[i].animation->at(m_CurrentAnimIndex));
					obj->m_Tex = obj->m_GUITex->texture;
					obj->m_Renderable->m_Meshes[0]->m_Material->SetTexture(obj->m_Tex);
				}
			}
			else
			{
				m_CurrentAnimIndex++;
				for (int i = 0; i < m_NumObjects; i++)
				{
					//Textures with Hover but no animation
					if (m_TexIDs[i].animation == nullptr && m_TexIDs[i].hover != -1)
					{
						Object2D* obj = m_Object[i];
						if (m_TexIDs[i].hover != -1) //has hover
						{
							obj->m_GUITex = FileManager::GetMenuGUITexture(m_TexIDs[i].hover);
							obj->m_Tex = obj->m_GUITex->texture;
							obj->m_Renderable->m_Meshes[0]->m_Material->SetTexture(obj->m_Tex);
						}//else no hover, just continue
						continue;
					}

					//hover and animation
					if (m_CurrentAnimIndex >= m_TexIDs[i].animation->size() - 1)
					{
						m_Reversing = true;
						m_CurrentAnimIndex = m_TexIDs[i].animation->size() - 2;
						if (m_CurrentAnimIndex < 0)m_CurrentAnimIndex = 0;
					}
					Object2D* obj = m_Object[i];
					obj->m_GUITex = FileManager::GetMenuGUITexture(m_TexIDs[i].animation->at(m_CurrentAnimIndex));
					obj->m_Tex = obj->m_GUITex->texture;
					obj->m_Renderable->m_Meshes[0]->m_Material->SetTexture(obj->m_Tex);
				}
			}
		}
		for (size_t i = 0; i < 3; i++)
		{
			m_FontObjects[i]->SetVisibility(false);
		}
		m_FontObjects[1]->SetVisibility(true);
	}
	else
	{
		m_AnimTime = 0;
		for (int i = 0; i < m_NumObjects; i++)
		{
			Object2D* obj = m_Object[i];
			obj->m_GUITex = FileManager::GetMenuGUITexture(m_TexIDs[i].normal);
			obj->m_Tex = obj->m_GUITex->texture;
			obj->m_Renderable->m_Meshes[0]->m_Material->SetTexture(obj->m_Tex);
		}

		for (size_t i = 0; i < 3; i++)
		{
			m_FontObjects[i]->SetVisibility(false);
		}
		m_FontObjects[0]->SetVisibility(true);
	}
	if (m_IsDisabled)
	{
		for (int i = 0; i < m_NumObjects; i++)
		{
			Object2D* obj = m_Object[i];
			obj->m_GUITex = FileManager::GetMenuGUITexture(m_TexIDs[i].normal);
			obj->m_Tex = obj->m_GUITex->texture;
			obj->m_Renderable->m_Meshes[0]->m_Material->SetTexture(obj->m_Tex);
		}
		for (size_t i = 0; i < 3; i++)
		{
			m_FontObjects[i]->SetVisibility(false);
		}
		m_FontObjects[2]->SetVisibility(true);
	}
	return isColliding;
}
void Themp::GUIButton::SetPosition(float x, float y, float z)
{
	m_CenterPosX = x;
	m_CenterPosY = y;
	for (int i = 0; i < m_NumObjects; i++)
	{
		m_Object[i]->m_Renderable->m_Position.x = m_TexIDs[i].xOffset + x;
		m_Object[i]->m_Renderable->m_Position.y = m_TexIDs[i].yOffset + y;
		m_Object[i]->m_Renderable->m_Position.z = m_TexIDs[i].zOffset + z;
	}
	for (size_t i = 0; i < 3; i++)
	{
		m_FontObjects[i]->m_ScreenObj->m_Renderable->SetPosition(x, y,z - 0.05);
	}
}
