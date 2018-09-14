#include "ThempSystem.h"
#include "ThempD3D.h"
#include "ThempObject3D.h"
#include "ThempGame.h"
#include "ThempResources.h"
#include "ThempMaterial.h"
#include "ThempMesh.h"
#include "ThempRenderTexture.h"
#include "ThempGUI.h"
#include "ThempCamera.h"
#include "ThempDebugDraw.h"
#include <imgui.h>
#include <iostream>
#include <fstream>

using namespace DirectX;

namespace Themp
{
	XMFLOAT3 Normalize(const XMFLOAT3& v)
	{
		XMVECTOR x = XMLoadFloat3(&v);
		x = XMVector3Normalize(x);
		XMFLOAT3 r;
		XMStoreFloat3(&r, x);
		return r;
	}
	XMFLOAT3 XMFLOAT3Add(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		return XMFLOAT3(a.x + b.x, a.y + b.y, a.z + b.z);
	}
	D3D11_INPUT_ELEMENT_DESC D3D::DefaultInputLayoutDesc[] =
	{
		{ "POSITION" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3D* D3D::s_D3D = nullptr;

	uint32_t D3D::DefaultInputLayoutNumElements = 5;
	Material* D3D::DefaultMaterial;
	ID3D11SamplerState* D3D::DefaultTextureSampler;
	ID3D11SamplerState* D3D::DefaultTextureSamplerFiltered;


	//0 object
	//1 camera
	//2 system
	//3 material
	//4 light
	ID3D11Buffer* D3D::ConstantBuffers[5];
	bool D3D::Init()
	{
		s_D3D = this;
		//create swap chain
		DXGI_SWAP_CHAIN_DESC scd;

		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));


		scd.BufferCount = 2;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		RECT windowRect;
		GetClientRect(Themp::System::tSys->m_Window, &windowRect);

		int windowWidth = windowRect.right;
		int windowHeight = windowRect.bottom;

		scd.Windowed = !static_cast<UINT>(Themp::System::tSys->m_SVars.find(SVAR_FULLSCREEN)->second);
		if (scd.Windowed)
		{
			scd.BufferDesc.Width = windowRect.right;
			scd.BufferDesc.Height = windowRect.bottom;
			scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		}
		else
		{
			scd.BufferDesc.Width = GetSystemMetrics(SM_CXSCREEN);
			scd.BufferDesc.Height = GetSystemMetrics(SM_CYSCREEN);
			scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		}

		scd.OutputWindow = Themp::System::tSys->m_Window;
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
		};
		HRESULT result;
#ifdef _DEBUG
		result = D3D11CreateDeviceAndSwapChain(NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL, D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT, featureLevels, 3,
			D3D11_SDK_VERSION,
			&scd,
			&m_Swapchain,
			&m_Device, NULL,
			&m_DevCon);
#else 
		result = D3D11CreateDeviceAndSwapChain(NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, featureLevels, 3,
			D3D11_SDK_VERSION,
			&scd,
			&m_Swapchain,
			&m_Device, NULL,
			&m_DevCon);
#endif 


		if (result != S_OK) { System::Print("Could not create D3D11 Device and/or swapchain."); return false; }
		int fl = m_Device->GetFeatureLevel();
		System::Print("FeatureLevel: %s", (fl == D3D_FEATURE_LEVEL_11_1 ? "11_1" : fl == D3D_FEATURE_LEVEL_11_0 ? "11_0" : "10_1"));

#ifdef _DEBUG	
		result = m_Device->QueryInterface(&m_DebugInterface);
		result = m_DebugInterface->QueryInterface(&m_D3dInfoQueue);
		if (SUCCEEDED(result))
		{
			D3D11_MESSAGE_ID hide[] =
			{
				D3D11_MESSAGE_ID_DEVICE_DRAW_SAMPLER_NOT_SET,
				// TODO: Add more message IDs here as needed 
			};
			D3D11_INFO_QUEUE_FILTER filter;
			memset(&filter, 0, sizeof(filter));
			filter.DenyList.NumIDs = _countof(hide);
			filter.DenyList.pIDList = hide;
			m_D3dInfoQueue->AddStorageFilterEntries(&filter);
		}
#endif

		int multisample = Themp::System::tSys->m_SVars[SVAR_MULTISAMPLE];
		if (multisample == 0) multisample = 1;
		if (!CreateBackBuffer() || !CreateDepthStencil(windowWidth, windowHeight, multisample))
		{
			System::Print("Could not initialise all required resources, shutting down");
			return false;
		}


		SetViewPort(0.0f, 0.0f, (float)windowWidth, (float)windowHeight);

		//create default material's and other data
		D3D11_SAMPLER_DESC texSamplerDesc;
		texSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		texSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		texSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		texSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		texSamplerDesc.MipLODBias = 0.0f;
		texSamplerDesc.MaxAnisotropy = static_cast<UINT>(Themp::System::tSys->m_SVars.find(SVAR_ANISOTROPIC_FILTERING)->second);
		texSamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		texSamplerDesc.BorderColor[0] = 0;
		texSamplerDesc.BorderColor[1] = 0;
		texSamplerDesc.BorderColor[2] = 0;
		texSamplerDesc.BorderColor[3] = 0;
		texSamplerDesc.MinLOD = 0;
		texSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		result = m_Device->CreateSamplerState(&texSamplerDesc, &D3D::DefaultTextureSampler);
		texSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		result = m_Device->CreateSamplerState(&texSamplerDesc, &D3D::DefaultTextureSamplerFiltered);

		D3D11_RASTERIZER_DESC rDesc;
		memset(&rDesc, 0, sizeof(D3D11_RASTERIZER_DESC));
		rDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID; //change for Wireframe
		rDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK; //Backface culling yes/no/inverted
		rDesc.DepthClipEnable = true; //default true

		m_Device->CreateRasterizerState(&rDesc, &m_RasterizerState);

		rDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		m_Device->CreateRasterizerState(&rDesc, &m_ShadowRasterizerState);

		rDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		rDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		m_Device->CreateRasterizerState(&rDesc, &m_WireframeRasterizerState);

		m_DevCon->RSSetState(m_RasterizerState);
		
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.AlphaToCoverageEnable = false;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		m_Device->CreateBlendState(&desc, &m_BlendState);
		const float blend_factor[4] = { 0.f, 0.f, 0.f, 0.f };
		m_DevCon->OMSetBlendState(m_BlendState, blend_factor, 0xffffffff);
		
		
		
		std::vector<std::string> defaultTextures = {
			"DefaultDiffuse.dds",
			"",
			"",
			"",
		};
		std::vector<uint8_t> defaultTypes = { 1,((uint8_t)(-1)),((uint8_t)(-1)),((uint8_t)(-1)) };
		D3D::DefaultMaterial = Resources::TRes->GetMaterial("G-Buffer", defaultTextures, defaultTypes, "",  false);

		defaultTypes[0] = Material::DIFFUSE;
		defaultTypes[1] = Material::NORMALS;
		defaultTypes[2] = Material::PBR;
		defaultTypes[3] = Material::UNKNOWN;
		defaultTextures[0] = "DefaultDiffuse.dds";
		defaultTextures[1] = "DefaultNormal.dds";
		defaultTextures[2] = "DefaultPBR.dds";
		defaultTextures[3] = "DefaultMisc.dds";

		System::Print("D3D11 Initialisation success!");

		m_FullScreenQuad = new Object3D();
		m_FullScreenQuad->CreateQuad("ScreenSpace", false);

#ifdef _DEBUG
		DebugDraw::DefaultLineMaterial = Resources::TRes->GetMaterial("DebugLine", "", "DebugLine", false, DebugDraw::DefaultLineInputLayoutDesc, 2, false);
#endif // _DEBUG

		//m_ShadowUnfiltered = new ShadowUnfiltered();
		//m_ShadowPCF = new ShadowPCF();
		//m_ShadowCascade = new ShadowCascade(m_ConstantBufferData.num_cascades);
		//m_ShadowCascade->SetCascade(m_ConstantBufferData.num_cascades, 0.1, 500);
		//m_ShadowVariance = new ShadowVariance();
		//m_ShadowMoment = new ShadowMoment();

		SetMultiSample(multisample);
		return true;
	}

	void D3D::ResizeWindow(int newX, int newY)
	{
		if (m_Swapchain)
		{
			m_DevCon->OMSetRenderTargets(0, 0, 0);

			// Release all outstanding references to the swap chain's buffers.
			CLEAN(m_BackBuffer);

			//Recreate back buffer
			HRESULT hr;
			// Preserve the existing buffer count and format.
			// Automatically choose the width and height to match the client rect for HWNDs.
			hr = m_Swapchain->ResizeBuffers(0, newX, newY, DXGI_FORMAT_UNKNOWN, 0);
			if (hr != S_OK)
			{
				System::Print("failed to resize buffers");
			}

			// Get buffer and create a render-target-view.
			ID3D11Texture2D* pBuffer;
			hr = m_Swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D),
				(void**)&pBuffer);
			if (hr != S_OK)
			{
				System::Print("failed to obtain backbuffer texture");
			}

			hr = m_Device->CreateRenderTargetView(pBuffer, NULL, &m_BackBuffer);
			if (hr != S_OK)
			{
				System::Print("failed to create rendertarget view from backbuffer");
			}
			pBuffer->Release();
			
			CLEAN(m_DepthStencil);
			CLEAN(m_DepthStencilView);

			D3D11_TEXTURE2D_DESC depthBufferDesc;
			ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
			depthBufferDesc.SampleDesc.Count = 1;
			depthBufferDesc.SampleDesc.Quality = 0;

			depthBufferDesc.Width = newX;
			depthBufferDesc.Height = newY;
			depthBufferDesc.MipLevels = 1;
			depthBufferDesc.ArraySize = 1;
			depthBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS; // DXGI_FORMAT_D16_UNORM;
			depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			depthBufferDesc.CPUAccessFlags = 0;
			depthBufferDesc.MiscFlags = 0;
			hr = m_Device->CreateTexture2D(&depthBufferDesc, NULL, &m_DepthStencil);
			if (hr != S_OK)
			{
				System::Print("failed to create depth texture");
			}

			D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
			memset(&descDSV, 0, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
			descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			descDSV.Texture2D.MipSlice = 0;

			// Create the depth stencil view
			hr = m_Device->CreateDepthStencilView(m_DepthStencil, // Depth stencil texture
				&descDSV, // Depth stencil desc
				&m_DepthStencilView);  // [out] Depth stencil view
			if (hr != S_OK)
			{
				System::Print("Could not create Depthstencil view");
				return;
			}

			// Set up the viewport.
			D3D11_VIEWPORT vp;
			vp.Width = (float)newX;
			vp.Height = (float)newY;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0.0f;
			vp.TopLeftY = 0.0f;
			m_DevCon->RSSetViewports(1, &vp);
			Themp::System::tSys->m_SVars[SVAR_SCREENWIDTH] = vp.Width;
			Themp::System::tSys->m_SVars[SVAR_SCREENHEIGHT] = vp.Height;
			m_ConstantBufferData.screenHeight = vp.Width;
			m_ConstantBufferData.screenWidth = vp.Height;
			m_ConstantBufferData.shadow_atlas_size = 4096;
			dirtySystemBuffer = true;
		}
	}

	
	bool D3D::SetMultiSample(int num)
	{
		if (num != 0 && num != 1 && num != 2 && num != 4 && num != 8)
		{
			System::Print("Multiplesample value (\"num\") has to be 0, 1, 2, 4 or 8, given: %i. Nothing has changed", num);
			return false;
		}
		if (num == 0)
		{
			System::Print("Multisample value (\"num\") == 0, this is allowed because it's set to 1, but no real MS value of 0 exists");
			num = 1;
		}
		if (m_Swapchain)
		{
			m_DevCon->OMSetRenderTargets(0, 0, 0);
			dirtySystemBuffer = true;
		}
		else
		{
			return false;
		}
		m_ConstantBufferData.MSAAValue = num;
		dirtySystemBuffer = true;

		return true;
	}
	
	void D3D::DrawGBufferPass(Game & game)
	{
		Themp::D3D& _this = *static_cast<Themp::D3D*>(this);
		if (m_Wireframe)
		{
			m_DevCon->RSSetState(m_WireframeRasterizerState);
		}
		else
		{
			m_DevCon->RSSetState(m_RasterizerState);
		}
		m_DevCon->OMSetRenderTargets(1, &m_BackBuffer, m_DepthStencilView);
		m_DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_DevCon->IASetInputLayout(D3D::DefaultMaterial->m_InputLayout); //skybox layout doesn't differ so this is fine
		
		//Models
		m_DevCon->OMSetDepthStencilState(m_DepthStencilState, 1);
		m_DevCon->PSSetShaderResources(0, 4, D3D::DefaultMaterial->m_Views);

		m_DevCon->PSSetShader(D3D::DefaultMaterial->m_PixelShader, 0, 0);
		m_DevCon->VSSetShader(D3D::DefaultMaterial->m_VertexShader, 0, 0);
		m_DevCon->GSSetShader(D3D::DefaultMaterial->m_GeometryShader, 0, 0);

		for (int i = 0; i < game.m_Objects3D.size(); ++i)
		{
			game.m_Objects3D[i]->Draw(_this);
		}

#ifdef _DEBUG
		DebugDraw::Draw(m_Device, m_DevCon);
#endif // _DEBUG

	}
	Camera* shadowCamera; 

	void D3D::Draw(Themp::Game& game)
	{
		static const float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

		m_DevCon->ClearRenderTargetView(m_BackBuffer, ClearColor);
		m_DevCon->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH, 1.0f, 0);

		//Draw all geometry to the GBuffer //positions, normals, etc
		DrawGBufferPass(game);
		
		//set stuff back to prevent issues next draw
		VSUploadConstantBuffersToGPUNull();
		PSUploadConstantBuffersToGPUNull();
		GSUploadConstantBuffersToGPUNull();
		for (size_t i = 0; i < 32; i++)
		{
			m_ShaderResourceViews[i] = nullptr;
		}
		m_DevCon->PSSetShaderResources(0, 32, m_ShaderResourceViews);
	}
	void D3D::DrawImGUI()
	{
		ImDrawData* drawData = ImGui::GetDrawData();
		GUI::gui->PrepareDraw(drawData);
		GUI::gui->Draw(drawData);
		GUI::gui->EndDraw();
		m_Swapchain->Present(0, 0);
	}
	D3D::~D3D()
	{
#if _DEBUG
		DebugDraw::Destroy();
#endif

		if (Themp::System::tSys->m_SVars.find(SVAR_FULLSCREEN)->second == 1)
		{
			m_Swapchain->SetFullscreenState(FALSE, NULL);  // switch to windowed mode
		}
		CLEAN(m_CBuffer);
		CLEAN(D3D::ConstantBuffers[0]);
		CLEAN(D3D::ConstantBuffers[1]);
		CLEAN(D3D::ConstantBuffers[2]);
		CLEAN(D3D::DefaultTextureSampler);
		CLEAN(D3D::DefaultTextureSamplerFiltered);

		VSUploadConstantBuffersToGPUNull();
		PSUploadConstantBuffersToGPUNull();
		GSUploadConstantBuffersToGPUNull();

		CLEAN(m_OMBlendState);
		CLEAN(m_DepthStencil);
		CLEAN(m_DepthStencilView);
		CLEAN(m_DepthStencilState);
		CLEAN(m_SkyboxDepthStencilState);
		CLEAN(m_ShadowClearDepthStencilState);
		CLEAN(m_RasterizerState);
		CLEAN(m_ShadowRasterizerState);
		CLEAN(m_BlendState);
		CLEAN(m_WireframeRasterizerState);
		CLEAN(m_InputLayout);
		CLEAN(m_Swapchain);
		CLEAN(m_BackBuffer);
		CLEAN(m_Device);
		CLEAN(m_DevCon);

#ifdef _DEBUG

		CLEAN(m_D3dInfoQueue);

#if ReportLiveObjects
		if (m_DebugInterface)
		{
			m_DebugInterface->ReportLiveDeviceObjects(D3D11_RLDO_FLAGS::D3D11_RLDO_DETAIL);
		}
#endif
		CLEAN(m_DebugInterface);
#endif
	}

	void D3D::SetViewPort(float xPos, float yPos, float width, float height)
	{
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = xPos;
		viewport.TopLeftY = yPos;
		viewport.Width = width;
		viewport.Height = height;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1.0f;

		m_DevCon->RSSetViewports(1, &viewport);
	}
	void D3D::SetObject3DConstantBuffer(ID3D11Buffer* buf)
	{
		D3D::ConstantBuffers[0] = buf;
	}
	void D3D::SetCameraConstantBuffer(ID3D11Buffer* buf)
	{
		D3D::ConstantBuffers[1] = buf;
	}
	void D3D::SetSystemConstantBuffer(ID3D11Buffer* buf)
	{
		D3D::ConstantBuffers[2] = buf;
	}
	void D3D::SetMaterialConstantBuffer(ID3D11Buffer* buf)
	{
		D3D::ConstantBuffers[3] = buf;
	}
	void D3D::SetLightConstantBuffer(ID3D11Buffer* buf)
	{
		D3D::ConstantBuffers[4] = buf;
	}
	void D3D::VSUploadConstantBuffersToGPU()
	{
		if (D3D::ConstantBuffers[1] == nullptr) System::Print("No Camera active in scene");
		m_DevCon->VSSetConstantBuffers(0, 3, D3D::ConstantBuffers);
	}
	void D3D::VSUploadConstantBuffersToGPUNull()
	{
		D3D::ConstantBuffers[0] = nullptr;
		D3D::ConstantBuffers[1] = nullptr;
		D3D::ConstantBuffers[2] = nullptr;
		D3D::ConstantBuffers[3] = nullptr;
		D3D::ConstantBuffers[4] = nullptr;
		m_DevCon->VSSetConstantBuffers(0, 5, D3D::ConstantBuffers);
	}
	void D3D::PSUploadConstantBuffersToGPUNull()
	{
		ID3D11Buffer* buffers[5] = { nullptr,nullptr,nullptr,nullptr,nullptr };
		m_DevCon->PSSetConstantBuffers(0, 5, buffers);
	}
	void D3D::GSUploadConstantBuffersToGPUNull()
	{
		ID3D11Buffer* buffers[5] = { nullptr,nullptr,nullptr,nullptr,nullptr };
		m_DevCon->GSSetConstantBuffers(0, 5, buffers);
	}
	void D3D::PSUploadConstantBuffersToGPU()
	{
		m_DevCon->PSSetConstantBuffers(0, 5, D3D::ConstantBuffers);
	}
	void D3D::GSUploadConstantBuffersToGPU()
	{
		m_DevCon->GSSetConstantBuffers(0, 5, D3D::ConstantBuffers);
	}
	bool D3D::CreateBackBuffer()
	{
		HRESULT result;
		////get back buffer
		ID3D11Texture2D *pBackBuffer;
		result = m_Swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (result != S_OK) { System::Print("Could not obtain BackBuffer"); return false; }
		// use the back buffer address to create the render target
		D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
		ZeroMemory(&RTVDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		RTVDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
		RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		result = m_Device->CreateRenderTargetView(pBackBuffer, NULL, &m_BackBuffer);
		if (result != S_OK) { System::Print("Could not create Render target (BackBuffer)"); return false; }
		pBackBuffer->Release();
		return true;
	}
	bool D3D::CreateDepthStencil(int width, int height,int multisample)
	{
		HRESULT result;
		SetMultiSample(multisample);
		
		D3D11_DEPTH_STENCIL_DESC dsDesc;

		// Depth test parameters
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

		// Stencil test parameters
		dsDesc.StencilEnable = false;
		dsDesc.StencilReadMask = 0xFF;
		dsDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create depth stencil state
		result = m_Device->CreateDepthStencilState(&dsDesc, &m_DepthStencilState);
		if (result != S_OK)
		{
			System::Print("Could not create Depthstencil state");
			return false;
		}
		m_DevCon->OMSetDepthStencilState(m_DepthStencilState, 1);

		
		D3D11_DEPTH_STENCIL_DESC dssDesc;
		ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		dssDesc.DepthEnable = true;
		dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		result = m_Device->CreateDepthStencilState(&dssDesc, &m_SkyboxDepthStencilState);
		if (result != S_OK)
		{
			System::Print("Could not create Skybox Depthstencil state");
			return false;
		}

		ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		dssDesc.DepthEnable = true;
		dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dssDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		dssDesc.StencilEnable = false;
		dssDesc.StencilReadMask = 0xFF;
		dssDesc.StencilWriteMask = 0xFF;

		result = m_Device->CreateDepthStencilState(&dssDesc, &m_ShadowClearDepthStencilState);
		if (result != S_OK)
		{
			System::Print("Could not create ShadowClear Depthstencil state");
			return false;
		}
		return true;
	}
	void D3D::PrepareSystemBuffer()
	{
		SetSystemConstantBuffer(m_CBuffer);
		if (!dirtySystemBuffer)return;

		// Supply the vertex shader constant data.
		m_ConstantBufferData.screenWidth = Themp::System::tSys->m_SVars[SVAR_SCREENWIDTH];
		m_ConstantBufferData.screenHeight = Themp::System::tSys->m_SVars[SVAR_SCREENHEIGHT];
		if (!m_CBuffer)
		{
			// Fill in a buffer description.
			D3D11_BUFFER_DESC cbDesc;
			cbDesc.ByteWidth = sizeof(CONSTANT_BUFFER);
			cbDesc.Usage = D3D11_USAGE_DYNAMIC;
			cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cbDesc.MiscFlags = 0;
			cbDesc.StructureByteStride = 0;

			// Fill in the subresource data.
			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = &m_ConstantBufferData;
			InitData.SysMemPitch = 0;
			InitData.SysMemSlicePitch = 0;

			// Create the buffer.
			m_Device->CreateBuffer(&cbDesc, &InitData, &m_CBuffer);
		}
		else
		{
			D3D11_MAPPED_SUBRESOURCE ms;
			m_DevCon->Map(m_CBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
			memcpy(ms.pData, &m_ConstantBufferData, sizeof(CONSTANT_BUFFER));
			m_DevCon->Unmap(m_CBuffer, NULL);
		}
		dirtySystemBuffer = false;
		SetSystemConstantBuffer(m_CBuffer);
	}
	
};