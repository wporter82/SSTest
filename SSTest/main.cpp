#include <Windows.h>
#include <ScrnSave.h>
#include <CommCtrl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _UNICODE
#pragma comment(lib, "scrnsavw.lib")
#else
#pragma comment(lib, "scrnsav.lib")
#endif // UNICODE
#pragma comment(lib, "comctl32.lib")

#pragma region globals
HWND winhwnd;
HBITMAP backBMP;
HDC backDC;
int backBufferCX, backBufferCY;

int totalPoints = 0;

#define TIMER 1
#pragma endregion

void Draw()
{
	
	for (int i = 0; i < 1000; i++)
	{
	int shade = rand() % 256;
	int greenShade = rand() % 256;
	SetPixel(backDC, rand() % backBufferCX, rand() % backBufferCY, RGB(0, greenShade, shade));
	}

	SetBkMode(backDC, TRANSPARENT);
	SetTextColor(backDC, RGB(rand() % 256, rand() % 256, rand() % 256));
	TextOutA(backDC, rand() % backBufferCX, rand() % backBufferCY, "X", 1);
	totalPoints++;

	if (totalPoints > 1000)
	{
		HBRUSH oldBrush = (HBRUSH)SelectObject(backDC, GetStockObject(BLACK_BRUSH));
		Rectangle(backDC, 0, 0, backBufferCX, backBufferCY);
		SelectObject(backDC, oldBrush);

		totalPoints = 0;
	}

}

LRESULT WINAPI ScreenSaverProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC winhdc;
	
	switch (message)
	{
	case WM_CREATE:
		winhwnd = hwnd;

		backBufferCX = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		backBufferCY = GetSystemMetrics(SM_CYVIRTUALSCREEN);

		winhdc = GetDC(winhwnd);
		backBMP = (HBITMAP)CreateCompatibleBitmap(winhdc, backBufferCX, backBufferCY);

		backDC = CreateCompatibleDC(winhdc);
		ReleaseDC(winhwnd, winhdc);

		SelectObject(backDC, backBMP);
		
		SetTimer(winhwnd, TIMER, 24, NULL);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		Draw();
		PAINTSTRUCT ps;

		winhdc = BeginPaint(hwnd, &ps);
		BitBlt(winhdc, 0, 0, backBufferCX, backBufferCY, backDC, 0, 0, SRCCOPY);

		EndPaint(hwnd, &ps);
		break;
	case WM_TIMER:
	
		RECT r;
		GetClientRect(winhwnd, &r);
		InvalidateRect(hwnd, &r, false);

		break;
	default:
		return DefScreenSaverProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

BOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hOK;

	switch (message)
	{
	case WM_INITDIALOG:
		hOK = GetDlgItem(hDlg, IDOK);
		return true;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hDlg, LOWORD(wParam) == IDOK);
			return true;
		default:
			return false;
		}
	}

	return false;
}

BOOL WINAPI RegisterDialogClasses(HANDLE hInst)
{
	return true;
}