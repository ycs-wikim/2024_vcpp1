#include "new.h"

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
