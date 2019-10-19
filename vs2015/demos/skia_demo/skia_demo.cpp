// skia_demo.cpp : 定义应用程序的入口点。
//

#include <Windows.h>
#include "skia_demo.h"

#include "FrameWindowWnd.h"
#include "trace/SkEventTracing.h"

void globalInit();
void globalUnInit();

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	CWindowWnd::SetResourceInstance(hInstance);

    SkEventTracing::initializeEventTracingForTools("skia_demo.json");

	globalInit();
	
	{
		// 
		const size_t kMinChunkSize = 1024 * 1024;

		//PreReadImage(L"C:\\Windows\\System32\\dxgi.dll", 0, kMinChunkSize);
		//PreReadImage(L"C:\\Windows\\System32\\d3d11.dll", 0, kMinChunkSize);
		//PreReadImage(L"C:\\Windows\\System32\\dcomp.dll", 0, kMinChunkSize);
	}

#if USE_NVAPI
	{
		nv_api nvapi;
		std::wstring app_name;
		std::wstring app_full_path;
		nvapi.GetCurrentAppNameAndFullPath(app_name, app_full_path);
		nvapi.CreateNvidiaProfileForCurrentApp(app_name.c_str(), app_full_path.c_str());
		nvapi.DisableFXAA();
	}
#endif

	//::Sleep(60000 * 2);

	CFrameWindowWnd* pFrame = new CFrameWindowWnd();
	if( pFrame == NULL ) return 0;
#if 1
	// create 非透明窗口
	pFrame->Create(NULL, TEXT("skia_demo : "), UI_WNDSTYLE_FRAME, 0);
#else
	// create 透明窗口
	pFrame->Create(NULL, TEXT("skia_demo : "), UI_WNDSTYLE_FRAME, WS_EX_LAYERED);
#endif


	CWindowWnd::MessageLoop();

#if USE_NVAPI
	//{
	//	nv_api nvapi;
	//	std::wstring app_name;
	//	std::wstring app_full_path;
	//	nvapi.GetCurrentAppNameAndFullPath(app_name, app_full_path);
	//	nvapi.DestroyNvidiaProfileForCurrentApp(app_name.c_str());
	//}
#endif

	globalUnInit();

	return 0;
}

