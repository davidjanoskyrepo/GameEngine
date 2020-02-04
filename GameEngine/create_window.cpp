// create_window.cpp : defines the entry point for the application and the creation of the window
//

/* Disclaimer - a lot of the code for setting up a window in OpenGL context was based on nehe tutorial at
   http://nehe.gamedev.net/tutorial/creating_an_opengl_window_%28win32%29/13001/
*/

// These are the libraries we need for a winAPI/openGL window
#include <windows.h>								// Header File For Windows
#include <gl\gl.h>									// Header File For The OpenGL32 Library
#include <gl\glu.h>									// Header File For The GLu32 Library

// Libraries we define that include functions and defs
#include "framework.h"
#include "resource.h"

// Resources and variables used by the program
#define			MAX_LOADSTRING			100

/*
	This is where we declare globals we will use in our openGL programs
*/
HGLRC           hRC = NULL;							// Permanent Rendering Context for our OpenGL program to link to
HDC             hDC = NULL;                         // Private GDI Device Context - allows us to draw to the window
HWND            hWnd = NULL;                        // Holds Our Window Handle
HINSTANCE       hInst;								// Holds The Instance Of The Application

static  WCHAR	szTitle[MAX_LOADSTRING]         = _T("Don't Blink");    // The title bar text
static  WCHAR	szWindowClass[MAX_LOADSTRING]   = _T("DesktopApp");     // the main window class name
HWND			hButton;							// Example button format say, settings

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);		// Declare WndProc() so that CreateGLWindow() can make reference to WndProc()
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

bool    keys[256];									// Array Used For The Keyboard Routine
bool    active = TRUE;                              // Window Active Flag Set To TRUE By Default
bool    fullscreen = TRUE;                          // Fullscreen Flag Set To Fullscreen Mode By Default


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Register the class/window
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MENU));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize         = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MENU);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

   // The parameters to CreateWindow explained:
   // szWindowClass: the name of the application
   // szTitle: the text that appears in the title bar
   // WS_OVERLAPPEDWINDOW: the type of window to create
   // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
   // 500, 100: initial size (width, length)
   // NULL: the parent of this window
   // NULL: this application does not have a menu bar
   // hInstance: the first parameter from WinMain
   // NULL: not used in this application
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
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    HDC hDC;                    // device context (DC) for window

    //static POINT midPoint;

    // Note rects go left, top, right, bottom
    static RECT rcLeft  { 600, 120, 700, 220 };         // Left rectangle to draw when key pressed
    static RECT rcRight { 820, 120, 920, 220 };         // Right rectangle to draw when key pressed
    static RECT rcUp    { 710, 010, 810, 110 };         // Up rectangle to draw when key pressed
    static RECT rcDown  { 710, 230, 810, 330 };         // Down rectangle to draw when key pressed

    static HBRUSH hbrBkgnd;  // handle of background-color brush  
    static COLORREF crBkgnd; // color of client-area background 

    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case IDC_MYBUTTON1:
            //DestroyWindow(hWnd);
            CheckDlgButton(hWnd, IDC_MYBUTTON1, !IsDlgButtonChecked(hWnd, IDC_MYBUTTON1));
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_CREATE:
    {
        hButton = CreateWindowEx(WS_EX_WINDOWEDGE,
            TEXT("BUTTON"), TEXT("TRIGGERBOT"),
            WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
            40, 40,
            125, 20,
            hWnd,
            (HMENU)IDC_MYBUTTON1,
            hInst, NULL);
        CheckDlgButton(hWnd, IDC_MYBUTTON1, BST_UNCHECKED);

        // Create a brush of the same color as the background  
        // of the client area. The brush is used later to erase  
        // the old rect.
        hDC = GetDC(hWnd);
        crBkgnd = GetBkColor(hDC);
        hbrBkgnd = CreateSolidBrush(crBkgnd);
        ReleaseDC(hWnd, hDC);
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        hDC = BeginPaint(hWnd, &ps);
        TCHAR test[] = _T("Hello this is the test message to paint!");
        /*  Set txt color to blue*/
        SetTextColor(hDC, COLORREF(0x00FF0000));
        /*  Display text in middle of window*/
        TextOut(hDC, 150, 150, test, _tcslen(test));
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_LEFT:
        {
            // Process the LEFT ARROW key. 
            // All painting occurs here, between BeginPaint and EndPaint.
            hDC = GetDC(hWnd);

            HBRUSH brush = CreateSolidBrush(RGB(151, 0, 0));

            FillRect(hDC, &rcLeft, brush);

            // Free resources
            ReleaseDC(hWnd, hDC);
            DeleteObject(brush);
        }
        break;

        case VK_RIGHT:
        {
            // Process the LEFT ARROW key. 
            // All painting occurs here, between BeginPaint and EndPaint.
            hDC = GetDC(hWnd);

            HBRUSH brush = CreateSolidBrush(RGB(151, 0, 0));

            FillRect(hDC, &rcRight, brush);

            // Free resources
            ReleaseDC(hWnd, hDC);
            DeleteObject(brush);
        }
        break;

        case VK_UP:
        {
            // Process the UP ARROW key. 
            // All painting occurs here, between BeginPaint and EndPaint.
            hDC = GetDC(hWnd);

            HBRUSH brush = CreateSolidBrush(RGB(151, 0, 0));

            FillRect(hDC, &rcUp, brush);

            // Free resources
            ReleaseDC(hWnd, hDC);
            DeleteObject(brush);
        }
        break;

        case VK_DOWN:
        {
            // Process the DOWN ARROW key. 
            // All painting occurs here, between BeginPaint and EndPaint.
            hDC = GetDC(hWnd);

            HBRUSH brush = CreateSolidBrush(RGB(151, 0, 0));

            FillRect(hDC, &rcDown, brush);

            // Free resources
            ReleaseDC(hWnd, hDC);
            DeleteObject(brush);
        }
        break;

        case VK_END:
        {
            // Process the END key. 
        }
        break;

        default:
            break;
        }
    }
    break;
    case WM_KEYUP:
    {
        switch (wParam)
        {
        case VK_LEFT:
        {
            // Process the LEFT ARROW key. 
            // All painting occurs here, between BeginPaint and EndPaint.
            hDC = GetDC(hWnd);

            FillRect(hDC, &rcLeft, hbrBkgnd);

            // Free resources
            ReleaseDC(hWnd, hDC);
        }
        break;

        case VK_RIGHT:
        {
            // Process the RIGHT ARROW key. 
            // All painting occurs here, between BeginPaint and EndPaint.
            hDC = GetDC(hWnd);

            FillRect(hDC, &rcRight, hbrBkgnd);

            // Free resources
            ReleaseDC(hWnd, hDC);
        }
        break;

        case VK_UP:
        {
            // Process the UP ARROW key. 
            // All painting occurs here, between BeginPaint and EndPaint.
            hDC = GetDC(hWnd);

            FillRect(hDC, &rcUp, hbrBkgnd);

            // Free resources
            ReleaseDC(hWnd, hDC);
        }
        break;

        case VK_DOWN:
        {
            // Process the DOWN ARROW key. 
            // All painting occurs here, between BeginPaint and EndPaint.
            hDC = GetDC(hWnd);

            FillRect(hDC, &rcDown, hbrBkgnd);

            // Free resources
            ReleaseDC(hWnd, hDC);
        }
        break;

        case VK_END:
        {
            // Process the END key. 
        }
        break;

        default:
            break;
        }
    }
    break;
    case WM_DESTROY:
        // Delete/free brushes
        DeleteObject(hbrBkgnd);

        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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
