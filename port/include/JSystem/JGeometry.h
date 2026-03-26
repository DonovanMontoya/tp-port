#pragma once

#include "port/types.h"

namespace JGeometry {

template <typename T>
struct TVec3 {
    T x;
    T y;
    T z;

    TVec3() : x(0), y(0), z(0) {}
    TVec3(T ix, T iy, T iz) : x(ix), y(iy), z(iz) {}
};

}  // namespace JGeometry
