#include "ThempSystem.h"

#include "ThempD3D.h"
#include "ThempGame.h"
#include "ThempAudio.h"

#include "ThempResources.h"
#include "ThempGUI.h"

#include <imgui.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cstdarg>
#include <sys/timeb.h>
#include <shellapi.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
IMGUI_API LRESULT ImGui_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void ImGui_PrepareFrame();

namespace Themp
{
	float lerp(float x, float y, float t)
	{
		return x * (1.0 - t) + y * t;
	}


	System* System::tSys = nullptr;
	FILE* System::logFile = nullptr;
	void System::Start()
	{
		m_D3D = new Themp::D3D();
		m_Audio = new Themp::Audio();
		m_Game = new Themp::Game();
		m_Resources = new Themp::Resources();
		tSys->m_Quitting = !m_D3D->Init();
		if (tSys->m_Quitting) { MessageBox(m_Window, L"Failed to initialise all required D3D11 resources, Is your hardware supported?", L"ThempSystem - Critical Error", MB_OK); }
		
		m_GUI = new Themp::GUI(m_Window);

		m_Game->Start();

		Timer mainTimer;
		Timer tickTimer;
		Timer drawTimer;
		mainTimer.StartTime();
		tickTimer.StartTime();
		drawTimer.StartTime();
		double trackerTime = 0;
		int numSamples = 0;
		double frameTimeAdd = 0, tickTimeAdd=0;

		RECT windowRect,clientRect;
		GetWindowRect(m_Window, &windowRect);
		GetClientRect(m_Window, &clientRect);

		m_D3D->ResizeWindow(clientRect.right, clientRect.bottom);
		DEVMODE dm = {0};
		dm.dmSize = sizeof(DEVMODE);
		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
		//printf("BorderX: %i\n BorderY: %i\n Caption: %i\n", borderX, borderY, caption);
		SetCursorPos(windowRect.left + (windowRect.right - windowRect.left) / 2, windowRect.top + (windowRect.bottom - windowRect.top) / 2);
		ImGuiIO& io = ImGui::GetIO();
		double totalDelta = 0;
		double time = 0;

		ShowCursor(false);
		while (!tSys->m_Quitting)
		{
			double delta = mainTimer.GetDeltaTimeReset();
			//System::Print("Total Delta was: %lf", delta);
			totalDelta += delta;
			time += delta;
			trackerTime += delta;

			//for (size_t i = 0; i < 256; i++)
			//{
			//	uint16_t keystate = GetAsyncKeyState(i) & 0x8000;
			//	int8_t currentKey = m_Game->m_Keys[i];
			//	if (keystate)//pressed
			//	{
			//		m_Game->m_Keys[i] = currentKey == 0 ? 2 : 1;
			//	}
			//	else //unpressed
			//	{
			//		m_Game->m_Keys[i] = currentKey > 0 ? -1 : 0;
			//	}
			//}

			MSG msg;
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				if (msg.message == WM_QUIT)
				{
					tSys->m_Quitting = true;
				}
				if (msg.message == WM_LBUTTONDOWN)
				{
					if (m_Game->m_Keys[256] <= 0)
					{
						m_Game->m_Keys[256] = 2;
					}
				}
				if (msg.message == WM_LBUTTONUP)
				{
					m_Game->m_Keys[256] = -1;
				}
				if (msg.message == WM_RBUTTONDOWN)
				{
					if (m_Game->m_Keys[257] <= 0)
					{
						m_Game->m_Keys[257] = 2;
					}
					//if (m_CursorShown)
					//{
					//	//SetCursorPos(windowRect.left + (windowRect.right - windowRect.left) / 2, windowRect.top + (windowRect.bottom - windowRect.top) / 2);
					//	m_CursorShown = false;
					//	ShowCursor(m_CursorShown);
					//}
				}
				if (msg.message == WM_RBUTTONUP)
				{
					m_Game->m_Keys[257] = -1;
					//if (!m_CursorShown)
					//{
					//	m_CursorShown = true;
					//	ShowCursor(m_CursorShown);
					//}
				}
				if (msg.message == WM_KEYDOWN)
				{
					if (msg.wParam > 256)continue;
					if (m_Game->m_Keys[msg.wParam] <= 0)
					{
						m_Game->m_Keys[msg.wParam] = 2;
					}
				}
				if (msg.message == WM_KEYUP)
				{
					if (msg.wParam > 256)continue;
					m_Game->m_Keys[msg.wParam] = -1;
				}
			}
			const float targetFPS = dm.dmDisplayFrequency;
			if (totalDelta > 1.0 / targetFPS)
			{
				GetWindowRect(m_Window, &windowRect);
				io = ImGui::GetIO();
				
				//sadly we need all these calls
				GetClientRect(m_Window, &clientRect);
				GetCursorPos(&m_Game->m_CursorPos);
				int windowDiffX = (windowRect.right - windowRect.left - clientRect.right) / 2;
				int windowDiffY = (int)((windowRect.bottom - windowRect.top - clientRect.bottom) * 0.75);
				int WindowedMouseX = m_Game->m_CursorPos.x - windowRect.left - windowDiffX;
				int WindowedMouseY = m_Game->m_CursorPos.y - windowRect.top - windowDiffY;
				io.DeltaTime = (float)totalDelta;
				m_Game->m_CursorWindowedX = WindowedMouseX;
				m_Game->m_CursorWindowedY = WindowedMouseY;

				//windows Title bar messes up the actual mouse position for collision testing with the UI, so I adjust it to fit "good enough" since getting exact measurements from top and bottom is a pain
				io.MousePos = ImVec2(WindowedMouseX, WindowedMouseY);
				io.DisplaySize = ImVec2((float)clientRect.right, (float)clientRect.bottom);

				ImGui_PrepareFrame();
				//finally we can start our important stuff, ImGui::NewFrame is needed for ImGui so it can get set up for any UI calls we make
				ImGui::NewFrame();

				tickTimer.StartTime();
				m_Game->Update(totalDelta);
				tickTimeAdd += tickTimer.GetDeltaTimeReset();
				for (size_t i = 0; i < 258; i++)
				{
					int8_t currentKey = m_Game->m_Keys[i];
					if (currentKey == 2)//pressed
					{
						m_Game->m_Keys[i] = 1;
					}
					else if (currentKey == -1)//unpressed
					{
						m_Game->m_Keys[i] = 0;
					}
				}

				drawTimer.StartTime();
				//Doesn't actually render but prepares render data for us to use
				ImGui::Render();
				m_D3D->m_ConstantBufferData.time = time;
				m_D3D->dirtySystemBuffer = true;
				m_D3D->PrepareSystemBuffer();
				m_D3D->Draw(*m_Game);
				m_D3D->DrawImGUI();
				frameTimeAdd += drawTimer.GetDeltaTimeReset();

				numSamples++;
				if (trackerTime >= 1.0)
				{
					System::Print("Avg FPS: %5i  Avg Frametime: %.5f   Avg Tick Time: %.5f", numSamples, frameTimeAdd / (float)numSamples, tickTimeAdd/(float)numSamples);
					trackerTime = trackerTime - 1.0;
					frameTimeAdd = 0;
					tickTimeAdd = 0;
					numSamples = 0;
				}
				totalDelta = totalDelta - (1.0 / targetFPS);
			}
		}

		GetWindowRect(m_Window, &windowRect);
		m_SVars[SVAR_SCREENWIDTH] = windowRect.right - windowRect.left;
		m_SVars[SVAR_SCREENHEIGHT] = windowRect.bottom - windowRect.top;


		m_Game->Stop();
		delete m_Game;
		m_Game = nullptr;
		Themp::System::tSys->m_Game = nullptr;
		delete m_Resources;
		m_Resources = nullptr;
		delete m_GUI;
		m_GUI = nullptr;
		delete m_D3D;
		m_D3D = nullptr;
		ShowCursor(true);
		ImGui::DestroyContext();
	}
}

int newWindowSizeX = 0;
int newWindowSizeY = 0;

std::string GetPathName(std::string s)
{
	std::string name = "";
	int64_t size = s.size() - 1;
	for (int64_t i = size; i >= 0; i--)
	{
		if (s.at(i) == '\\' || s[i] == '/')
		{
			i++;
			name = s.substr(0, i);
			break;
		}
	}
	return name;
}



int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,	LPSTR lpCmdLine,int nCmdShow)
{
	AllocConsole();
	FILE* conout = freopen("CONOUT$", "w", stdout);

	Themp::System::tSys = new Themp::System();
	Themp::System* tSys = Themp::System::tSys;

	char szFileName[MAX_PATH + 1];
	GetModuleFileNameA(NULL, szFileName, MAX_PATH + 1);
	tSys->m_BaseDir = GetPathName(std::string(szFileName));

	Themp::System::logFile = fopen("log.txt", "w+");
	std::ifstream configFile("config.ini");
	std::string line;
	if (configFile.is_open())
	{
		while (std::getline(configFile, line))
		{
			size_t cIndex = line.find(" ", 0);
			if (cIndex != std::string::npos)
			{
				tSys->m_SVars[line.substr(0, cIndex)] = std::stof(line.substr(cIndex + 1, line.size() - (cIndex + 1)));
			}
		}
		configFile.close();
	}
	else
	{
		Themp::System::Print("Could not find config.ini, creating");
		std::ofstream nConfig("config.ini");
		if (nConfig.is_open())
		{
			nConfig << "Fullscreen 0\n";
			nConfig << "WindowPosX 0\n";
			nConfig << "WindowPosY 0\n";
			nConfig << "WindowSizeX 1024\n";
			nConfig << "WindowSizeY 900\n";
			nConfig << "Anisotropic_Filtering 1\n";
			nConfig << "Multisample 1\n";

			nConfig.close();
		}
		tSys->m_SVars[std::string(SVAR_FULLSCREEN)] = 0;
		tSys->m_SVars[std::string(SVAR_WINDOWPOSX)] = 0;
		tSys->m_SVars[std::string(SVAR_WINDOWPOSY)] = 0;
		tSys->m_SVars[std::string(SVAR_SCREENWIDTH)] = 1024;
		tSys->m_SVars[std::string(SVAR_SCREENHEIGHT)] = 800;
		tSys->m_SVars[std::string(SVAR_ANISOTROPIC_FILTERING)] = 1;
		tSys->m_SVars[std::string(SVAR_MULTISAMPLE)] = 1;
	}
	
	//check whether all values exist: (in case of outdated config.ini)
	if (tSys->m_SVars.find(SVAR_FULLSCREEN) == tSys->m_SVars.end()) { tSys->m_SVars[std::string(SVAR_FULLSCREEN)] = 0; }
	if (tSys->m_SVars.find(SVAR_WINDOWPOSX) == tSys->m_SVars.end()) { tSys->m_SVars[std::string(SVAR_WINDOWPOSX)] = 0; }
	if (tSys->m_SVars.find(SVAR_WINDOWPOSY) == tSys->m_SVars.end()) { tSys->m_SVars[std::string(SVAR_WINDOWPOSY)] = 0; }
	if (tSys->m_SVars.find(SVAR_SCREENWIDTH) == tSys->m_SVars.end()) { tSys->m_SVars[std::string(SVAR_SCREENWIDTH)] = 1024; }
	if (tSys->m_SVars.find(SVAR_SCREENHEIGHT) == tSys->m_SVars.end()) { tSys->m_SVars[std::string(SVAR_SCREENHEIGHT)] = 800; }
	if (tSys->m_SVars.find(SVAR_ANISOTROPIC_FILTERING) == tSys->m_SVars.end()) { tSys->m_SVars[std::string(SVAR_ANISOTROPIC_FILTERING)] = 1; }
	if (tSys->m_SVars.find(SVAR_MULTISAMPLE) == tSys->m_SVars.end()) { tSys->m_SVars[std::string(SVAR_MULTISAMPLE)] = 1; }
	
	ImGui::CreateContext();
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW; 
	WORD icon = 0;
	wc.hIcon = ExtractAssociatedIcon(hInstance, L"KEEPER95.exe", &icon);
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	if(tSys->m_SVars.find(SVAR_FULLSCREEN)->second == 1)
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"Dungeon Keeper";

	RegisterClassEx(&wc);

	if (tSys->m_SVars.find(SVAR_FULLSCREEN)->second == 1)
	{
		HWND desktop = GetDesktopWindow();
		RECT bSize;
		GetWindowRect(desktop, &bSize);

		tSys->m_SVars.find(SVAR_SCREENWIDTH)->second = static_cast<float>(bSize.right);
		tSys->m_SVars.find(SVAR_SCREENHEIGHT)->second = static_cast<float>(bSize.bottom);
		tSys->m_Window = CreateWindowEx(NULL,
			L"Dungeon Keeper",
			L"Dungeon Keeper",
			WS_EX_TOPMOST,
			bSize.left,
			bSize.top,
			bSize.right,
			bSize.bottom,
			NULL, NULL, hInstance, NULL);
	}
	else
	{
		tSys->m_Window = CreateWindowEx(NULL,
			L"Dungeon Keeper",
			L"Dungeon Keeper",
			WS_OVERLAPPEDWINDOW,
			static_cast<int>(tSys->m_SVars.find(SVAR_WINDOWPOSX)->second ),
			static_cast<int>(tSys->m_SVars.find(SVAR_WINDOWPOSY)->second ),
			static_cast<int>(tSys->m_SVars.find(SVAR_SCREENWIDTH)->second),
			static_cast<int>(tSys->m_SVars.find(SVAR_SCREENHEIGHT)->second),
			NULL, NULL, hInstance, NULL);
	}
	RECT winRect;

	newWindowSizeX = static_cast<int>(tSys->m_SVars.find(SVAR_SCREENWIDTH)->second);
	newWindowSizeY = static_cast<int>(tSys->m_SVars.find(SVAR_SCREENHEIGHT)->second);

	winRect.left = 0;
	winRect.right = newWindowSizeX;
	winRect.top = 0;
	winRect.bottom = newWindowSizeY;

	//We have to recalculate and rescale the window because window size doesn't equal render (client) size, so windows will rescale the render targets
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, true);
	SetWindowPos(tSys->m_Window, 0, static_cast<int>(tSys->m_SVars.find(SVAR_WINDOWPOSX)->second), static_cast<int>(tSys->m_SVars.find(SVAR_WINDOWPOSY)->second), winRect.right, winRect.bottom, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
	ShowWindow(tSys->m_Window, nCmdShow);

	//reset all values again
	newWindowSizeX = winRect.right;
	newWindowSizeY = winRect.bottom;

	ImGuiIO& imgIo = ImGui::GetIO();
	imgIo.DisplaySize.x = (float)winRect.right;
	imgIo.DisplaySize.y = (float)winRect.bottom;

	imgIo.KeyMap[ImGuiKey_Tab] = VK_TAB;                       // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array that we will update during the application lifetime.
	imgIo.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
	imgIo.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
	imgIo.KeyMap[ImGuiKey_UpArrow] = VK_UP;
	imgIo.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
	imgIo.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
	imgIo.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
	imgIo.KeyMap[ImGuiKey_Home] = VK_HOME;
	imgIo.KeyMap[ImGuiKey_End] = VK_END;
	imgIo.KeyMap[ImGuiKey_Insert] = VK_INSERT;
	imgIo.KeyMap[ImGuiKey_Delete] = VK_DELETE;
	imgIo.KeyMap[ImGuiKey_Backspace] = VK_BACK;
	imgIo.KeyMap[ImGuiKey_Space] = VK_SPACE;
	imgIo.KeyMap[ImGuiKey_Enter] = VK_RETURN;
	imgIo.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
	imgIo.KeyMap[ImGuiKey_A] = 'A';
	imgIo.KeyMap[ImGuiKey_C] = 'C';
	imgIo.KeyMap[ImGuiKey_V] = 'V';
	imgIo.KeyMap[ImGuiKey_X] = 'X';
	imgIo.KeyMap[ImGuiKey_Y] = 'Y';
	imgIo.KeyMap[ImGuiKey_Z] = 'Z';
	imgIo.ImeWindowHandle = tSys->m_Window;

	tSys->Start();

	std::ofstream nConfig("config.ini");
	if (nConfig.is_open())
	{
		for (std::map<std::string,float>::iterator i = tSys->m_SVars.begin(); i != tSys->m_SVars.end(); i++)
		{
			nConfig << i->first << " " << i->second << std::endl;
		}
		nConfig.close();
	}

	fclose(Themp::System::logFile);
	fclose(conout);
	delete tSys;
	return 0;
}

void Themp::System::Print(const char* message, ...)
{
	size_t strLength = strlen(message);
	int fmtMsgSize = strLength < 128 ? 256 : strLength * 2;
	char* buffer = new char[fmtMsgSize];
	memset(buffer, 0, fmtMsgSize);
	std::string timestamp;
	char* msg = new char[fmtMsgSize];
	timeb t;
	ftime(&t);
	strftime(msg, fmtMsgSize, "[%T", localtime(&t.time));
	timestamp.insert(0, msg);
	timestamp.append(":");
	memset(msg, 0, 4);
	short msVal = t.millitm;
	timestamp.append(itoa(t.millitm, msg, 10));
	timestamp.append(msVal < 10 ? "00] " : (msVal < 100 ? "0] " : "] "));
	va_list args;
	va_start(args, message);
	vsnprintf(buffer, fmtMsgSize, message, args);
	//snprintf(buffer, fmtMsgSize, message, args);
	va_end(args);
	timestamp.append(buffer);
	timestamp.append("\n");
	printf("%s", timestamp.c_str());
	delete[] buffer;
	delete[] msg;
	if (logFile)
	{
		fwrite(timestamp.c_str(), timestamp.size(), 1, logFile);
		fflush(logFile);
	}
}
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	ImGui_WndProcHandler(hWnd, message, wParam, lParam);
	ImGuiIO& imgIo = ImGui::GetIO();

	if (message == WM_SIZE)
	{
		if (wParam == SIZE_MAXIMIZED)
		{
			Themp::System::tSys->m_SVars[std::string(SVAR_WINDOWPOSX)] = 0;
			Themp::System::tSys->m_SVars[std::string(SVAR_WINDOWPOSY)] = 0;
		}
		if (wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED)
		{
			RECT windowRect;
			GetClientRect(Themp::System::tSys->m_Window, &windowRect);

			newWindowSizeX = windowRect.right;
			newWindowSizeY = windowRect.bottom;

			imgIo.DisplaySize.x = (float)newWindowSizeX;
			imgIo.DisplaySize.y = (float)newWindowSizeY;
			if (Themp::System::tSys->m_D3D)
			{
				Themp::System::tSys->m_D3D->ResizeWindow(newWindowSizeX, newWindowSizeY);
			}
		}
	}

	RECT windowRect;
	switch (message)
	{
		case WM_CLOSE:
		{
			Themp::System::tSys->m_Quitting = true;
		}break;
		case WM_DESTROY:
		{
			Themp::System::tSys->m_Quitting = true;
		}break;
		case WM_TIMER:
		{
			if (Themp::System::tSys->m_D3D)
			{
				GetClientRect(Themp::System::tSys->m_Window, &windowRect);
				newWindowSizeX = windowRect.right;
				newWindowSizeY = windowRect.bottom;
				imgIo.DisplaySize.x = (float)windowRect.right;
				imgIo.DisplaySize.y = (float)windowRect.bottom;
				Themp::System::tSys->m_D3D->ResizeWindow(newWindowSizeX, newWindowSizeY);


				ImGui_PrepareFrame();
				ImGui::NewFrame();
				Themp::System::tSys->m_Game->Update(0.01);
				ImGui::Render();
				RedrawWindow(hWnd, 0, 0, RDW_UPDATENOW | RDW_FRAME  | RDW_INVALIDATE);
			}
		}break;
		case WM_MOVING:
		{
			GetWindowRect(Themp::System::tSys->m_Window, &windowRect);
			Themp::System::tSys->m_SVars[std::string(SVAR_WINDOWPOSX)] = windowRect.left;
			Themp::System::tSys->m_SVars[std::string(SVAR_WINDOWPOSY)] = windowRect.top;
		}break;
		case WM_PAINT:
			if (Themp::System::tSys->m_D3D && Themp::System::tSys->m_D3D->m_DevCon)
			{
				Themp::System::tSys->m_D3D->PrepareSystemBuffer();
				Themp::System::tSys->m_D3D->Draw(*Themp::System::tSys->m_Game);
				Themp::System::tSys->m_D3D->DrawImGUI();
			}
			break;
		case WM_ENTERSIZEMOVE:
			SetTimer(hWnd, 1, USER_TIMER_MINIMUM, 0);
		break;
		case WM_EXITSIZEMOVE:
		{
			//we're done resizing the window, now resize all the rendering resources
			GetClientRect(Themp::System::tSys->m_Window, &windowRect);
			newWindowSizeX = windowRect.right;
			newWindowSizeY = windowRect.bottom;
			imgIo.DisplaySize.x =(float)windowRect.right;
			imgIo.DisplaySize.y =(float)windowRect.bottom;
			Themp::System::tSys->m_D3D->ResizeWindow(newWindowSizeX, newWindowSizeY);
			KillTimer(hWnd, 1);
		}break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

static void ImGui_UpdateMouseCursor()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGuiMouseCursor imgui_cursor = io.MouseDrawCursor ? ImGuiMouseCursor_None : ImGui::GetMouseCursor();
	if (imgui_cursor == ImGuiMouseCursor_None)
	{
		// Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
		::SetCursor(NULL);
	}
	else
	{
		// Hardware cursor type
		LPTSTR win32_cursor = IDC_ARROW;
		switch (imgui_cursor)
		{
		case ImGuiMouseCursor_Arrow:        win32_cursor = IDC_ARROW; break;
		case ImGuiMouseCursor_TextInput:    win32_cursor = IDC_IBEAM; break;
		case ImGuiMouseCursor_ResizeAll:    win32_cursor = IDC_SIZEALL; break;
		case ImGuiMouseCursor_ResizeEW:     win32_cursor = IDC_SIZEWE; break;
		case ImGuiMouseCursor_ResizeNS:     win32_cursor = IDC_SIZENS; break;
		case ImGuiMouseCursor_ResizeNESW:   win32_cursor = IDC_SIZENESW; break;
		case ImGuiMouseCursor_ResizeNWSE:   win32_cursor = IDC_SIZENWSE; break;
		}
		::SetCursor(::LoadCursor(NULL, win32_cursor));
	}
}
IMGUI_API LRESULT ImGui_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui::GetCurrentContext() == NULL)
		return 0;

	ImGuiIO& io = ImGui::GetIO();
	switch (msg)
	{
	case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
	{
		int button = 0;
		if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK) button = 0;
		if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK) button = 1;
		if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK) button = 2;
		if (!ImGui::IsAnyMouseDown() && ::GetCapture() == NULL)
			::SetCapture(hwnd);
		io.MouseDown[button] = true;
		return 0;
	}
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	{
		int button = 0;
		if (msg == WM_LBUTTONUP) button = 0;
		if (msg == WM_RBUTTONUP) button = 1;
		if (msg == WM_MBUTTONUP) button = 2;
		io.MouseDown[button] = false;
		if (!ImGui::IsAnyMouseDown() && ::GetCapture() == hwnd)
			::ReleaseCapture();
		return 0;
	}
	case WM_MOUSEWHEEL:
		io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
		return 0;
	case WM_MOUSEHWHEEL:
		io.MouseWheelH += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
		return 0;
	case WM_MOUSEMOVE:
		io.MousePos.x = (signed short)(lParam);
		io.MousePos.y = (signed short)(lParam >> 16);
		return 0;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (wParam < 256)
			io.KeysDown[wParam] = 1;
		return 0;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		if (wParam < 256)
			io.KeysDown[wParam] = 0;
		return 0;
	case WM_CHAR:
		// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
		if (wParam > 0 && wParam < 0x10000)
			io.AddInputCharacter((unsigned short)wParam);
		return 0;
	case WM_SETCURSOR:
		if (LOWORD(lParam) == HTCLIENT)
		{
			ImGui_UpdateMouseCursor();
			return 1;
		}
		return 0;
	}
	return 0;
}
ImGuiMouseCursor oldCursor;
void ImGui_PrepareFrame()
{
	ImGuiIO& io = ImGui::GetIO();

	// Read keyboard modifiers inputs
	io.KeyCtrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
	io.KeyShift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
	io.KeyAlt = (GetKeyState(VK_MENU) & 0x8000) != 0;
	io.KeySuper = false;
	// io.KeysDown : filled by WM_KEYDOWN/WM_KEYUP events
	// io.MousePos : filled by WM_MOUSEMOVE events
	// io.MouseDown : filled by WM_*BUTTON* events
	// io.MouseWheel : filled by WM_MOUSEWHEEL events

	// Set OS mouse position if requested last frame by io.WantMoveMouse flag (used when io.NavMovesTrue is enabled by user and using directional navigation)
	if (io.WantMoveMouse)
	{
		POINT pos = { (int)io.MousePos.x, (int)io.MousePos.y };
		ClientToScreen(Themp::System::tSys->m_Window, &pos);
		SetCursorPos(pos.x, pos.y);
	}

	// Update OS mouse cursor with the cursor requested by imgui
	ImGuiMouseCursor mouse_cursor = io.MouseDrawCursor ? ImGuiMouseCursor_None : ImGui::GetMouseCursor();
	if (oldCursor != mouse_cursor)
	{
		oldCursor = mouse_cursor;
		ImGui_UpdateMouseCursor();
	}

	// Start the frame. This call will update the io.WantCaptureMouse, io.WantCaptureKeyboard flag that you can use to dispatch inputs (or not) to your application.
	//ImGui::NewFrame();
}