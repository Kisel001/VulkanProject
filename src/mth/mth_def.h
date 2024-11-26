/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        mth_def.h
 * PROGRAMMER:  IP5
 * LAST UPDATE: 22.07.2024
 * PURPOSE:     Base math defines header file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#ifndef __mth_def_h_
#define __mth_def_h_

#include <cmath>
#include <cstring>
#include <iostream>
#include <cassert>

/* Intel instrinsics functions library */
#include <intrin.h>

#ifndef WIN32
#define WIN32
#include <commondf.h>
#undef WIN32
#else  // !WIN32
#include <commondf.h>
#endif // !WIN32

/* Short version of float types */
typedef float FLT;
typedef double DBL;

/* Base mathematic constants */
#define E         2.71828182845904523536 
#define LOG2E     1.44269504088896340736 
#define LOG10E    0.434294481903251827651
#define LN2       0.693147180559945309417
#define LN10      2.30258509299404568402 
#define PI        3.14159265358979323846 
#define PI_2      1.57079632679489661923 
#define PI_4      0.785398163397448309616
#define D1_PI     0.318309886183790671538
#define D2_PI     0.636619772367581343076
#define D2_SQRTPI 1.12837916709551257390 
#define SQRT2     1.41421356237309504880 
#define SQRT1_2   0.707106781186547524401
#define PI_180    0.017453292519943295769
#define D180_PI   57.29577951308232087679

/* Convert angle from Degree to Radians */
#define D2R(A) ((A) * (PI_180))
#define Degree2Radian(a) D2R(a)

/* Convert angle from Radians to Degree */
#define R2D(A) ((A) * (D180_PI))
#define Radian2Degree(a) D2R(a)

/* Special flag for set status of using mth float intrin library */
#define USE_MTH_FLOAT_INTRIN_LIB_FLAG 0

#if defined(_MSC_VER) && defined(USE_MTH_FLOAT_INTRIN_LIB_FLAG) && (USE_MTH_FLOAT_INTRIN_LIB_FLAG == 1)
#  define USE_MTH_FLOAT_INTRIN_LIB
#endif // _MSC_VER && USE_MTH_FLOAT_INTRIN_LIB_FLAG == 1

/* Space math namespace */
namespace mth
{
  template<typename Type> class vec2;
  template<typename Type> class vec3;
  template<typename Type> class vec4;
  template<typename Type> class matr;
  template<typename Type> class ray;
  template<typename Type> class camera;
} /* end of 'mth' namespace */

#endif // !__mth_def_h_

/* END OF 'mth_def.h' FILE */
