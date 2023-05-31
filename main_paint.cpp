#include <Windows.h>
#include <vector>

// Идентификаторы элементов меню
#define IDM_FILE_NEW 1
#define IDM_FILE_OPEN 2
#define IDM_FILE_SAVE 3
#define IDM_FILE_EXIT 4

#define IDM_COLOR_RED 1001
#define IDM_COLOR_GREEN 1002
#define IDM_COLOR_BLUE 1003
#define IDM_SIZE_SMALL 1011
#define IDM_SIZE_MEDIUM 1012
#define IDM_SIZE_LARGE 1013

using namespace std;

struct Point
{
    int x;
    int y;
};
// Глобальные переменные
COLORREF g_Color = RGB(0, 0, 0);
int g_Size = 5;
std::vector<Point> g_Points;

// Прототипы функций
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void CreateMenu(HWND hWnd);
void DrawPoint(HWND hWnd, int, int);
// Точка входа в программу
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Регистрация класса окна
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"MyWindowClass";
    RegisterClass(&wc);

    // Создание окна
    HWND hWnd = CreateWindow(L"MyWindowClass", L"Window Title", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, hInstance, NULL);

    // Создание меню
    CreateMenu(hWnd);

    // Отображение окна
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Цикл обработки сообщений
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// Обработчик сообщений окна
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        // Обработка сообщений меню
        switch (LOWORD(wParam))
        {
        case IDM_FILE_NEW:
            MessageBox(hWnd, L"New file created", L"File", MB_OK);
            break;
        case IDM_FILE_OPEN:
            MessageBox(hWnd, L"File opened", L"File", MB_OK);
            break;
        case IDM_FILE_SAVE:
            MessageBox(hWnd, L"File saved", L"File", MB_OK);
            break;
        case IDM_FILE_EXIT:
            PostQuitMessage(0);
            break;
        case IDM_COLOR_RED:
            g_Color = RGB(255, 0, 0);
            break;
        case IDM_COLOR_GREEN:
            g_Color = RGB(0, 255, 0);
            break;
        case IDM_COLOR_BLUE:
            g_Color = RGB(0, 0, 255);
            break;
        case IDM_SIZE_SMALL:
            g_Size = 1;
            break;
        case IDM_SIZE_MEDIUM:
            g_Size = 5;
            break;
        case IDM_SIZE_LARGE:
            g_Size = 10;
            break;
        }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // Рисование всех точек
        for (auto& point : g_Points)
        {
            HBRUSH hBrush = CreateSolidBrush(g_Color);
            SelectObject(hdc, hBrush);
            Ellipse(hdc, point.x - g_Size, point.y - g_Size, point.x + g_Size, point.y + g_Size);
            DeleteObject(hBrush);
        }

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_LBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);

        DrawPoint(hWnd, x, y);
    }
    break;

    case WM_RBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);

        // Удаление ближайшей точки
        int minDistance = INT_MAX;
        int minIndex = -1;
        for (int i = 0; i < g_Points.size(); i++)
        {
            int distance = (x - g_Points[i].x) * (x - g_Points[i].x) + (y - g_Points[i].y) * (y - g_Points[i].y);
            if (distance < minDistance)
            {
                minDistance = distance;
                minIndex = i;
            }
        }
        if (minIndex != -1)
        {
            g_Points.erase(g_Points.begin() + minIndex);
        }

        // Обновление окна
        InvalidateRect(hWnd, NULL, TRUE);
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

// Создание меню
void CreateMenu(HWND hWnd)
{
    HMENU hMenu = CreateMenu();
    HMENU hFileMenu = CreatePopupMenu();

    AppendMenu(hFileMenu, MF_STRING, IDM_FILE_NEW, L"&New");
    AppendMenu(hFileMenu, MF_STRING, IDM_FILE_OPEN, L"&Open");
    AppendMenu(hFileMenu, MF_STRING, IDM_FILE_SAVE, L"&Save");
    AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hFileMenu, MF_STRING, IDM_FILE_EXIT, L"E&xit");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"&File");

    SetMenu(hWnd, hMenu);
}

void DrawPoint(HWND hWnd, int x, int y)
{
    // Создание новой точки
    Point point = { x, y };
    g_Points.push_back(point);

    // Обновление окна
    InvalidateRect(hWnd, NULL, TRUE);
}


