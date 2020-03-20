#pragma once
#include <Windows.h>
#include "CommonsLibrary/Miscellaneous/BitMaskEnum.h"

namespace CommonsLibrary
{
#ifdef _MBCS
    using TDSTR = LPCSTR;
#elif _UNICODE
    using TDSTR = LPCWSTR;
#endif // _MBCS

    //https://docs.microsoft.com/en-us/windows/win32/winmsg/window-styles
    enum class Window_Style : long long
    {
        Border = WS_BORDER,
        Caption = WS_CAPTION,
        Child = WS_CHILD,
        Child_Window = WS_CHILDWINDOW,
        Clip_Children = WS_CLIPCHILDREN,
        Clip_Siblings = WS_CLIPSIBLINGS,
        Disabled = WS_DISABLED,
        Dialog_Frame = WS_DLGFRAME,
        Group = WS_GROUP,
        Horizontal_Scroll = WS_HSCROLL,
        Iconic = WS_ICONIC,
        Maximize = WS_MAXIMIZE,
        Maximize_Box = WS_MAXIMIZEBOX,
        Minimize = WS_MINIMIZE,
        Minimize_Box = WS_MINIMIZEBOX,
        Overlapped = WS_OVERLAPPED,
        Overlapped_Window = WS_OVERLAPPEDWINDOW,
        Pop_Up = WS_POPUP,
        Pop_Up_Window = WS_POPUPWINDOW,
        Size_Box = WS_SIZEBOX,
        System_Menu = WS_SYSMENU,
        Tab_Stop = WS_TABSTOP,
        Thick_Frame = WS_THICKFRAME,
        Tiled = WS_TILED,
        Tiled_Window = WS_TILEDWINDOW,
        Visible = WS_VISIBLE,
        Vertical_Scroll = WS_VSCROLL,
    };

    //https://docs.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
    enum class Window_Style_Extended : long long
    {
        None = 0,
        Accept_Files = WS_EX_ACCEPTFILES,
        App_Window = WS_EX_APPWINDOW,
        Client_Edge = WS_EX_CLIENTEDGE,
        Composited = WS_EX_COMPOSITED,
        Context_Help = WS_EX_CONTEXTHELP,
        Control_Parent = WS_EX_CONTROLPARENT,
        Dialog_Modal_Frame = WS_EX_DLGMODALFRAME,
        Layered = WS_EX_LAYERED,
        Layout_Right_To_Left = WS_EX_LAYOUTRTL,
        Left = WS_EX_LEFT,
        Left_Scroll_Bar = WS_EX_LEFTSCROLLBAR,
        Left_To_Right_Reading = WS_EX_LTRREADING,
        MDI_Child = WS_EX_MDICHILD,
        No_Activate = WS_EX_NOACTIVATE,
        No_Inherit_Layout = WS_EX_NOINHERITLAYOUT,
        No_Parent_Notify = WS_EX_NOPARENTNOTIFY,
        No_Redirection_Bitmap = WS_EX_NOREDIRECTIONBITMAP,
        Overlapped_Window = WS_EX_OVERLAPPEDWINDOW,
        Palette_Window = WS_EX_PALETTEWINDOW,
        Right = WS_EX_RIGHT,
        Right_Scroll_Bar = WS_EX_RIGHTSCROLLBAR,
        Right_To_Left_Reading = WS_EX_RTLREADING,
        Static_Edge = WS_EX_STATICEDGE,
        Tool_Window = WS_EX_TOOLWINDOW,
        Top_Most = WS_EX_TOPMOST,
        Transparent = WS_EX_TRANSPARENT,
        Window_Edge = WS_EX_WINDOWEDGE
    };

    //https://docs.microsoft.com/en-us/windows/win32/winmsg/window-class-styles
    enum class Window_Class_Style : long long
    {
        Byte_Align_Client = CS_BYTEALIGNCLIENT,
        Byte_Align_Window = CS_BYTEALIGNWINDOW,
        Class_Device_Context = CS_CLASSDC,
        Double_Clicks = CS_DBLCLKS,
        Drop_Shadow = CS_DROPSHADOW,
        Global_Class = CS_GLOBALCLASS,
        Horizontal_Redraw = CS_HREDRAW,
        No_Close = CS_NOCLOSE,
        Own_Device_Context = CS_OWNDC,
        Parent_Device_Context = CS_PARENTDC,
        Save_Bits = CS_SAVEBITS,
        Vertical_Redraw = CS_VREDRAW
    };

    enum class Window_Long_Ptr_Index
    {
        Extended_Style = GWL_EXSTYLE,
        Instance_Handle = GWLP_HINSTANCE,
        ID = GWLP_ID,
        Style = GWL_STYLE,
        User_Data = GWLP_USERDATA,
        Window_Prodcedure = GWLP_WNDPROC
    };

    //Resizes window so that the drawable portion of the window matches the requested size
    void ResizeToClientSize(HWND hwnd, int requestedWidth, int requestedHeight, int& outAdjustedWidth, int& outAdjustedHeight);
    //Resizes window so that the drawable portion of the window matches the requested size
    void ResizeToClientSize(DWORD dwStyle, int requestedWidth, int requestedHeight, int& outAdjustedWidth, int& outAdjustedHeight);
    //Resizes window so that the drawable portion of the window matches the requested size
    void ResizeToClientSize(DWORD dwStyle, DWORD dwStyleEX, int requestedWidth, int requestedHeight, int& outAdjustedWidth, int& outAdjustedHeight);
    void CenterWindow(int windowWidth, int windowHeight, int& outX, int& outY);

    struct CreateWindowHelper
    {
        TDSTR    lpClassName;
        TDSTR    lpWindowName;
        DWORD     dwStyle;
        int       X;
        int       Y;
        int       nWidth;
        int       nHeight;
        HWND      hWndParent;
        HMENU     hMenu;
        HINSTANCE hInstance;
        LPVOID    lpParam;

        CreateWindowHelper() = default;

        CreateWindowHelper(TDSTR className, TDSTR windowName, Window_Style style, int posX, int posY, int width, int height, HWND parent, HMENU menu, HINSTANCE instanceHandle, LPVOID param = nullptr)
        {
            lpClassName = className;
            lpWindowName = windowName;
            dwStyle = static_cast<DWORD>(style);
            X = posX;
            Y = posY;
            nWidth = width;
            nHeight = height;
            hWndParent = parent;
            hMenu = menu;
            hInstance = instanceHandle;
            lpParam = param;
        }

        static CreateWindowHelper Standard(HINSTANCE hInstance, int windowWidth, int windowHeight, TDSTR lpClassName)
        {
            int x, y;
            CenterWindow(windowWidth, windowHeight, x, y);

            CreateWindowHelper helper =
            {
                lpClassName,                     // Window class
                lpClassName,    // Window text
                Window_Style::Overlapped | Window_Style::Caption | Window_Style::System_Menu | Window_Style::Minimize_Box | Window_Style::Maximize_Box,           // Window style
                x, y,	// Position
                windowWidth, windowHeight,					// Size
                NULL,							// Parent window    
                NULL,							// Menu
                hInstance,					// Instance handle
            };

            return helper;
        }

        static CreateWindowHelper Borderless(HINSTANCE hInstance, int windowWidth, int windowHeight, TDSTR lpClassName)
        {
            int x, y;
            CenterWindow(windowWidth, windowHeight, x, y);

            CreateWindowHelper helper =
            {
                lpClassName,                     // Window class
                lpClassName,    // Window text
                Window_Style::Pop_Up,            // Window style
                x, y,	// Position
                windowWidth, windowHeight,					// Size
                NULL,							// Parent window    
                NULL,							// Menu
                hInstance,					// Instance handle
            };

            return helper;
        }
    };

    struct CreateWindowHelperExtended
    {
        DWORD     dwExStyle;
        TDSTR    lpClassName;
        TDSTR    lpWindowName;
        DWORD     dwStyle;
        int       X;
        int       Y;
        int       nWidth;
        int       nHeight;
        HWND      hWndParent;
        HMENU     hMenu;
        HINSTANCE hInstance;
        LPVOID    lpParam;

        CreateWindowHelperExtended() = default;

        CreateWindowHelperExtended(Window_Style_Extended styleEX, TDSTR className, TDSTR windowName, Window_Style style, int posX, int posY, int width, int height, HWND parent, HMENU menu, HINSTANCE instanceHandle, LPVOID param = nullptr)
        {
            dwExStyle = static_cast<DWORD>(styleEX);
            lpClassName = className;
            lpWindowName = windowName;
            dwStyle = static_cast<DWORD>(style);
            X = posX;
            Y = posY;
            nWidth = width;
            nHeight = height;
            hWndParent = parent;
            hMenu = menu;
            hInstance = instanceHandle;
            lpParam = param;
        }


        static CreateWindowHelperExtended Standard(HINSTANCE hInstance, int windowWidth, int windowHeight, TDSTR lpClassName)
        {
            int x, y;
            CenterWindow(windowWidth, windowHeight, x, y);

            CreateWindowHelperExtended helper =
            {
                Window_Style_Extended::None,                           // Optional window styles.
                lpClassName,                     // Window class
                lpClassName,    // Window text
                Window_Style::Overlapped | Window_Style::Caption | Window_Style::System_Menu | Window_Style::Minimize_Box | Window_Style::Maximize_Box,          // Window style
                x, y,	// Position
                windowWidth, windowHeight,					// Size
                nullptr,							// Parent window    
                nullptr,							// Menu
                hInstance,					// Instance handle
                nullptr							// Additional application data
            };

            return helper;
        }

        static CreateWindowHelperExtended Borderless(HINSTANCE hInstance, int windowWidth, int windowHeight, TDSTR lpClassName)
        {
            int x, y;
            CenterWindow(windowWidth, windowHeight, x, y);

            CreateWindowHelperExtended helper =
            {
                Window_Style_Extended::None,                           // Optional window styles.
                lpClassName,                     // Window class
                lpClassName,    // Window text
                Window_Style::Pop_Up,              // Window style
                x,y,	// Position
                windowWidth, windowHeight,					// Size
                nullptr,							// Parent window    
                nullptr,							// Menu
                hInstance,					// Instance handle
                nullptr							// Additional application data
            };

            return helper;
        }

    };

    inline void CenterWindow(int windowWidth, int windowHeight, int& outX, int& outY)
    {
        HWND window = GetDesktopWindow();
        RECT windowRect;
        GetClientRect(window, &windowRect);

        outX = (windowRect.right - windowWidth) / 2;
        outY = (windowRect.bottom - windowHeight) / 2;
    }

    //Resizes window so that the drawable portion of the window matches the requested size
    inline void ResizeToClientSize(HWND hwnd, int requestedWidth, int requestedHeight, int& outAdjustedWidth, int& outAdjustedHeight)
    {
        DWORD dwStyleEx = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
        DWORD dwStyle = GetWindowLongPtr(hwnd, GWL_STYLE);

        if (dwStyleEx == 0)
            ResizeToClientSize(dwStyle, requestedWidth, requestedHeight, outAdjustedWidth, outAdjustedHeight);
        else
            ResizeToClientSize(dwStyle, dwStyleEx, requestedWidth, requestedHeight, outAdjustedWidth, outAdjustedHeight);

    }
    //Resizes window so that the drawable portion of the window matches the requested size
    inline void ResizeToClientSize(DWORD dwStyle, int requestedWidth, int requestedHeight, int& outAdjustedWidth, int& outAdjustedHeight)
    {
        RECT rect = {};
        rect.bottom = requestedHeight;
        rect.right = requestedWidth;
        AdjustWindowRect(&rect, dwStyle, false);

        outAdjustedHeight = rect.bottom - rect.top;
        outAdjustedWidth = rect.right - rect.left;

        //X += rect.left;
        //Y += rect.top;
    }
    //Resizes window so that the drawable portion of the window matches the requested size
    inline void ResizeToClientSize(DWORD dwStyle, DWORD dwStyleEX, int requestedWidth, int requestedHeight, int& outAdjustedWidth, int& outAdjustedHeight)
    {
        RECT rect = {};
        rect.bottom = requestedHeight;
        rect.right = requestedWidth;
        AdjustWindowRectEx(&rect, dwStyle, false, dwStyleEX);

        outAdjustedHeight = rect.bottom - rect.top;
        outAdjustedWidth = rect.right - rect.left;
    }

    struct WindowClass : public WNDCLASS
    {
        WindowClass() = default;
        WindowClass(HINSTANCE hInstance, WNDPROC lpfnWndProc, TDSTR lpszClassName, HICON hIcon = LoadIcon(NULL, IDI_APPLICATION), HCURSOR hCursor = LoadCursor(NULL, IDC_ARROW))
        {
            this->style = static_cast<UINT>(Window_Class_Style::Horizontal_Redraw | Window_Class_Style::Vertical_Redraw);
            this->lpfnWndProc = lpfnWndProc;
            this->cbClsExtra = 0;
            this->cbWndExtra = 0;
            this->hInstance = hInstance;
            this->hIcon = hIcon;
            this->hCursor = hCursor;
            this->hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
            this->lpszMenuName = NULL;
            this->lpszClassName = lpszClassName;
        }
    };

    struct WindowClassExtended : public WNDCLASSEX
    {
        WindowClassExtended() = default;
        WindowClassExtended(HINSTANCE hInstance, WNDPROC lpfnWndProc, TDSTR lpszClassName, HICON hIcon = LoadIcon(NULL, IDI_APPLICATION), HCURSOR hCursor = LoadCursor(NULL, IDC_ARROW))
        {
            this->cbSize = sizeof(WNDCLASSEX);
            this->style = static_cast<UINT>(Window_Class_Style::Horizontal_Redraw | Window_Class_Style::Vertical_Redraw);
            this->lpfnWndProc = lpfnWndProc;
            this->cbClsExtra = 0;
            this->cbWndExtra = 0;
            this->hInstance = hInstance;
            this->hIcon = hIcon;
            this->hCursor = hCursor;
            this->lpszMenuName = NULL;
            this->lpszClassName = lpszClassName;
            this->hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
            this->hIconSm = NULL;
        }
    };
}