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


std::unordered_map<Object2D::Source,std::unordered_map<int, GUIButton::ButtonAnim>> buttonAnimations =
{
	{
		Object2D::Source::sMENU_MAIN,
		{
			{ 1, GUIButton::ButtonAnim({ 13,10,7,4,1 },true,10) },
			{ 2, GUIButton::ButtonAnim({ 14,11,8,5,2 },true,10) },
			{ 3, GUIButton::ButtonAnim({ 15,12,9,6,3 },true,10) },
			//{15, GUIButton::ButtonAnim(true,{ 17,19		}) },
		},
	},
	{ 
		Object2D::Source::sMENU_LEVELFLAG,
		{
			{ 1, GUIButton::ButtonAnim({ 1,2,3,4 },false,5) },
			{ 6, GUIButton::ButtonAnim({ 5,6,7,8 },false,5) },
		}
	},
};

Themp::GUIButton::~GUIButton()
{
	for (size_t i = 0; i < m_Objects.size(); i++)
	{
		delete m_Objects[i];
	}
	for (size_t i = 0; i < m_FontObjects.size(); i++)
	{
		delete m_FontObjects[i];
	}
}

//source is Object2D::Source
GUIButton::GUIButton(int source, int textureIDNormal, int textureIDHover, int textureIDClick, bool hiRes)
{
	m_Source = source;
	m_Objects.push_back(new Object2D((Object2D::Source)source, textureIDNormal, hiRes));
	Themp::System::tSys->m_Game->AddObject3D(m_Objects[0]->m_Renderable);
	ButtonInfo texID;
	texID.click = textureIDClick;
	texID.hover = textureIDHover;
	texID.normal = textureIDNormal;
	texID.xOffset = 0;
	texID.yOffset = 0;
	texID.zOffset = 0;
	auto&& it = buttonAnimations.find(((Object2D::Source)source));
	if (it != buttonAnimations.end())
	{
		auto&& itNormal = it->second.find(textureIDNormal);
		if (itNormal != it->second.end())
		{
			texID.normalAnimation = &itNormal->second;
		}
		else
		{
			texID.normalAnimation = nullptr;
		}
		auto&& itHover = it->second.find(textureIDHover);
		if (itHover != it->second.end())
		{
			texID.hoverAnimation = &itHover->second;
		}
		else
		{
			texID.hoverAnimation = nullptr;
		}
	}
	else
	{
		texID.normalAnimation = nullptr;
		texID.hoverAnimation = nullptr;
	}
	m_TexIDs.push_back(texID);
	SetSize(1, 1, 1, 1, 0);
}

Themp::GUIButton::GUIButton(int source, int* textureIDNormal, int* textureIDHover, int* textureIDClick, float * xOffsets, float * yOffsets, float * zOffsets, int numIDs, bool hiRes, std::string text, int textFontNormal, int texttFontHover, int textFontDisabled)
{
	m_Source = source;
	for (size_t i = 0; i < numIDs; i++)
	{
		m_Objects.push_back(new Object2D((Object2D::Source)source, textureIDNormal[i], hiRes));
		Themp::System::tSys->m_Game->AddObject3D(m_Objects[i]->m_Renderable);
		ButtonInfo texID;
		texID.click = textureIDClick[i];
		texID.hover = textureIDHover[i];
		texID.normal = textureIDNormal[i];
		texID.xOffset = xOffsets[i];
		texID.yOffset = yOffsets[i];
		texID.zOffset = zOffsets[i];

		auto&& it = buttonAnimations.find(((Object2D::Source)source));
		if (it != buttonAnimations.end())
		{
			auto&& itNormal = it->second.find(textureIDNormal[i]);
			if (itNormal != it->second.end())
			{
				texID.normalAnimation = &itNormal->second;
			}
			else
			{
				texID.normalAnimation = nullptr;
			}
			auto&& itHover = it->second.find(textureIDHover[i]);
			if (itHover != it->second.end())
			{
				texID.hoverAnimation = &itHover->second;
			}
			else
			{
				texID.hoverAnimation = nullptr;
			}
		}
		else
		{
			texID.normalAnimation = nullptr;
			texID.hoverAnimation = nullptr;
		}
		m_TexIDs.push_back(texID);
		SetSize(1.0f, 1.0f, 1.0f, 1.0f, i);
	}
	if (text.size() > 0)
	{
		m_FontObjects.push_back(new Themp::Font(text, (Font::FontTexID)textFontNormal, false, DirectX::XMFLOAT3(m_CenterPosX, m_CenterPosY, zOffsets[0])));
		m_FontObjects.push_back(new Themp::Font(text, (Font::FontTexID)texttFontHover, false, DirectX::XMFLOAT3(m_CenterPosX, m_CenterPosY, zOffsets[0])));
		m_FontObjects.push_back(new Themp::Font(text, (Font::FontTexID)textFontDisabled, false, DirectX::XMFLOAT3(m_CenterPosX, m_CenterPosY, zOffsets[0])));
	}
}


//size in multiplier
void GUIButton::SetSize(float W,float H, float textW, float textH, int index)
{
	Texture* t = m_Objects[index]->m_Tex;
	float originalW = (float)t->m_Width  / 640.0f; //original screen width
	float originalH = (float)t->m_Height / 480.0f; //original screen height
	float newW = originalW*2.0f;// *4.0 ;//* System::tSys->m_SVars[SVAR_SCREENWIDTH];
	float newH = originalH*2.0f;// *4.0 ;//* System::tSys->m_SVars[SVAR_SCREENHEIGHT];

	m_Objects[index]->m_Renderable->SetScale(W * newW,H * newH,1);

	m_TexIDs[index].xScale = newW * W;
	m_TexIDs[index].yScale = newH * H;

	for (size_t i = 0; i < m_FontObjects.size(); i++)
	{
		m_FontObjects[i]->SetScale(textW ,textH, 1.0f);
	}

	//m_Object[index]->m_Renderable->m_Scale.x = W * 2.0;
	//m_Object[index]->m_Renderable->m_Scale.y = H * 2.0;
	//m_FontObjects[index]->SetScale(W, H, 1);
}

void Themp::GUIButton::SetVisibility(bool val)
{
	for (int i = 0; i < m_Objects.size(); i++)
	{
		m_Objects[i]->SetVisibility(val);
	}
	for (size_t i = 0; i < m_FontObjects.size(); i++)
	{
		m_FontObjects[i]->SetVisibility(val);
	}
	m_IsVisible = val;
}

bool GUIButton::Update(float delta, float mouseX, float mouseY, int lmbuttonState, GUIButton* InitialClick)
{
	bool didCollide = false;
	for (int i = 0; i < m_Objects.size(); i++)
	{
		Object2D* obj = m_Objects[i];
		ButtonInfo& bInfo = m_TexIDs[i];
		if (mouseX < m_CenterPosX + bInfo.xOffset + (bInfo.xScale) && mouseX > m_CenterPosX + bInfo.xOffset - (bInfo.xScale))
		{
			if (mouseY < m_CenterPosY + bInfo.yOffset + (bInfo.yScale) && mouseY > m_CenterPosY + bInfo.yOffset - (bInfo.yScale))
			{
				if (!m_IsColliding)
				{
					for (size_t j = 0; j < m_Objects.size(); j++)
					{
						m_TexIDs[j].currentAnimIndex = 0;
						m_TexIDs[j].currentNormalTime = 0;
						m_TexIDs[j].currentHoverTime = m_TexIDs[i].hoverAnimation ? 1.0f / (float)m_TexIDs[i].hoverAnimation->fps : 0.0f;
					}
				}
				m_IsColliding = true;
				didCollide = true;
			}
		}
	}
	if (m_IsColliding && !didCollide)
	{
		for (size_t i = 0; i < m_Objects.size(); i++)
		{
			m_TexIDs[i].currentAnimIndex = 0;
			m_TexIDs[i].currentNormalTime = m_TexIDs[i].normalAnimation ? 1.0f / (float)m_TexIDs[i].normalAnimation->fps : 0.0f;
			m_TexIDs[i].currentHoverTime = 0;
		}
		m_IsColliding = false;
	}
	if (m_IsColliding)
	{
		for (int i = 0; i < m_Objects.size(); i++)
		{
			Object2D* obj = m_Objects[i];
			ButtonInfo& bInfo = m_TexIDs[i];
			if (bInfo.hoverAnimation)
			{
				bInfo.currentHoverTime += delta;
				if (bInfo.currentHoverTime > 1.0f / (float)bInfo.hoverAnimation->fps)
				{
					bInfo.currentHoverTime -= 1.0f / (float)bInfo.hoverAnimation->fps;
					if (bInfo.hoverAnimation->pingpong) //pingpong animation
					{
						if (bInfo.reversing)
						{
							bInfo.currentAnimIndex--;
							if (bInfo.currentAnimIndex <= 0)
							{
								bInfo.reversing = false;
								bInfo.currentAnimIndex = 0;
							}
							//hover and animation
							obj->SetTexture(obj->GetTexture((Object2D::Source)m_Source, bInfo.hoverAnimation->animation[bInfo.currentAnimIndex]));
						}
						else
						{
							bInfo.currentAnimIndex++;
							//hover and animation
							if (bInfo.currentAnimIndex >= bInfo.hoverAnimation->animation.size() - 1)
							{
								bInfo.reversing = true;
								bInfo.currentAnimIndex = (int)(bInfo.hoverAnimation->animation.size() - 1);
								if (bInfo.currentAnimIndex < 0)bInfo.currentAnimIndex = 0;
							}
							obj->SetTexture(obj->GetTexture((Object2D::Source)m_Source, bInfo.hoverAnimation->animation[bInfo.currentAnimIndex]));
						}
					}
					else
					{
						bInfo.currentAnimIndex++;
						if(bInfo.currentAnimIndex >= bInfo.hoverAnimation->animation.size())
						{
							bInfo.currentAnimIndex = 0;
						}
						obj->SetTexture(obj->GetTexture((Object2D::Source)m_Source, bInfo.hoverAnimation->animation[bInfo.currentAnimIndex]));
					}
				}
			}
			else
			{
				if (bInfo.hover != -1) //has hover
				{
					obj->SetTexture(obj->GetTexture((Object2D::Source)m_Source, bInfo.hover));
				}//else no hover, just continue
				continue;
			}
		}
		
		if (m_FontObjects.size())
		{
			for (size_t i = 0; i < m_FontObjects.size(); i++)
			{
				m_FontObjects[i]->SetVisibility(false);
			}
			m_FontObjects[1]->SetVisibility(true);
		}
	}
	else
	{
		for (int i = 0; i < m_Objects.size(); i++)
		{
			Object2D* obj = m_Objects[i];
			ButtonInfo& bInfo = m_TexIDs[i];
			if (bInfo.normalAnimation)
			{
				bInfo.currentNormalTime += delta;
				if (bInfo.currentNormalTime > 1.0f / (float)bInfo.normalAnimation->fps)
				{
					bInfo.currentNormalTime -= 1.0f / (float)bInfo.normalAnimation->fps;
					if (bInfo.normalAnimation->pingpong) //pingpong animation
					{
						if (bInfo.reversing)
						{
							bInfo.currentAnimIndex--;
							if (bInfo.currentAnimIndex <= 0)
							{
								bInfo.reversing = false;
								bInfo.currentAnimIndex = 0;
							}
							//normal and animation
							obj->SetTexture(obj->GetTexture((Object2D::Source)m_Source, bInfo.normalAnimation->animation[bInfo.currentAnimIndex]));
						}
						else
						{
							bInfo.currentAnimIndex++;
							//hover and animation
							if (bInfo.currentAnimIndex >= bInfo.normalAnimation->animation.size() - 1)
							{
								bInfo.reversing = true;
								bInfo.currentAnimIndex = (int)(bInfo.normalAnimation->animation.size() - 1);
								if (bInfo.currentAnimIndex < 0)bInfo.currentAnimIndex = 0;
							}
							obj->SetTexture(obj->GetTexture((Object2D::Source)m_Source, bInfo.normalAnimation->animation[bInfo.currentAnimIndex]));
						}
					}
					else
					{
						bInfo.currentAnimIndex++;
						if (bInfo.currentAnimIndex >= bInfo.normalAnimation->animation.size())
						{
							bInfo.currentAnimIndex = 0;
						}
						obj->SetTexture(obj->GetTexture((Object2D::Source)m_Source, bInfo.normalAnimation->animation[bInfo.currentAnimIndex]));
					}
				}
			}
			else
			{
				for (int i = 0; i < m_Objects.size(); i++)
				{
					Object2D* obj = m_Objects[i];
					obj->SetTexture(obj->GetTexture((Object2D::Source)m_Source, m_TexIDs[i].normal));
				}
			}
		}
		if (m_FontObjects.size())
		{
			for (size_t i = 0; i < m_FontObjects.size(); i++)
			{
				m_FontObjects[i]->SetVisibility(false);
			}
			m_FontObjects[0]->SetVisibility(true);
		}
	}
	if (m_IsDisabled)
	{
		for (int i = 0; i < m_Objects.size(); i++)
		{
			Object2D* obj = m_Objects[i];
			obj->SetTexture(obj->GetTexture((Object2D::Source)m_Source, m_TexIDs[i].normal));
		}
		if (m_FontObjects.size())
		{
			for (size_t i = 0; i < m_FontObjects.size(); i++)
			{
				m_FontObjects[i]->SetVisibility(false);
			}
			m_FontObjects[2]->SetVisibility(true);
		}
	}
	return m_IsDisabled ? false : m_IsColliding;
}
void Themp::GUIButton::SetPosition(const XMFLOAT3& pos)
{
	float x = pos.x;
	float y = pos.y;
	float z = pos.z;

	m_CenterPosX = x;
	m_CenterPosY = y;
	for (int i = 0; i < m_Objects.size(); i++)
	{
		m_Objects[i]->m_Renderable->SetPosition(m_TexIDs[i].xOffset + x, m_TexIDs[i].yOffset + y, m_TexIDs[i].zOffset + z);
	}
	for (size_t i = 0; i < m_FontObjects.size(); i++)
	{
		m_FontObjects[i]->m_ScreenObj->m_Renderable->SetPosition(x, y, z - 0.05f);
	}
}
void Themp::GUIButton::SetPosition(float x, float y, float z)
{
	m_CenterPosX = x;
	m_CenterPosY = y;
	for (int i = 0; i < m_Objects.size(); i++)
	{
		m_Objects[i]->m_Renderable->SetPosition(m_TexIDs[i].xOffset + x, m_TexIDs[i].yOffset + y, m_TexIDs[i].zOffset + z);
	}
	for (size_t i = 0; i < m_FontObjects.size(); i++)
	{
		m_FontObjects[i]->m_ScreenObj->m_Renderable->SetPosition(x, y,z - 0.05f);
	}
}
