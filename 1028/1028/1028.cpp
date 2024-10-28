// 1028.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "1028.h"
#include "new.h"

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

HANDLE g_handle;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    /// OS에게 해당 이름에 맞는 객체가 존재한다면 객체의 핸들 반환을 요구
    HANDLE h = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"asdf");

    if (NULL == h)
    {
        /// asdf라는 이름의 mutex 객체는 존재하지 않는다.
        //MessageBox(NULL, L"객체 없어", L"피곤하니", MB_OK);
        g_handle = CreateMutex(NULL, TRUE, L"asdf");
    }
    else
    {
        /// asdf라는 이름의 mutex 객체가 존재한다.
        //MessageBox(NULL, L"객체 이써", L"왜이러지", MB_OK);
        return 0;
    }


    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY1028, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1028));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1028));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1028);
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

/// * 공유 자원 선언
int g_x;
HWND g_hWnd;

//////////////////////////////////////////////////// Mutex 해결 방법
/// 1. 자료구조 선언
HANDLE g_mux;

DWORD WINAPI thread_mux(LPVOID param)       /// lParam을 인수로 받는다.
{
    LPARAM lParam = (LPARAM)param;
    int x, y, i;
    x = LOWORD(lParam);
    y = HIWORD(lParam);
    HDC hdc = GetDC(g_hWnd);

    for (i = 0; i < y; i++)
    {
        /// Mutex 사용
        WaitForSingleObject(g_mux, INFINITE);
        ////////////////////////////////// 임계 영역 시작
        g_x = x;
        MoveToEx(hdc, g_x, 0, NULL);
        LineTo(hdc, g_x, i);
        ////////////////////////////////// 임계 영역 종료
        /// Mutex 해제
        ReleaseMutex(g_mux);
        Sleep(30);
    }


    ReleaseDC(g_hWnd, hdc);

    return 0;
}



//////////////////////////////////////////////////// Semaphore 해결 방법
/// 1. 자료구조 선언
HANDLE g_sem;

DWORD WINAPI thread_sem(LPVOID param)       /// lParam을 인수로 받는다.
{
    LPARAM lParam = (LPARAM)param;
    int x, y, i;
    x = LOWORD(lParam);
    y = HIWORD(lParam);
    HDC hdc = GetDC(g_hWnd);

    
    for (i = 0; i < y; i++)
    {
        /// Semaphore 사용
        WaitForSingleObject(g_sem, INFINITE);
        ////////////////////////////////// 임계 영역 시작
        g_x = x;
        MoveToEx(hdc, g_x, 0, NULL);
        LineTo(hdc, g_x, i);
        ////////////////////////////////// 임계 영역 종료
        /// Semaphore 해제
        ReleaseSemaphore(g_sem, 1, NULL);
        Sleep(30);
    }
    

    ReleaseDC(g_hWnd, hdc);

    return 0;
}





//////////////////////////////////////////////////// Critical Section 해결 방법
/// 1. 자료구조 선언
CRITICAL_SECTION g_cs;

DWORD WINAPI thread_cs(LPVOID param)       /// lParam을 인수로 받는다.
{
    LPARAM lParam = (LPARAM)param;
    int x, y, i;
    x = LOWORD(lParam);
    y = HIWORD(lParam);
    HDC hdc = GetDC(g_hWnd);

    for (i = 0; i < y; i++)
    {
        /// Critical Section 사용
        EnterCriticalSection(&g_cs);
        ////////////////////////////////// 임계 영역 시작
        g_x = x;
        MoveToEx(hdc, g_x, 0, NULL);
        LineTo(hdc, g_x, i);
        ////////////////////////////////// 임계 영역 종료
        /// Critical Section 해제
        LeaveCriticalSection(&g_cs);
        Sleep(30);
    }

    ReleaseDC(g_hWnd, hdc);

    return 0;
}





//////////////////////////////////////////////////// SW적인 해결 방법
BOOL g_use = FALSE;

/// Thread 생성
DWORD WINAPI threadfunction(LPVOID param)       /// lParam을 인수로 받는다.
{
    LPARAM lParam = (LPARAM)param;
    int x, y, i;
    x = LOWORD(lParam);
    y = HIWORD(lParam);
    HDC hdc = GetDC(g_hWnd);

    for (i = 0; i < y; i++)
    {
        /// g_use가 TRUE라면 아무것도 하지 않고 대기한다.
        while (g_use) {};
        // cs
        g_use = TRUE;
        ////////////////////////////////// 임계 영역 시작
        g_x = x;
        MoveToEx(hdc, g_x, 0, NULL);
        LineTo(hdc, g_x, i);
        ////////////////////////////////// 임계 영역 종료
        g_use = FALSE;
        Sleep(30);
    }

    ReleaseDC(g_hWnd, hdc);

    return 0;
}


class asdf
{
public:
    asdf();
    void aksfduhaisdfuy() { }
};

asdf *g_asdf;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        g_asdf = new asdf();
        g_asdf->aksfduhaisdfuy();

        /// Critical Section 객체 생성
        InitializeCriticalSection(&g_cs);

        /// Semaphore 객체 생성
        /// 보안 속성 : NULL이면 프로세스 권한을 그대로 상속
        /// 초기 카운트 : 처음에 사용할 수 있는 자원의 개수
        /// 최대 카운트 : 전체적으로 사용할 수 있는 모든 자원의 개수
        /// 이름 : 지역(자기 자신, 프로세스) 내에 이름을 등록하여 중복 생성 등을 방지
        g_sem = CreateSemaphore(NULL, 1, 1, NULL);

        /// Mutex 객체 생성
        /// 보안 속성
        /// 초기 소유자 존재 유무
        /// 이름 : 전역(OS) 범위에 이름을 등록하여 중복 생성 등을 방지
        g_mux = CreateMutex(NULL, FALSE, NULL);
    }
        break;
    case WM_LBUTTONDOWN:
    {
        g_hWnd = hWnd;
        /// SW 적인 해결방법 스레드 생성 코드
        //CreateThread(NULL, 0, threadfunction, (LPVOID)lParam, 0, NULL);
        /// Critical Section 스레드 생성 코드
        //CreateThread(NULL, 0, thread_cs, (LPVOID)lParam, 0, NULL);
        /// Semaphore 스레드 생성 코드
        //CreateThread(NULL, 0, thread_sem, (LPVOID)lParam, 0, NULL);
        /// Mutex 스레드 생성 코드
        CreateThread(NULL, 0, thread_mux, (LPVOID)lParam, 0, NULL);
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
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        /// Critical Section 객체 삭제 요청 ==> OS에게
        DeleteCriticalSection(&g_cs);
        /// Semaphore 객체 삭제 요청
        CloseHandle(g_sem);
        /// Mutex 객체 삭제 요청
        CloseHandle(g_mux);
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
