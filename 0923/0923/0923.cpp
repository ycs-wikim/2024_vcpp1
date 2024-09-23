// 0923.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "0923.h"
#include <stdlib.h>
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
    LoadStringW(hInstance, IDC_MY0923, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0923));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0923));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0923);
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

/// 나와 상대의 객체를 전역 변수로 선언
RECT g_me, g_you, g_item;       // 전역 변수는 별도 초기화가 없어도 0으로 초기화
/// 상대 시간을 조절하는 변수
int g_youtime = 500;
/// 상대를 숨기기 위한 전용 변수
int g_view = 1;
/// 게임 시간 체크
int g_gametime = 10;
RECT g_time;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_TIMER:
        /// WM_TIMER에서는 ID 값을 wParam에 전달되어 가져온다.
        if (1 == wParam)
        {
            /// 상대 객체의 좌표만 이동하는 코드
            if (g_me.left > g_you.left)
            {
                g_you.left += 10;
                g_you.right += 10;
            }
            else
            {
                g_you.left -= 10;
                g_you.right -= 10;
            }

            if (g_me.top > g_you.top)
            {
                g_you.top += 10;
                g_you.bottom += 10;
            }
            else
            {
                g_you.top -= 10;
                g_you.bottom -= 10;
            }
            RECT rect;

            /// 게임 종료 조건
            if (IntersectRect(&rect, &g_me, &g_you))
            {
                /// 타이머를 세운다
                KillTimer(hWnd, 1);
                KillTimer(hWnd, 2);
                MessageBox(hWnd, L"Game Over", L"안녕", MB_OK);
            }
            /// 상대가 나의 위치로 좌표를 변경만!한 상태
            /// OS에게 화면을 다시 그려달라고 요청!
            InvalidateRect(hWnd, NULL, TRUE);
        }
        else if (2 == wParam)
        {
            g_youtime -= 100;
            if (g_youtime <= 100)
                g_youtime = 100;

            KillTimer(hWnd, 1);
            SetTimer(hWnd, 1, g_youtime, NULL);
        }
        else
        {
            g_gametime -= 1;
            g_time.right = g_time.right - g_gametime * 10;
        }
        break;

    /// 초기값 설정을 위한 윈도우 메시지
    /// 프로그램이 실행되면 OS가 반드시 한번 호출해주는 윈도우 메시지
    case WM_CREATE:
        /// 랜덤 시드 설정
        srand(time(NULL));

        /// OS에게 지정 시간이 되면 호출을 요청
        /// 상대가 이동하기 위한 시간 타이머 설정
        SetTimer(hWnd, 1, g_youtime, NULL);
        /// 상대의 속도를 변경하기 위한 타이머 설정
        SetTimer(hWnd, 2, 1000, NULL);
        /// 게임 시간 확인을 위한 타이머 설정
        SetTimer(hWnd, 3, 1000, NULL);

        /// 내 객체의 초기 위치 설정
        g_me.left = 10;         // 나의 좌상단 x
        g_me.top = 10;          // 나의 좌상단 y
        g_me.right = 100;       // 나의 우하단 x
        g_me.bottom = 100;      // 나의 우하단 y

        /// 상대 객체의 초기 위치 설정
        g_you.left = 500;
        g_you.top = 400;
        g_you.right = 590;
        g_you.bottom = 490;

        // 아이템 초기 위치 설정
        g_item.left = 300;
        g_item.top = 300;
        g_item.right = 350;
        g_item.bottom = 350;

        // 시간 초기 위치
        g_time.left = 10;
        g_time.top = 10;
        g_time.right = 500;
        g_time.bottom = 90;
        break;

    /// 나의 객체를 이동시키자!
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_LEFT:
            g_me.left -= 10;
            g_me.right -= 10;
            break;
        case VK_RIGHT:
            g_me.left += 10;
            g_me.right += 10;
            break;
        case VK_UP:
            g_me.top -= 10;
            g_me.bottom -= 10;
            break;
        case VK_DOWN:
            g_me.top += 10;
            g_me.bottom += 10;
            break;
        }
        /// 나의 위치 좌표 변경 완료.
        if (g_me.left >= 500)
        {
            g_me.left = 500;
            g_me.right = 590;
        }

        /// 나의 위치 좌표 설정을 변경
        RECT item;
        if (IntersectRect(&item, &g_me, &g_item))
        {
            g_item.left = rand() % 500;
            g_item.top = rand() % 500;
            g_item.right = g_item.left + 50;
            g_item.bottom = g_item.top + 50;
            //g_view = 0;
            /*
            KillTimer(hWnd, 2);
            g_youtime += 200;
            if (g_youtime >= 1000)
                g_youtime = 1000;
            SetTimer(hWnd, 2, g_youtime, NULL);
            */
        }

        /// 사용자 키보드 입력에 따른 객체 좌표 위치 변경 완료.
        /// 화면 무효화가 발생! ==> OS에게 요청
        InvalidateRect(hWnd, NULL, TRUE);
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
            /// 나의 객체를 화면에 그린다.
            Rectangle(hdc, g_me.left, g_me.top, g_me.right, g_me.bottom);

            /// 상대 객체를 화면에 그린다.
            Rectangle(hdc, g_you.left, g_you.top, g_you.right, g_you.bottom);

            /// 게임 시간 표시
            Rectangle(hdc, g_time.left, g_time.top, g_time.right, g_time.bottom);
            /// 아이템 객체를 화면에 그린다.
            if( 1 == g_view )
                Ellipse(hdc, g_item.left, g_item.top, g_item.right, g_item.bottom);
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
