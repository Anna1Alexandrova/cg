#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#pragma warning(push)
#pragma warning(disable : 4458)
#include <gdiplus.h>
#pragma warning(pop)
#include <memory>
#include <stdexcept>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include "resource.h"

TCHAR const CLASS_NAME[] = _T("MainWndClass");
TCHAR const WINDOW_TITLE[] = _T("File dialog example");

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int MainLoop(HWND hMainWindow);
bool RegisterWndClass(HINSTANCE hInstance);
HWND CreateMainWindow(HINSTANCE hInstance);

HINSTANCE g_hInstance = NULL;

using namespace Gdiplus;
using namespace std;

auto_ptr<Bitmap> g_pBitmap;
int g_x = 0;
int g_y = 0;

class CGdiplusInitializer
{
public:
	CGdiplusInitializer()
	{
		Gdiplus::GdiplusStartupInput input;
		Gdiplus::GdiplusStartupOutput output;

		if (Gdiplus::GdiplusStartup(&m_token, &input, &output) != Gdiplus::Ok)
		{
			throw std::runtime_error("Failed to initialize GDI+");
		}
	}

	~CGdiplusInitializer()
	{
		Gdiplus::GdiplusShutdown(m_token);
	}
private:
	ULONG_PTR m_token;
};


int WINAPI WinMain(
				   HINSTANCE hInstance,
				   HINSTANCE /*hPrevInstance*/,
				   LPSTR /*lpCmdLine*/,
				   int nCmdShow)
{
	g_hInstance = hInstance;
	try
	{
		CGdiplusInitializer initializer;

		if (!RegisterWndClass(hInstance))
		{
			return 1;
		}

		HWND hMainWindow = CreateMainWindow(hInstance);
		if (hMainWindow == NULL)
		{
			return 1;
		}

		ShowWindow(hMainWindow, nCmdShow);
		UpdateWindow(hMainWindow);

		int result = MainLoop(hMainWindow);

		g_pBitmap.release();

		return result;
	}
	catch (std::runtime_error &)
	{
		return 2;
	}
}

HWND CreateMainWindow(HINSTANCE hInstance)
{
	HMENU hMainMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MAIN_MENU));

	HWND hMainWindow = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		hMainMenu,
		hInstance,
		NULL);

	return hMainWindow;
}

int MainLoop(HWND hMainWindow)
{

	HACCEL accel = LoadAccelerators(g_hInstance, MAKEINTRESOURCE(IDR_MAIN_MENU));

	MSG msg;
	BOOL res;
	while ((res = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (res == -1)
		{
		}
		else
		{
			if (!TranslateAccelerator(hMainWindow, accel, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	return msg.wParam;
}

void OnDestroy(HWND /*hWnd*/)
{
	PostQuitMessage(0);
}

void Relocation(HWND hwnd)
{
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	int imageWidth = g_pBitmap->GetWidth();
	int imageHeight = g_pBitmap->GetHeight();

	g_x = (clientRect.right - clientRect.left - imageWidth) / 2;
	g_y = (clientRect.bottom - clientRect.top - imageHeight) / 2;
}

void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC dc = BeginPaint(hwnd, &ps);

	if (g_pBitmap.get() != NULL)
	{
		Graphics g(dc);

		Relocation(hwnd);

		g.DrawImage(g_pBitmap.get(), g_x, g_y);
	}

	EndPaint(hwnd, &ps);
}

void InitFileNameStructure(HWND hwndOwner, OPENFILENAME * pOpenFileName, TCHAR * pFileName, DWORD maxFileName)
{
	ZeroMemory(pOpenFileName, sizeof(OPENFILENAME));
	
	pOpenFileName->lStructSize = sizeof(OPENFILENAME);
	pOpenFileName->hwndOwner = hwndOwner;
	pOpenFileName->hInstance = g_hInstance;
	pOpenFileName->nMaxFile = maxFileName;
	pOpenFileName->lpstrFile = pFileName;
	pOpenFileName->lpstrFilter = 
		_T("Images (BMP, PNG, JPG, TIFF)\0*.bmp;*.png;*.jpg;*.tif\0")
		_T("All files\0*.*\0")
		_T("\0");

}

void OnOpenFile(HWND hwnd, UINT /*codeNotify*/)
{
	OPENFILENAME ofn;
	TCHAR fileName[MAX_PATH + 1] = _T("");
	InitFileNameStructure(hwnd, &ofn, fileName, MAX_PATH);
	
	if (GetOpenFileName(&ofn))
	{
		Image img(ofn.lpstrFile);

		if (img.GetLastStatus() == Ok)
		{

			g_pBitmap = auto_ptr<Bitmap>(new Bitmap(img.GetWidth(), img.GetHeight(), PixelFormat32bppARGB));
			Relocation(hwnd);
			Graphics g(g_pBitmap.get());
			g.DrawImage(&img, g_x, g_y, 100, 100);

			InvalidateRect(hwnd, NULL, TRUE);
		}
	}
}

std::wstring GetFileExtension(std::wstring const& fileName)
{
	size_t dotPos = fileName.find_last_of(L'.');
	if (dotPos != std::wstring::npos)
	{
		return fileName.substr(dotPos + 1);
	}
	else
	{
		return std::wstring();
	}
}

std::wstring WStringToLower(std::wstring const& str)
{
	std::wstring result(str);

	std::transform(result.begin(), result.end(), result.begin(), [](wchar_t ch) { return static_cast<wchar_t>(std::tolower(ch)); });

	return result;
}

CLSID GetEncoderCLSID(std::wstring const& fileExtension)
{
	std::wstring extensionMask = L"*." + WStringToLower(fileExtension) + L";";

	UINT numEncoders;
	UINT encodersSize;
	GetImageEncodersSize(&numEncoders, &encodersSize);

	std::vector<BYTE> encodersBuffer(encodersSize);

	ImageCodecInfo* pInstalledCodecs = reinterpret_cast<ImageCodecInfo *>(&encodersBuffer[0]);
	GetImageEncoders(numEncoders, encodersSize, pInstalledCodecs);

	for (unsigned i = 0; i < numEncoders; ++i)
	{
		ImageCodecInfo & codec = pInstalledCodecs[i];

		std::wstring extensions = WStringToLower(codec.FilenameExtension) + L";";

		if (extensions.find(extensionMask) != wstring::npos)
		{
			return codec.Clsid;
		}
	}

	return CLSID_NULL;
}

void SaveBitmap(Bitmap & bitmap, std::wstring const& fileName, int quality = 0)
{	
	std::wstring fileExtension = GetFileExtension(fileName);

	CLSID codecId = GetEncoderCLSID(fileExtension);
  
	if (IsEqualCLSID(codecId, CLSID_NULL))
	{
		return;
	}

	EncoderParameters params;
	params.Count = 1;	// ó íàñ òîëüêî îäèí ïàðàìåòð (ñòåïåíü êîìïðåñèè 0-100)

	EncoderParameter & param0 = params.Parameter[0];

	LONG qualityParam = quality;

	param0.Guid = EncoderCompression;
	param0.NumberOfValues = 1;
	param0.Type = EncoderParameterValueTypeLong;
	param0.Value = &qualityParam;

	bitmap.Save(fileName.c_str(), &codecId, &params);
}

void OnSaveFile(HWND hwnd, UINT /*codeNotify*/)
{
	if (!g_pBitmap.get())
	{
		return;
	}

	OPENFILENAME ofn;
	TCHAR fileName[MAX_PATH + 1] = _T("");
	InitFileNameStructure(hwnd, &ofn, fileName, MAX_PATH);

	if (GetSaveFileName(&ofn))
	{
		SaveBitmap(*g_pBitmap, fileName, 75);
	}
}

void OnExit(HWND hwnd, UINT /*codeNotify*/)
{
	DestroyWindow(hwnd);
}

void OnCommand(HWND hwnd, int id, HWND /*hwndCtl*/, UINT codeNotify)
{
	switch (id)
	{
	case ID_FILE_OPEN_IMAGE:
		OnOpenFile(hwnd, codeNotify);
		break;
	case ID_FILE_SAVE_IMAGE_AS:
		OnSaveFile(hwnd, codeNotify);
		break;
	case ID_FILE_EXIT:
		OnExit(hwnd, codeNotify);
		break;
	}
}

void OnResize(HWND hwnd, UINT state, int cx, int cy)
{
	if (g_pBitmap.get() != NULL)
	{
		Relocation(hwnd);
		InvalidateRect(hwnd, NULL, TRUE);
	}
}

LRESULT CALLBACK WindowProc(
							HWND hwnd,
							UINT uMsg,
							WPARAM wParam,
							LPARAM lParam)
{
	switch (uMsg)
	{
		HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hwnd, WM_SIZE, OnResize);
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool RegisterWndClass(HINSTANCE hInstance)
{
	WNDCLASSEX wndClass =
	{
		sizeof(wndClass),	//UINT cbSize;
		CS_HREDRAW | CS_VREDRAW, //UINT style;
		&WindowProc,		//WNDPROC lpfnWndProc;
		0,					//int cbClsExtra;
		0,					//int cbWndExtra;
		hInstance,			//HINSTANCE hInstance;
		NULL,				//HICON hIcon;
		LoadCursor(NULL, IDC_ARROW), //HCURSOR hCursor;
		(HBRUSH)(COLOR_BTNFACE + 1), //HBRUSH hbrBackground;
		NULL,				//LPCTSTR lpszMenuName;
		CLASS_NAME,			//LPCTSTR lpszClassName;
		NULL,				//HICON hIconSm;
	};

	return RegisterClassEx(&wndClass) != FALSE;
}
