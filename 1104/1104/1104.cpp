﻿// 1104.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "1104.h"

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
    LoadStringW(hInstance, IDC_MY1104, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1104));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1104));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1104);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
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

/// 실행 전에 프로세스의 상태를 OS에게 전달하는 용도
STARTUPINFO g_si;
/// 실행 후에 프로세스의 제어 정보를 OS로부터 획득하는 용도
PROCESS_INFORMATION g_pi;

HWND g_hWnd;

DWORD WINAPI pig(LPVOID param)
{
    LPARAM lParam = (LPARAM)param;
    int x = LOWORD(lParam);
    int y = HIWORD(lParam);
    int i = 0;
    //TCHAR buf[128] = { 0, };
    wchar_t buf[128] = { 0, };

    HDC hdc = GetDC(g_hWnd);

    for (; i < y; i++)
    {
        MoveToEx(hdc, x, 0, NULL);
        LineTo(hdc, x, i);
        /// 배열에 printf 형식으로 문자열을 입력
        wsprintf(buf, L"x : %d // y: %d", x, i);
        /// 화면에 직접 출력
        TextOut(hdc, x, i, buf, lstrlenW(buf));
        /// 배열 내용을 초기화
        memset(buf, 0x00, 128);
        Sleep(30);
    }

    ReleaseDC(g_hWnd, hdc);
    /// 스레드가 종료될 때, OS에게 작업이 종료 되었으니 자원을 정리해달라라는 요청
    ExitThread(0);
    return 0;
}

/// 프로세스와 스레드의 정보 출력을 위한 버퍼
wchar_t g_buf[128];
/// 자식 프로세스와 스레드의 정보 출력을 위한 버퍼
wchar_t g_child[128];


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        wsprintf(g_buf, L"My Process ID: %d\tThread ID: %d",
            GetCurrentProcessId(), GetCurrentThreadId());
    }
        break;

    case WM_RBUTTONDOWN:
    {
        /// 자식 프로세스 종료 제어
        TerminateProcess(g_pi.hProcess, 0);
        /// 자식 프로세스 베이스 스레드의 일시 중단
        //SuspendThread(g_pi.hThread);
    }
        break;

    case WM_KEYDOWN:
    {
        /// 자식 프로세스 베이스 스레드의 작업 재개
        //ResumeThread(g_pi.hThread);
        /// 자식 프로세스 베이스 스레드의 강제 종료
        TerminateThread(g_pi.hThread, 0);
    }
        break;

    case WM_LBUTTONDOWN:
    {
        //wchar_t processName[] = L"explorer.exe d:\\";
        //wchar_t processName[] = L"notepad.exe d:\\eula.txt";
        wchar_t processName[] = L"1104.exe";
        CreateProcess(NULL, processName, NULL, NULL, FALSE, 0, NULL, NULL, &g_si, &g_pi);
        wsprintf(g_child, L"Child PID: %d \t TID : %d", g_pi.dwProcessId, g_pi.dwThreadId);
        //g_hWnd = hWnd;
        //CreateThread(NULL, 0, pig, (LPVOID)lParam, 0, NULL);
    }
        break;
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
            TextOut(hdc, 10, 10, g_buf, lstrlenW(g_buf));
            TextOut(hdc, 10, 30, g_child, lstrlenW(g_child));
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
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
