#include <iostream>
#include <cstdlib>
#include <algorithm>

#include "window.hh"
#include "renderer.hh"
#include "math.hh"

#define COLOR(r, g, b) ( 1 | ((r) << 16) | ((g) << 8) | (b))

template<typename T>
void TransformVertices(void *in, void* out, size_t count, T op)
{
    vec3 *vin = reinterpret_cast<vec3*>(in);
    vec3 *vout = reinterpret_cast<vec3*>(out);
    for (size_t i = 0; i < count; i++)
    {
        vout[i] = op(vin[i]);
    }
}

int main() {
    std::cout << "Pinga!\n";

    Window window(CW_USEDEFAULT, CW_USEDEFAULT, L"Cool Window");
    Renderer renderer(&window);

    float vertices[] = {
         0.5, -0.5, -0.5,
         0.5, -0.5,  0.5,
        -0.5, -0.5,  0.5,
        -0.5, -0.5, -0.5,
         0.5,  0.5, -0.5,
         0.5,  0.5,  0.5,
        -0.5,  0.5,  0.5,
        -0.5,  0.5, -0.5
    };

    u32_t indices[] = {
        0, 2, 3,
        0, 1, 2,
        0, 5, 4,
        0, 1, 5,
        4, 0, 3,
        4, 3, 7,
        4, 5, 6,
        6, 7, 4,
        5, 1, 2,
        2, 6, 5,
        3, 2, 6,
        3, 6, 7
    };

    float transformedVertices[sizeof(vertices) / sizeof(vertices[0])] = {0};


    LARGE_INTEGER begin, end, freq;
    POINT previous, now;
    u8_t keys[255];


    vec3 position = {0, 0, 100};
    float anglex = 0.0f, angley = 0.0f;


#define MAX_CUBES 500

    vec3 positions[MAX_CUBES];
    float angles[MAX_CUBES];
    u32_t colors[MAX_CUBES];

    for (int i = 0; i < MAX_CUBES; i++)
    {
        positions[i] = (vec3){
            (float)((rand() % 50) - 25),
            (float)((rand() % 50) - 25),
            (float)((rand() % 50) - 25)
        };

        angles[i] = (float)(rand() % 100);
        colors[i] = COLOR(rand() % 255, rand() % 255, rand() % 255);
    }


    double dt = 1.0;

    while (window.Running())
    {
        QueryPerformanceCounter(&begin);

        auto keyPressed = [keys](u8_t key) {
            return keys[key] & 0x80;
        };

        GetCursorPos(&previous);
        GetKeyboardState(keys);

    #define MOVEMENT_SPEED 15
    #define PI 3.1416

        if (keyPressed(0x41))
        {
            position.x += MOVEMENT_SPEED * sin(-anglex + (PI/2)) * dt;
            position.z -= MOVEMENT_SPEED * cos(-anglex + (PI/2)) * dt;
        }

        if (keyPressed(0x44))
        {
            position.x += MOVEMENT_SPEED * sin(-anglex - (PI/2)) * dt;
            position.z -= MOVEMENT_SPEED * cos(-anglex - (PI/2)) * dt;
        }

        if (keyPressed(0x53))
        {
            position.x += MOVEMENT_SPEED * sin(anglex) * dt;
            position.z += MOVEMENT_SPEED * cos(anglex) * cos(angley) * dt;
            position.y -= MOVEMENT_SPEED * sin(angley) * dt;
        }

        if (keyPressed(0x57)) 
        {
            position.x += MOVEMENT_SPEED * sin(-anglex) * dt;
            position.z -= MOVEMENT_SPEED * cos(-anglex) * cos(angley) * dt;
            position.y += MOVEMENT_SPEED * sin(angley) * dt;
        }
        

        renderer.Clear(COLOR(10, 30, 180));


        int j = 0;
        for (int i = 0; i < 50; i++)
        {
            auto transform = [&](vec3 v) {

                v = (((v + (vec3){0, 0, 1}) * mat3::RotateY(angles[i]) * mat3::RotateX(angles[i])) + positions[i] + position) * mat3::RotateY(anglex) * mat3::RotateX(angley);
                //v = (((v + (vec3){0, 0, 10}) + position) * mat3::RotateY(anglex) * mat3::RotateX(angley));

                if (v.z > 0)
                {
                    v.x = v.x / (v.z * 0.001) + window.GetWidth() / 2;
                    v.y = v.y / (v.z * 0.001) + window.GetHeight() / 2;
                    v.z = v.z / (v.z * 0.001);
                }

                return v;
            };

            //angles[i] += 1 * dt;

            TransformVertices(vertices, transformedVertices, 8, transform);
            if (j < 10) 
            {
                renderer.DrawIndices(transformedVertices, indices, sizeof(indices) / 32 * 3, colors[i]);
            }

            j++;
        }

        window.PollMessage();
        renderer.Refresh();

        GetCursorPos(&now);

        if (keyPressed(0x01))
        {
            anglex += (now.x - previous.x) * 0.4 * dt;
            angley -= (now.y - previous.y) * 0.4 * dt;
        }
        

        QueryPerformanceCounter(&end);
        QueryPerformanceFrequency(&freq);

        dt = (double)(end.QuadPart - begin.QuadPart) / freq.QuadPart;
    }

    return 0;
}
