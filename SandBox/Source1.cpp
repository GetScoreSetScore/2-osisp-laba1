/*#include <windows.h>
#include <tchar.h>
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
HFONT hfnt;
const TCHAR* szCaption;
static void Paint(HWND hWnd, LPPAINTSTRUCT lpPS, HBITMAP hBmp, int x, int y)
{
    RECT rc;
    HDC BufferDC, BmpContainerDC;
    HBITMAP BufferBackgroundBmp, PicBmp, OldPic;
    HBRUSH brush;
    GetClientRect(hWnd, &rc);
    BufferDC = CreateCompatibleDC(lpPS->hdc);
    BufferBackgroundBmp = CreateCompatibleBitmap(lpPS->hdc, rc.right - rc.left, rc.bottom - rc.top);
    SelectObject(BufferDC, BufferBackgroundBmp);
    FillRect(BufferDC, &rc, CreateSolidBrush(RGB(255, 255, 255)));
    BmpContainerDC = CreateCompatibleDC(BufferDC);
    OldPic = (HBITMAP)SelectObject(BmpContainerDC, hBmp);
    BitBlt(BufferDC, x, y, 150, 92, BmpContainerDC, 0, 0, SRCCOPY);
    BitBlt(lpPS->hdc,rc.left, rc.top,rc.right - rc.left, rc.bottom - rc.top,BufferDC,0, 0,SRCCOPY);
	(HBITMAP)SelectObject(BmpContainerDC, OldPic);
	DeleteObject(BufferBackgroundBmp);
	DeleteObject(OldPic);
    DeleteDC(BufferDC);
	DeleteDC(BmpContainerDC);
}
HBITMAP hBitmap = NULL;
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
    static RECT rect;
	switch (msg) {
	case WM_SETTEXT:
		break;

	case WM_SETFONT:
		break;

	case WM_ERASEBKGND:
		return (LRESULT)1; // Say we handled it.

	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		Paint(hWnd, &ps,hBitmap,rect.left,rect.top);
		EndPaint(hWnd, &ps);
		break;
    case WM_CREATE:
        hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), L"c:\\Users\\ANev\\Pictures\\Picture1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        SetRect(&rect, 100, 100, 250, 192);
        break;
    case WM_MOUSEMOVE:
        if (wParam == MK_LBUTTON) {
            RECT tmp;
            GetClientRect(hWnd, &tmp);
            InvalidateRect(hWnd, &tmp, FALSE);
            rect.left = LOWORD(lParam) - 75;
            rect.right = rect.left + 150;
            rect.top = HIWORD(lParam) - 46;
            rect.bottom = rect.top + 92;
            //InvalidateRect(hWnd, &rect, FALSE);
        }
        break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	}
	return NULL;
}*/