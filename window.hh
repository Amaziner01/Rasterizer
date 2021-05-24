#pragma once

#define UNICODE
#include <windows.h>

#include <iostream>

class Window
{
    public:
        Window() = default;
        Window(int width, int height, const std::wstring &name);
        ~Window();
        void PollMessage();
        [[nodiscard]] bool Running() const;
        [[nodiscard]] HWND GetHandle() const;
        [[nodiscard]] int GetWidth() const;
        [[nodiscard]] int GetHeight() const;
        [[nodiscard]] bool HaveResized();

    private:
        static LRESULT wnd_proc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
        HWND        m_win;
        HDC         m_dc;
        MSG         m_msg;
        int         m_width;
        int         m_height;
        bool        m_running;
        bool        m_resized;
};