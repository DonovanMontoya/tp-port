#pragma once

#include "port/types.h"

namespace JGeometry {

template <typename T>
struct TVec2 {
    T x;
    T y;

    TVec2() : x(0), y(0) {}
    TVec2(T ix, T iy) : x(ix), y(iy) {}

    template <typename U>
    void set(const TVec2<U>& other) {
        x = static_cast<T>(other.x);
        y = static_cast<T>(other.y);
    }

    void set(T ix, T iy) {
        x = ix;
        y = iy;
    }
};

template <typename T>
struct TVec3 {
    T x;
    T y;
    T z;

    TVec3() : x(0), y(0), z(0) {}
    TVec3(T ix, T iy, T iz) : x(ix), y(iy), z(iz) {}

    template <typename U>
    TVec3(const U& other) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)),
                            z(static_cast<T>(other.z)) {}

    template <typename U>
    void set(const TVec3<U>& other) {
        x = static_cast<T>(other.x);
        y = static_cast<T>(other.y);
        z = static_cast<T>(other.z);
    }

    void set(T ix, T iy, T iz) {
        x = ix;
        y = iy;
        z = iz;
    }

    template <typename U>
    TVec3& operator=(const U& other) {
        x = static_cast<T>(other.x);
        y = static_cast<T>(other.y);
        z = static_cast<T>(other.z);
        return *this;
    }

    TVec3 operator+(const TVec3& other) const {
        return TVec3(x + other.x, y + other.y, z + other.z);
    }

    void add(const TVec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
    }

    void scale(T factor) {
        x *= factor;
        y *= factor;
        z *= factor;
    }

    bool isZero() const { return x == 0 && y == 0 && z == 0; }
};

template <typename T>
struct TBox2 {
    TVec2<T> i;
    TVec2<T> f;

    TBox2() : i(0, 0), f(0, 0) {}
    TBox2(T left, T top, T right, T bottom) : i(left, top), f(right, bottom) {}

    T getWidth() const { return f.x - i.x; }
    T getHeight() const { return f.y - i.y; }
};

}  // namespace JGeometry
