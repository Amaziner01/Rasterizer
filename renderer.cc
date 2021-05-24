#include "renderer.hh"
#include "math.hh"

Renderer::Renderer(Window* window)
{
    m_width = window->GetWidth();
    m_height = window->GetHeight();

    m_win = window;
    m_dc = GetDC(window->GetHandle());
    m_memdc = CreateCompatibleDC(m_dc);
    m_bmp = CreateCompatibleBitmap(m_dc, m_width, m_height);

    SelectObject(m_memdc, m_bmp);
    
    m_bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    m_bi.bmiHeader.biCompression = BI_RGB;
    m_bi.bmiHeader.biHeight = -m_height;
    m_bi.bmiHeader.biWidth = m_width;
    m_bi.bmiHeader.biBitCount = 32;
    m_bi.bmiHeader.biPlanes = 1;

    m_buffer = new u32_t[m_width * m_height];
}

Renderer::~Renderer()
{
    delete[] m_buffer;
    DeleteDC(m_memdc);
    DeleteObject(m_bmp);
    ReleaseDC(m_win->GetHandle(), m_dc);
}

void Renderer::Refresh()
{
    bool resized = m_win->HaveResized();
    if (resized)
    {
        m_width = m_win->GetWidth();
        m_height = m_win->GetHeight();

        delete[] m_buffer;
        m_buffer = new u32_t[m_width * m_height];

        m_bi.bmiHeader.biWidth = m_width;
        m_bi.bmiHeader.biHeight = -m_height;

        ReleaseDC(m_win->GetHandle(), m_dc);
        m_dc = GetDC(m_win->GetHandle());

        RECT newRect = {0, 0, m_width, m_height};
        InvalidateRect(m_win->GetHandle(), &newRect, false);

        std::printf("\rw:%d h:%d    ", m_width, m_height);    
    }

    int w = m_width;
    int h = m_height;

    SetDIBits(nullptr, m_bmp, 0, h, m_buffer, &m_bi, DIB_RGB_COLORS);
    BitBlt(m_dc, 0, 0, w, h, m_memdc, 0, 0, SRCCOPY);

}

void Renderer::Clear(u32_t color)
{
    for (int i = 0; i < m_width * m_height; i++)
    {
        m_buffer[i] = color;
    }
}

void Renderer::DrawPoint(int x, int y, u32_t color)
{
    if ((x | y) < 0) return;
    if (x > m_width - 1) return;
    if (y > m_height - 1) return;

    m_buffer[y * m_width + x] = color;
}

#define lerp(a, b, t) ((a) + (((b) - (a))) * (t))

#define abs(a) ((a) < 0 ? -(a) : (a))

void Renderer::DrawLine(int x0, int y0, int x1, int y1, u32_t color)
{
    if (x0 < 0 && y0 < 0 && x1 < 0 && y1 < 0) return;
    if (x0 > m_width && y0 > m_height && x1 > m_width && y1 > m_height) return;

    int dy = abs(y1 - y0);
    int dx = abs(x1 - x0);

    int n, x, y;

    if (dx > dy)
    {
        n = x0 < x1 ? 1 : -1;
        x = x0;
        for (int i = 0; i < dx; i ++)
        {
            int y = lerp(y0, y1, (float) i / dx);
            DrawPoint(x, y, color);
            x += n;
        }
    }
    else
    {
        n = y0 < y1 ? 1 : -1;
        y = y0;
        for (int i = 0; i < dy; i ++)
        {
            int x = lerp(x0, x1, (float) i / dy);
            DrawPoint(x, y, color);
            y += n;
        }
    }
}

void Renderer::DrawArray(void *verts, size_t triangle_count, u32_t color)
{
    vec3 *ptr = reinterpret_cast<vec3*>(verts);
    for (size_t i = 0; i < triangle_count; i++)
    {
        DrawLine(ptr[0].x, ptr[0].y, ptr[1].x, ptr[1].y, color);
        DrawLine(ptr[1].x, ptr[1].y, ptr[2].x, ptr[2].y, color);
        DrawLine(ptr[2].x, ptr[2].y, ptr[0].x, ptr[0].y, color);

        ptr += 1;
    }
}

void Renderer::DrawIndices(void *verts, void *inds, size_t triangle_count, u32_t color)
{
    vec3 *ptr = reinterpret_cast<vec3*>(verts);
    u32_t *ind = reinterpret_cast<u32_t*>(inds);

    

        

    for (size_t i = 0; i < triangle_count; i++)
    {
        
        if ( ptr[ind[0]].z < 1 || ptr[ind[1]].z < 1 || ptr[ind[2]].z < 1)
        {
            ind += 3;
            continue;
        }        

        DrawLine(ptr[ind[0]].x, ptr[ind[0]].y, ptr[ind[1]].x, ptr[ind[1]].y, color);
        DrawLine(ptr[ind[1]].x, ptr[ind[1]].y, ptr[ind[2]].x, ptr[ind[2]].y, color);
        DrawLine(ptr[ind[2]].x, ptr[ind[2]].y, ptr[ind[0]].x, ptr[ind[0]].y, color);
        ind += 3;
    }
}   

[[nodiscard]] u32_t *Renderer::GetBufferPtr() { return m_buffer; }

[[nodiscard]] HDC Renderer::GetDirectContext() { return m_dc; }