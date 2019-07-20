// WinPuzzleApp.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "WinPuzzleApp.h"
#include <time.h>

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void MoveBlock(WPARAM wParam, int* s_nCursorX, int* s_nCursorY, HWND(*s_hwnd)[5], int(*s_nXPos)[5], int(*s_nYPos)[5]);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINPUZZLEAPP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINPUZZLEAPP));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINPUZZLEAPP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_CROSS);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BACKGROUND);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINPUZZLEAPP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, TEXT("퍼즐게임"), WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND s_hwnd[5][5];
	static int s_nXPos[5][5];
	static int s_nYPos[5][5];
	static int s_nCursorX;
	static int s_nCursorY;

	int count = 0;
    switch (message)
    {
	case WM_CREATE:
		{
			for (int y = 0; y < 5; y++) 
			{
				for (int x = 0; x < 5; x++) 
				{
					s_nCursorX = 4;
					s_nCursorY = 4;

					s_nXPos[y][x] = (110 * x) + 10;
					s_nYPos[y][x] = (110 * y) + 10;

					if (x == 4 && y == 4)
					{
						s_hwnd[y][x] = NULL;
						break;
					}

					s_hwnd[y][x] = CreateWindow(TEXT("STATIC"), NULL,
						WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
						s_nXPos[y][x], s_nYPos[y][x], 100, 100, hWnd, (HMENU)-1,
						(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
					count++;
					
					TCHAR text[10];
					wsprintf(text, _T("%d"), count);
					SetWindowText(s_hwnd[y][x], text);
				}	
			}

			srand((unsigned)time(NULL));

			int key = 0;
			for (int i = 0; i < 100; ++i) {
				key = VK_LEFT + (rand() % 4);
				MoveBlock(key, &s_nCursorX, &s_nCursorY, s_hwnd, s_nXPos, s_nYPos);
			}
		}
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				if (s_hwnd[i] != NULL) {
					DestroyWindow(s_hwnd[i][j]);
				}
			}	
		}
        PostQuitMessage(0);
        break;
	case WM_KEYUP:
		{
			MoveBlock(wParam, &s_nCursorX, &s_nCursorY, s_hwnd, s_nXPos, s_nYPos);
		}
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void MoveBlock(WPARAM wParam, int *s_nCursorX, int *s_nCursorY, HWND (*s_hwnd)[5], int (*s_nXPos)[5], int (*s_nYPos)[5]) 
{
	int XOffset = (wParam == VK_RIGHT) ? -1 : (wParam == VK_LEFT) ? 1 : 0;
	int YOffset = (wParam == VK_DOWN) ? -1 : (wParam == VK_UP) ? 1 : 0;

	if (*s_nCursorY + YOffset < 5 && *s_nCursorY + YOffset >= 0
		&& *s_nCursorX + XOffset < 5 && *s_nCursorX + XOffset >= 0)
	{
		HWND hWnd = s_hwnd[*s_nCursorY + YOffset][*s_nCursorX + XOffset];
		MoveWindow(hWnd, s_nXPos[*s_nCursorY][*s_nCursorX], s_nYPos[*s_nCursorY][*s_nCursorX], 100, 100, TRUE);

		s_hwnd[*s_nCursorY][*s_nCursorX] = hWnd;
		s_hwnd[*s_nCursorY + YOffset][*s_nCursorX + XOffset] = NULL;

		*s_nCursorX += XOffset;
		*s_nCursorY += YOffset;
	}
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
