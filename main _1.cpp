#include <Windows.h>

// Идентификаторы элементов меню
#define IDM_FILE_NEW 1
#define IDM_FILE_OPEN 2
#define IDM_FILE_SAVE 3
#define IDM_FILE_EXIT 4

// Прототипы функций
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void CreateMenu(HWND hWnd);

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
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        HBRUSH red = CreateSolidBrush(RGB(255, 0, 0)), blue = CreateSolidBrush(RGB(0, 0, 255)),
            gray_120 = CreateSolidBrush(RGB(120, 120, 120)), gray_90 = CreateSolidBrush(RGB(90, 90, 90)),
            gray_255 = CreateSolidBrush(RGB(255, 255, 255)), gray_0 = CreateSolidBrush(RGB(0, 0, 0));

        SelectObject(hdc, gray_90);
        Rectangle(hdc, 0, 0, 70, 2000);
        SelectObject(hdc, gray_120);
        Rectangle(hdc, 0, 0, 5000, 70);

        break;
    }
    case WM_LBUTTONDOWN:
    {
        POINT cursorPos;
        GetCursorPos(&cursorPos);
        ScreenToClient(hWnd, &cursorPos);
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        HBRUSH gray_0 = CreateSolidBrush(RGB(0, 0, 0));

        SelectObject(hdc, gray_0);
        RECT rect = { cursorPos.x - 10, cursorPos.y - 10, cursorPos.x + 10, cursorPos.y + 10 };
        Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

        break;
    }

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