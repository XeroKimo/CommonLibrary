#pragma once
#include "WindowsHelper.h"

namespace CommonsLibrary
{
    class Client
    {
    private:
        HWND m_windowHandle = nullptr;
        HINSTANCE m_hInstance = nullptr;
#ifdef _MBCS
        TDSTR m_className = "";
#elif _UNICODE
        TDSTR m_className = L"";
#endif // _MBCS
    public:
        Client() = default;
        Client(const Client& other) = delete;
        Client(Client&& other) noexcept
        {
            m_windowHandle = other.m_windowHandle;
            m_hInstance = other.m_hInstance;
            m_className = other.m_className;

            other.m_windowHandle = nullptr;
            other.m_hInstance = nullptr;
            other.m_className = nullptr;
        }

        Client(WindowClass winClass, CreateWindowHelper windowOptions)
        {
            Initialize(winClass, windowOptions);
        }
        Client(WindowClassExtended winClass, CreateWindowHelper windowOptions)
        {
            Initialize(winClass, windowOptions);
        }
        Client(WindowClass winClass, CreateWindowHelperExtended windowOptions)
        {
            Initialize(winClass, windowOptions);
        }
        Client(WindowClassExtended winClass, CreateWindowHelperExtended windowOptions)
        {
            Initialize(winClass, windowOptions);
        }
        ~Client()
        {
            DestroyWindow(m_windowHandle);
            UnregisterClass(m_className, m_hInstance);
        }

    public:
        void operator=(const Client& other) = delete;
        void operator=(Client&& other) noexcept
        {
            m_windowHandle = other.m_windowHandle;
            m_hInstance = other.m_hInstance;
            m_className = other.m_className;

            other.m_windowHandle = nullptr;
            other.m_hInstance = nullptr;
            other.m_className = nullptr;
        }

    public:

        LONG_PTR SetWindowLongPTR(Window_Long_Ptr_Index nIndex, void* ptr)
        {
            return SetWindowLongPtr(m_windowHandle, static_cast<int>(nIndex), reinterpret_cast<LONG_PTR>(ptr));
        }

        void SetWindowSize(int width, int height)
        {
            int x;
            int y;
            CenterWindow(width, height, x, y);
            SetWindowPos(m_windowHandle, nullptr, x, y, width, height, SWP_SHOWWINDOW);
        }


        HWND GetWindowHandle() const { return m_windowHandle; }
        int GetWindowWidth() const
        {
            int width, height;
            GetWindowSize(width, height);
            return width;
        }
        int GetWindowHeight() const
        {
            int width, height;
            GetWindowSize(width, height);
            return height;
        }
        void GetWindowSize(int& outWidth, int& outHeight) const
        {
            RECT rect;
            if (!GetClientRect(m_windowHandle, &rect))
                return;
            outWidth = rect.right;
            outHeight = rect.bottom;
        }
    private:
        template <class windowClass, class createWindowHelper>
        void Initialize(windowClass winClass, createWindowHelper windowOptions)
        {
            if (!Register(winClass))
            {
                MessageBox(NULL, L"Error registering class", L"ERROR", MB_OK | MB_ICONERROR);
                return;
            }
            if (!InitWindow(windowOptions))
            {
                MessageBox(NULL, L"Error intializing window", L"ERROR", MB_OK | MB_ICONERROR);
                return;
            }

            ShowWindow(m_windowHandle, SW_SHOW);
            SetForegroundWindow(m_windowHandle);
            SetFocus(m_windowHandle);
        }

        bool Register(WindowClass winClass)
        {
            return RegisterClass(&winClass);
        }

        bool Register(WindowClassExtended winClass)
        {
            return RegisterClassEx(&winClass);
        }

        bool InitWindow(CreateWindowHelper windowOptions)
        {
            m_className = windowOptions.lpClassName;
            m_hInstance = windowOptions.hInstance;
            m_windowHandle = CreateWindow
            (
                windowOptions.lpClassName,
                windowOptions.lpWindowName,
                windowOptions.dwStyle,
                windowOptions.X,
                windowOptions.Y,
                windowOptions.nWidth,
                windowOptions.nHeight,
                windowOptions.hWndParent,
                windowOptions.hMenu,
                windowOptions.hInstance,
                windowOptions.lpParam
            );
            return m_windowHandle != nullptr;
        }
        bool InitWindow(CreateWindowHelperExtended windowOptions)
        {
            m_className = windowOptions.lpClassName;
            m_hInstance = windowOptions.hInstance;
            m_windowHandle = CreateWindowEx
            (
                windowOptions.dwExStyle,
                windowOptions.lpClassName,
                windowOptions.lpWindowName,
                windowOptions.dwStyle,
                windowOptions.X,
                windowOptions.Y,
                windowOptions.nWidth,
                windowOptions.nHeight,
                windowOptions.hWndParent,
                windowOptions.hMenu,
                windowOptions.hInstance,
                windowOptions.lpParam
            );
            return m_windowHandle != nullptr;
        }
    };
}