#pragma once

#include <cmath>

struct vec3
{
    float x, y, z;
    vec3 operator+(const vec3& v) const;
    vec3 operator-(const vec3& v) const;
    vec3 operator*(const vec3& v) const;
    vec3 operator/(const vec3& v) const;
    vec3 Cross(const vec3& v);
    float Dot(const vec3& v);
    vec3 Normalize();
};

struct mat3
{
    vec3 r1;
    vec3 r2;
    vec3 r3;

    inline vec3 operator*(const vec3& v) const;

    static mat3 RotateZ(float angle)
    {
        return {
            { cosf(angle), sinf(angle), 0.0f},
            {-sinf(angle), cosf(angle), 0.0f},
            {0.0f,        0.0f,       1.0f}
        };
    }

    static mat3 RotateY(float angle)
    {
        return {
            { cosf(angle),  0.0f, -sinf(angle)},
            { 0.0f,        1.0f, 0.0f      },
            { sinf(angle), 0.0f, cosf(angle)},
        };
    }

    static mat3 RotateX(float angle)
    {
        return {
            { 1.0f, 0.0f,       0.0f      },
            { 0.0f, cosf(angle), sinf(angle)},
            { 0.0f,-sinf(angle), cosf(angle)},
        };
    }

    static mat3 Tranlate(const vec3& v)
    {
        return {
            {1.0f, 0.0f, v.x},
            {0.0f, 1.0f, v.y},
            {0.0f, 0.0f, v.z}
        };
    }

    static mat3 Scale(const vec3& v)
    {
        return {
            {v.x, 0.0f, 0.0f},
            {0.0f, v.y, 0.0f},
            {0.0f, 0.0f, v.z}
        };
    }
};

inline vec3 vec3::operator+(const vec3& v) const
{
    return {
        x + v.x,
        y + v.y,
        z + v.z
    };
}

inline vec3 vec3::operator-(const vec3& v) const
{
    return {
        x - v.x,
        y - v.y,
        z - v.z
    };
}

inline vec3 vec3::operator*(const vec3& v) const
{
    return {
        x * v.x,
        y * v.y,
        z * v.z
    };
}
inline vec3 mat3::operator*(const vec3& v) const
{
    return {
        r1.x * v.x + r1.y * v.y + r1.z * v.z,
        r2.x * v.x + r2.y * v.y + r2.z * v.z,
        r3.x * v.x + r3.y * v.y + r3.z * v.z
    };
}

inline vec3 operator*(const vec3& v, const mat3& m)
{
    return {
        (m.r1.x * v.x) + (m.r1.y * v.y) + (m.r1.z * v.z),
        (m.r2.x * v.x) + (m.r2.y * v.y) + (m.r2.z * v.z),
        (m.r3.x * v.x) + (m.r3.y * v.y) + (m.r3.z * v.z)
    };
}

inline vec3 vec3::operator/(const vec3& v) const
{
    return {
        x / v.x,
        y / v.y,
        z / v.z
    };
}


inline vec3 vec3::Cross(const vec3& v)
{
    return {
        y * v.z - z * v.y,
        x * v.z - z * v.x,
        x * v.y - y * v.x,
    };
}   

inline float vec3::Dot(const vec3& v)
{
    return { x*v.x + y*v.y + z*v.z};
}


#define sqr(a) ((a)*(a))
inline vec3 vec3::Normalize()
{
    float dist = sqrt(sqr(x) + sqr(y) + sqr(z));
    return {x / dist, y / dist, z / dist};
}