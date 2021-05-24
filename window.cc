#include "window.hh"

LRESULT Window::wnd_proc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    Window *pThis;

    if (msg == WM_NCCREATE)
    {
        pThis = static_cast<Window*>(reinterpret_cast<CREATESTRUCT*>(lparam)->lpCreateParams);

        SetLastError(0);
        if (!SetWindowLongPtr(wnd, -21, reinterpret_cast<LONG_PTR>(pThis)))
        {
            if (GetLastError() != 0)
                return FALSE;
        }
    }
    else
    {
        pThis = reinterpret_cast<Window*>(GetWindowLongPtr(wnd, -21));
    }

    switch (msg)
    {
        case WM_DESTROY:
        case WM_CLOSE:
            PostQuitMessage(0);
            break;

        case WM_SIZE:
            if (pThis)
            {
                RECT r;
                GetClientRect(wnd, &r);

                pThis->m_width = r.right - r.left;
                pThis->m_height = r.bottom - r.top;

                pThis->m_resized = true;
                
            }
            break;
        
        default: return DefWindowProcW(wnd, msg, wparam, lparam);
    }

    return (LRESULT)0;
}

Window::Window(int width, int height, const std::wstring &name)
{
    WNDCLASS w_class = {0};
    w_class.style = CS_OWNDC;
    w_class.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    w_class.hInstance = GetModuleHandleW(nullptr);
    w_class.lpszClassName = L"Renderer";
    w_class.lpfnWndProc = wnd_proc;

    RegisterClassW(&w_class);

    m_win = CreateWindowExW(
        0,
        w_class.lpszClassName,
        name.c_str(),
        WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_OVERLAPPED,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width,
        height,
        nullptr,
        nullptr,
        w_class.hInstance,
        this
    );

    ShowWindow(m_win, SW_SHOW);
    UpdateWindow(m_win);

    RECT r;
    GetClientRect(m_win, &r);

    m_width = r.right - r.left;
    m_height = r.bottom - r.top;

    m_running = true;
    m_resized = false;
}

Window::~Window()
{
    DestroyWindow(m_win);
}

void Window::PollMessage()
{ 
    if (PeekMessageW(&m_msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&m_msg);
        DispatchMessageW(&m_msg);
        if (m_msg.message == WM_QUIT) m_running = false;
    }
}

[[nodiscard]] bool Window::Running() const { return m_running; }

[[nodiscard]] HWND Window::GetHandle() const { return m_win; }

[[nodiscard]] int Window::GetWidth() const { return m_width; }

[[nodiscard]] int Window::GetHeight() const { return m_height; }

[[nodiscard]] bool Window::HaveResized()
{
    if (m_resized)
    {
        m_resized = false;
        return true;
    }

    return false;
}