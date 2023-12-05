#ifndef VEC_HPP
#define VEC_HPP
#include <cmath>
namespace game::vec
{
    struct Vec2
    {
        float x;
        float y;
        Vec2(float x, float y) : x(x), y(y){};
        Vec2() : x(0), y(0){};
        Vec2 operator+(const Vec2 &other) const
        {
            return Vec2(x + other.x, y + other.y);
        }
        Vec2 operator-(const Vec2 &other) const
        {
            return Vec2(x - other.x, y - other.y);
        }
        Vec2 operator+(const float scalar)
        {
            return Vec2(x + scalar, y + scalar);
        }
        Vec2 operator-(const float scalar)
        {
            return Vec2(x - scalar, y - scalar);
        }
        Vec2 operator*(const float scalar)
        {
            return Vec2(x * scalar, y * scalar);
        }
        Vec2 operator/(const float scalar)
        {
            return Vec2(x / scalar, y / scalar);
        }
        float getAngleDeg()
        {
            return atan2(y, x) * 180 / M_PI;
        }
    };
}
#endif