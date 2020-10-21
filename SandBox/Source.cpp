#include <windows.h>
#include <tchar.h>
#include <wingdi.h>
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex; HWND hWnd; MSG msg;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _T("HelloWorldClass");
	wcex.hIconSm = wcex.hIcon;
	RegisterClassEx(&wcex);
	hWnd = CreateWindow(_T("HelloWorldClass"), _T("Hello, World!"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
HBITMAP hBitmap = NULL;
HBITMAP Mask = NULL;
#define TIMER1 1001
HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent)
{
	HDC hdcMem, hdcMem2;
	HBITMAP hbmMask;
	BITMAP bm;

	//делаем одноцветную маску
	GetObject(hbmColour, sizeof(BITMAP), &bm);
	hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

	hdcMem = CreateCompatibleDC(0);
	hdcMem2 = CreateCompatibleDC(0);

	SelectObject(hdcMem, hbmColour);
	SelectObject(hdcMem2, hbmMask);

	//фоновым делаем выбранный цвет
	SetBkColor(hdcMem, crTransparent);
	BitBlt(hdcMem2, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY); //фоновый делаем белый, остальное - чёрное


	BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCINVERT);//фоновый делаем чёрный
	DeleteDC(hdcMem);
	DeleteDC(hdcMem2);

	return hbmMask;
}
static void Paint(HWND hWnd, LPPAINTSTRUCT lpPS, HBITMAP hBmp,HBITMAP Mask, int x, int y)
{
	RECT rc;
	HDC BufferDC, BmpContainerDC;
	HBITMAP BufferBackgroundBmp, OldPic;
	//создание битмапа и перекраска фона
	GetClientRect(hWnd, &rc);
	BufferDC = CreateCompatibleDC(lpPS->hdc);
	BufferBackgroundBmp = CreateCompatibleBitmap(lpPS->hdc, rc.right - rc.left, rc.bottom - rc.top);
	SelectObject(BufferDC, BufferBackgroundBmp);
	FillRect(BufferDC, &rc, CreateSolidBrush(RGB(0, 255, 0)));
	//контекст для изменения изображения
	BmpContainerDC = CreateCompatibleDC(BufferDC);
	//отображаем маску
	OldPic = (HBITMAP)SelectObject(BmpContainerDC, Mask);
	BitBlt(BufferDC, x, y, 200, 200, BmpContainerDC, 0, 0, SRCAND);
	//поверх маски исходное изображение
	SelectObject(BmpContainerDC, hBmp);
	BitBlt(BufferDC, x, y, 200, 200, BmpContainerDC, 0, 0, SRCPAINT);
	//копирование изображения на фон
	BitBlt(lpPS->hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, BufferDC, 0, 0, SRCCOPY);
	(HBITMAP)SelectObject(BmpContainerDC, OldPic);
	DeleteObject(BufferBackgroundBmp);
	DeleteObject(OldPic);
	DeleteDC(BufferDC);
	DeleteDC(BmpContainerDC);
}
BOOL IsTimed = 0;
int HorSpeed =10, VertSpeed=10;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	PAINTSTRUCT ps;
	static RECT rect;
	RECT tmp;
	switch (message)
	{
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		if (rect.top < 0) {
			rect.top = 0;
			rect.bottom = 200;
			VertSpeed = -VertSpeed;
		}
		if (rect.bottom > ps.rcPaint.bottom) {
			rect.top = ps.rcPaint.bottom - 200;
			rect.bottom = ps.rcPaint.bottom;
			VertSpeed = -VertSpeed;
		}
		if (rect.left < 0) {
			rect.left = 0;
			rect.right = rect.left + 200;
			HorSpeed = -HorSpeed;
		}
		if (rect.right > ps.rcPaint.right) {
			rect.right = ps.rcPaint.right;
			rect.left = rect.right - 200;
			HorSpeed = -HorSpeed;
		}
		Paint(hWnd, &ps,hBitmap,Mask,rect.left,rect.top);
		EndPaint(hWnd, &ps);
		break;
	case WM_MOUSEHWHEEL:
		GetClientRect(hWnd, &tmp);
		InvalidateRect(hWnd, &tmp, FALSE);
		if (LOWORD(wParam) == MK_SHIFT) {
			rect.left += (short)HIWORD(wParam) / 12;
			rect.right += (short)HIWORD(wParam) / 12;
		}
		else {
			rect.top += (short)HIWORD(wParam) / 12;
			rect.bottom += (short)HIWORD(wParam) / 12;
		}
		break;
	case WM_CHAR:
		GetClientRect(hWnd, &tmp);
		InvalidateRect(hWnd, &tmp, FALSE);
		switch (wParam)
		{
		case 'w':
			rect.top -=  10;
			rect.bottom -=  10;
			break;
		case 'a':
			rect.left -= 10;
			rect.right -= 10;
			break;
		case 's':
			rect.top += 10;
			rect.bottom += 10;
			break;
		case 'd':
			rect.left += 10;
			rect.right +=10;
			break;
		case 't':
			if (!IsTimed) {
				IsTimed = TRUE;
				SetTimer(hWnd,TIMER1,30,(TIMERPROC)NULL);
			}
			else {
				IsTimed = FALSE;
				KillTimer(hWnd, TIMER1);
			}
		}
		break;
	case WM_TIMER:
		if (wParam == TIMER1)
		{
			GetClientRect(hWnd, &tmp);
			InvalidateRect(hWnd, &tmp, FALSE);
			rect.left += HorSpeed;
			rect.right += HorSpeed;
			rect.top += VertSpeed;
			rect.bottom += VertSpeed;
		}
		break;
	case WM_MOUSEMOVE:
		if (wParam == MK_LBUTTON) {
			GetClientRect(hWnd, &tmp);
			InvalidateRect(hWnd, &tmp, FALSE);
			rect.left = LOWORD(lParam) - 100;
			rect.right = rect.left + 200;
			rect.top = HIWORD(lParam) - 100;
			rect.bottom = rect.top + 200;
		}
		break;
	case WM_SIZE:
		GetClientRect(hWnd, &tmp);
		InvalidateRect(hWnd, &tmp, FALSE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), L"Picture2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		Mask = CreateBitmapMask(hBitmap, RGB(0, 0, 0));
		SetRect(&rect, 100, 100, 250, 250);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}