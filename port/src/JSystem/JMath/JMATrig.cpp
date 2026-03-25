/**
 * port/src/JSystem/JMath/JMATrig.cpp
 * Instantiates the JMath trig lookup tables (sincos, atan, asin/acos).
 * The templates live in JMATrigonometric.h; this TU creates the objects.
 */
#include "JSystem/JMath/JMATrigonometric.h"

namespace JMath {
    TSinCosTable<13, f32>        sincosTable_;
    TAtanTable<1024, f32>        atanTable_;
    TAsinAcosTable<1024, f32>    asinAcosTable_;
}
