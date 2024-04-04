// dllmain.cpp : Defines the entry point for the DLL application.
#include "header.h"
#include <ctime>
//
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
WNDPROC oWndProc;
const char windowName[] = "Rules of Survival 1.295523.315160";
//const char windowName[] = "VN Rules of Survival";

// Menu
bool menuShow = false;
bool isPaused = false;
static const char* aimKey[] = { "LSHIFT", "LBUTTON", "RBUTTON" };
static const char* currentAimKey = aimKey[0];
static const char* weapon[] = { "LV1", "LV2", "LV3", "ALL" };
static const char* currentWeapon = weapon[0];
static bool AIMBOT = false;
static bool ESP_PLAYER = false;
static bool ESP_WEAPON = false;
static bool ESP_VEHICLE = false;
static bool ESP_DISTANCE = false;
static float ESPPlayerColor[3]{ 1.0f, 0.f, 0.f };
static float ESPVehicleColor[3]{ 0.0f, 1.0f, 0.f };
//
bool SAVED_AIMBOT = false;
bool SAVED_ESP_PLAYER = false;
bool SAVED_ESP_WEAPON = false;
bool SAVED_ESP_VEHICLE = false;
bool SAVED_ESP_DISTANCE = false;
//
static int myTeamNum = 1;
static int botNum = 0;
//
#define DEBUG 0
typedef HRESULT(__stdcall *EndScene_t)(LPDIRECT3DDEVICE9 pDevice);
EndScene_t EndScene_orig = nullptr;
//
typedef HRESULT(APIENTRY* Present_t) (IDirect3DDevice9*, const RECT *, const RECT *, HWND, const RGNDATA *);
Present_t Present_orig = nullptr;
//
typedef HRESULT(APIENTRY *Reset_t)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
Reset_t Reset_orig = 0;
//
HMODULE Hand;
CDraw Draw;
// 


LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (menuShow && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

void Loop()
{
	// my Location
	static D3DVECTOR localPos{ 0.f, 0.f, 0.f };
	botNum = 0;
	/////////////////
	fDecrypt oriDec = nullptr;
	DWORD base = (DWORD)GetModuleHandle(0);
	oriDec = (fDecrypt)(base + Offset::fDec);
	BYTE inGame = *(BYTE*)(base + Offset::inGame);
	if (inGame == 0)
		return;
	ClientApp* m_ClientApp = *(ClientApp**)(base + Offset::clientApp_ptr);
	D3DXMATRIX viewMatrix = *(D3DXMATRIX*)(*(DWORD*)(*(DWORD*)(base + Offset::sceneContext) + 0x4) + 0xC4);

	auto m_pFirst = m_ClientApp->m_pFirst;
	auto m_pStart = m_pFirst->m_pStart;
	int entityCount = 0;
	std::vector<D3DVECTOR> aimbot;
	D3DVECTOR aimAt{ 0.0f, 0.0f, 0.0f };
	D3DVECTOR center{ Draw.Screen.Width / 2, Draw.Screen.Height / 2, 0.0f };
	float Min = 99999999.f;
	
	//////////////////
	do
	{
		if (!m_pStart->m_pEntity)
			break;
		auto entity = oriDec(m_ClientApp, m_pStart->m_ObjectId, 0);
		//printf("%p \n", entity);
		CEntity* Ent;
		if (entity != 0)
		{
			Ent = (CEntity*)entity;
			//viewMatrix = *(D3DXMATRIX*)(*(DWORD*)(*(DWORD*)(base + Offset::sceneContext) + 0x4) + 0xC4);
			SceneContext* m_SceneContext = *(SceneContext**)(base + Offset::sceneContext);
			viewMatrix = m_SceneContext->m_pCamera->m_worldMatrix;
			D3DVECTOR pos, out, out_2;
			pos.x = decryptFloat(Ent->EncryptedX, Ent->XKey->Val);
			pos.y = decryptFloat(Ent->EncryptedY, Ent->YKey->Val);
			pos.z = decryptFloat(Ent->EncryptedZ, Ent->ZKey->Val);
			std::string name = std::string(Ent->m_pMeta->pName);
			float EntHeight = 14.0f;
			//std::cout << name << std::endl;
			bool isLocal = (name == "PlayerBattleGroundAvatar");
			bool isBot = (name == "Robot");
			bool isPlayer = (name == "BattleGroundAvatar");
			bool isWpOnGround = (name == "DtsProp");
			bool isVehicle = (name.find("Land") != -1);
			bool isHeritage = (name == "DtsPlayerHeritage");
			bool isUnknown = (!isLocal) && (!isBot) && (!isPlayer) && (!isWpOnGround) && (!isVehicle) && (!isHeritage);
			bool isTeamMate = false;

			if (isLocal)
			{
				localPos.x = pos.x, localPos.y = pos.y, localPos.z = pos.z;
			}
			if (isBot)
			{
				botNum++;
			}
			if (isPlayer)
			{
				int pose = Ent->m_Table->pTable->m_PoseRow->mVal;
				EntHeight = (pose == 0) ? 14.0f : (pose == 1) ? 11.5f : 3.5f;
			}
			//std::cout << Ent->m_Table->pTable->m_NickRow->m_NickName << std::endl;
		

			if (W2S(pos, out, viewMatrix, Draw.Screen.Width, Draw.Screen.Height))
			{
				if (!isUnknown)
				{
					pos.x = decryptFloat(Ent->EncryptedX, Ent->XKey->Val);
					pos.y = decryptFloat(Ent->EncryptedY, Ent->YKey->Val) + (isBot || isPlayer || isVehicle ? EntHeight : 3.0f);
					pos.z = decryptFloat(Ent->EncryptedZ, Ent->ZKey->Val);
					W2S(pos, out_2, viewMatrix, Draw.Screen.Width, Draw.Screen.Height);
					float height = abs(out_2.y - out.y);
					float width = height / 2;
					int x = out_2.x - width / 2;
					int y = out_2.y;
					D3DCOLOR color = 0;
					if (isPlayer)
					{
						int enemyTeamNum = Ent->m_Table->pTable->m_TeamRow->mVal;
						isTeamMate = enemyTeamNum == myTeamNum;
					}
					if (!isTeamMate && (isPlayer || isBot) && (center.x - out_2.x)*(center.x - out_2.x) + (center.y - out_2.y) * (center.y - out_2.y) < Min)
					{
						Min = (center.x - out_2.x)*(center.x - out_2.x) + (center.y - out_2.y)*(center.y - out_2.y);
						aimAt.x = out_2.x;
						aimAt.y = out_2.y;
					}
					//if (isTeamMate)
					//{
					//	std::cout << Ent << std::endl;
					//}
					if (isBot || isPlayer) color = D3DCOLOR_XRGB((int)(ESPPlayerColor[0] * 255.0f), (int)(ESPPlayerColor[1] * 255.0f), (int)(ESPPlayerColor[2] * 255.0f));
					else if (isWpOnGround || isHeritage) color = BLUE;
					else if (isVehicle) color = GREEN;
					else if (isUnknown) color = YELLOW;
					if (ESP_PLAYER && (isPlayer || isBot))
					{
						if (isTeamMate)
							color = ORANGE;
						Draw.ESPBox(x, y, width, height, 1, color);
						if (isBot)
							Draw.Text(x, y + height + 5, color, 3, "BOT");
					}
					if (0 && isHeritage)
					{
						Draw.ESPBox(x, y, width, height, 1, color);
					}
					if (ESP_WEAPON && isWpOnGround)
					{
						int m_val = Ent->m_Table->pTable->Prop->mVal;
						int level = (currentWeapon == weapon[0]) ? 1 : (currentWeapon == weapon[1]) ? 2 : (currentWeapon == weapon[2]) ? 3 : 0;
						if (getItemLevel(m_val) >= level)
							Draw.Text(x, y + height + 5, color, 3, getItem(m_val));
					}
					if (ESP_VEHICLE && isVehicle)
					{
						Draw.ESPBox(x, y, width, height, 1, color);
					}
					if (ESP_DISTANCE && (isPlayer || isBot || isVehicle))
					{
						Draw.Text(x, y - 5, color, 3, "%.f", distance(pos, localPos) / 10.f);
					}
					//Draw.ESPBox(x, y, width, height, 1, color);
					//Draw.Text(x, y + height + 5, lefted, 3, false, color, 0, "%s", name.c_str());
				}
			}
			entityCount++;
		}
		else break;
		m_pStart = m_pStart->m_pNext;

	} while (m_pStart != m_ClientApp->m_pFirst->m_pEnd);
	short m_aimKey = 0;
	m_aimKey = (currentAimKey == aimKey[0]) ? VK_LSHIFT : (currentAimKey == aimKey[1]) ? VK_LBUTTON : VK_RBUTTON;
	if (GetAsyncKeyState(m_aimKey) && AIMBOT)
	{
		printf("0x%08X\n", (*(DWORD*)(*(DWORD*)(base + Offset::sceneContext) + 0x4) + 0xC4));
		if ((aimAt.x < 0.01f && aimAt.y < 0.01f) || Min == 99999999.f)
			return;
		mouse_event(MOUSEEVENTF_MOVE, aimAt.x - center.x, aimAt.y - center.y, 0, 0);
		Sleep(1);
	}
}

HRESULT APIENTRY Present_hook(IDirect3DDevice9* pDevice, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion)
{

	Draw.GetDevice(pDevice);
	Draw.Reset();
	static bool initFonts = false;


	if (initFonts == false)
	{
		Draw.AddFont("Arial", 15, false, false);
		Draw.AddFont("Verdana", 15, true, false);
		Draw.AddFont("Verdana", 13, true, false);
		Draw.AddFont("Comic Sans MS", 15, true, false);
		initFonts = true;
	}

	Loop();
	//DrawMenu();
	return Present_orig(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT APIENTRY Reset_hook(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	auto hr = Reset_orig(pDevice, pPresentationParameters);
	ImGui_ImplDX9_CreateDeviceObjects();

	return hr;
}

HRESULT APIENTRY EndScene_Hook(LPDIRECT3DDEVICE9 pDevice)
{
	static bool init = true;
	if (init)
	{
		init = false;
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplWin32_Init(FindWindowA(NULL, windowName));
		ImGui_ImplDX9_Init(pDevice);
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	if (menuShow)
	{
		ImGui::Begin("ROSZone Hack v1.0");
		///////////////////////
		ImGui::Text("Aimbot");
		ImGui::SameLine();
		ImGui::Checkbox("##0", &AIMBOT);
		ImGui::SameLine(ImGui::GetWindowWidth() - 90);
		ImGui::PushItemWidth(-3);
		if (ImGui::BeginCombo("##1", currentAimKey, 0))
		{
			for (int n = 0; n < 3; n++)
			{
				bool is_selected = (currentAimKey == aimKey[n]); // You can store your selection however you want, outside or inside your objects
				if (ImGui::Selectable(aimKey[n], is_selected))
					currentAimKey = aimKey[n];
				if (is_selected)
					ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
			}
			ImGui::EndCombo();
		}

		/////////////////////////////////
		ImGui::Text("ESP Player");
		ImGui::SameLine();
		ImGui::Checkbox("##2", &ESP_PLAYER);
		ImGui::SameLine();
		ImGui::PushItemWidth(-3);
		ImGui::ColorEdit3("", (float*)&ESPPlayerColor, 0);
		///////////////////////////////////
		ImGui::Text("ESP Weapon");
		ImGui::SameLine();
		ImGui::Checkbox("##3", &ESP_WEAPON);
		ImGui::SameLine(ImGui::GetWindowWidth() - 90);
		ImGui::PushItemWidth(-3);
		if (ImGui::BeginCombo("##4", currentWeapon, 0))
		{
			for (int n = 0; n < 4; n++)
			{
				bool is_selected = (currentWeapon == weapon[n]); // You can store your selection however you want, outside or inside your objects
				if (ImGui::Selectable(weapon[n], is_selected))
					currentWeapon = weapon[n];
				if (is_selected)
					ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
			}
			ImGui::EndCombo();
		}
		/////////////////////////////////
		ImGui::Text("ESP Vehicle");
		ImGui::SameLine();
		ImGui::Checkbox("##5", &ESP_VEHICLE);
		ImGui::SameLine();
		ImGui::PushItemWidth(-3);
		ImGui::ColorEdit3("##6", (float*)&ESPVehicleColor, 0);
		/////////////////////////////////
		ImGui::Text("ESP Distance");
		ImGui::SameLine();
		ImGui::Checkbox("##7", &ESP_DISTANCE);

		////////////////////////////////
		ImGui::Text("MyTeam = %d, BotCount = %d", myTeamNum, botNum);
		ImGui::Text("FPS = %.1f", ImGui::GetIO().Framerate);
		ImGui::Text("Author: facebook.com/Trung149");
		ImGui::End();
	}

	ImGui::EndFrame();
	ImGui::Render();

	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	return EndScene_orig(pDevice);
}

DWORD WINAPI InternalHack(LPVOID lpParameter)
{
	HWND  window = FindWindowA(NULL, windowName);

	oWndProc = (WNDPROC)SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)WndProc);

	HMODULE dDll = NULL;
	while (!dDll)
	{
		dDll = GetModuleHandleA("d3d9.dll");
		Sleep(100);
	}
	CloseHandle(dDll);

	IDirect3D9* d3d = NULL;
	IDirect3DDevice9* d3ddev = NULL;

	HWND tmpWnd = CreateWindowA("BUTTON", "Foobar2000 v1.4.1", WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 300, 300, NULL, NULL, Hand, NULL);
	if (tmpWnd == NULL)
	{
		printf("HOOKED not ok 1\n");
		return 0;
	}

	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d == NULL)
	{
		DestroyWindow(tmpWnd);
		printf("HOOKED not ok 2\n");
		return 0;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = tmpWnd;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	HRESULT result = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, tmpWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);
	if (result != D3D_OK)
	{
		d3d->Release();
		DestroyWindow(tmpWnd);
		printf("HOOKED not ok 3\n");
		return 0;
	}

	// We have the device, so walk the vtable to get the address of all the dx functions in d3d9.dll
#if defined _M_X64
	DWORD64* dVtable = (DWORD64*)d3ddev;
	dVtable = (DWORD64*)dVtable[0];
#elif defined _M_IX86
	DWORD* dVtable = (DWORD*)d3ddev;
	dVtable = (DWORD*)dVtable[0]; // == *d3ddev
#endif

	if (MH_Initialize() != MH_OK) { printf("HOOKED not ok 4\n"); return 1; }
	if (MH_CreateHook((DWORD_PTR*)dVtable[42], &EndScene_Hook, reinterpret_cast<void**>(&EndScene_orig)) != MH_OK) { printf("HOOKED not ok 5\n");  return 1; }
	if (MH_EnableHook((DWORD_PTR*)dVtable[42]) != MH_OK) { printf("HOOKED not ok 6\n"); return 1; }
	if (MH_CreateHook((DWORD_PTR*)dVtable[17], &Present_hook, reinterpret_cast<void**>(&Present_orig)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)dVtable[17]) != MH_OK) { return 1; }
	if (MH_CreateHook((DWORD_PTR*)dVtable[16], &Reset_hook, reinterpret_cast<void**>(&Reset_orig)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)dVtable[16]) != MH_OK) { return 1; }

	d3ddev->Release();
	d3d->Release();
	DestroyWindow(tmpWnd);
	printf("HOOKED ok\n");
	return 1;
}

void saveCfg()
{
	//SAVED_AIMBOT = AIMBOT;
	SAVED_ESP_PLAYER = ESP_PLAYER;
	SAVED_ESP_WEAPON = ESP_WEAPON;
	SAVED_ESP_VEHICLE = ESP_VEHICLE;
	SAVED_ESP_DISTANCE = ESP_DISTANCE;

	//AIMBOT = false;
	ESP_PLAYER = false;
	ESP_WEAPON = false;
	ESP_VEHICLE = false;
	ESP_DISTANCE = false;

	menuShow = false;
}

void loadCfg()
{
	//AIMBOT = SAVED_AIMBOT;
	ESP_PLAYER = SAVED_ESP_PLAYER;
	ESP_WEAPON = SAVED_ESP_WEAPON;
	ESP_VEHICLE = SAVED_ESP_VEHICLE;
	ESP_DISTANCE = SAVED_ESP_DISTANCE;
}

void kbHandle()
{
	bool keydown = false;
	while (true)
	{
		if (GetAsyncKeyState(VK_MBUTTON))
		{
			if (!keydown)
			{
				keydown = true;
				if (isPaused)
				{
					isPaused = false;
					loadCfg();
				}
				else 
					menuShow = !menuShow;
			}
		}
		else if (GetAsyncKeyState(VK_LCONTROL))
		{
			if (!keydown)
			{
				keydown = true;
				if (!isPaused)
				{
					isPaused = true;
					saveCfg();
				}
			}
		}
		else if (GetAsyncKeyState(VK_UP))
		{
			if (!keydown)
			{
				keydown = true;
				myTeamNum++;
				std::cout << myTeamNum << std::endl;
			}
		}
		else if (GetAsyncKeyState(VK_DOWN))
		{
			if (!keydown)
			{
				keydown = true;
				myTeamNum--;
				std::cout << myTeamNum << std::endl;
			}
		}
		else
			keydown = false;
		Sleep(1);
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		Hand = hModule;
		DisableThreadLibraryCalls(hModule);
		if (!DEBUG)
		{
			AllocConsole();
			freopen("CONOUT$", "w", stdout);
		}
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)InternalHack, 0, 0, 0);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)kbHandle, 0, 0, 0);
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

