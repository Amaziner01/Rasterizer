#pragma once

#define UNICODE
#include <windows.h>

#include <functional>

#include "types.hh"
#include "window.hh"

class Renderer
{
    public:
        Renderer() = default;
        Renderer(Window* window);
        ~Renderer();
        void Refresh();
        void Clear(u32_t color);
        void DrawPoint(int x, int y, u32_t color);
        void DrawLine(int x0, int y0, int x1, int y1, u32_t color);
        void DrawArray(void *verts, size_t triangle_count, u32_t color);
        void DrawIndices(void *verts, void *inds, size_t triangle_count, u32_t color);
        [[nodiscard]] u32_t *GetBufferPtr();


    private:
        Window                    *m_win;
        HDC                       m_dc;
        HDC                       m_memdc;
        HBITMAP                   m_bmp;
        BITMAPINFO                m_bi;
        u32_t                     *m_buffer;
        int                       m_width;
        int                       m_height;
};