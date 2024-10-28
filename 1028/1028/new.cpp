#include "new.h"

HANDLE g_mux;

DWORD WINAPI thread_mux(LPVOID param)       /// lParam�� �μ��� �޴´�.
{
    LPARAM lParam = (LPARAM)param;
    int x, y, i;
    x = LOWORD(lParam);
    y = HIWORD(lParam);
    HDC hdc = GetDC(g_hWnd);

    for (i = 0; i < y; i++)
    {
        /// Mutex ���
        WaitForSingleObject(g_mux, INFINITE);
        ////////////////////////////////// �Ӱ� ���� ����
        g_x = x;
        MoveToEx(hdc, g_x, 0, NULL);
        LineTo(hdc, g_x, i);
        ////////////////////////////////// �Ӱ� ���� ����
        /// Mutex ����
        ReleaseMutex(g_mux);
        Sleep(30);
    }


    ReleaseDC(g_hWnd, hdc);

    return 0;
}
